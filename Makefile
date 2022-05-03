# Makefile inspiré de https://stackoverflow.com/a/2484343
# Compile dynamiquement selon une liste de répertoires ou "modules"
#
# Cibles
# - all : appelle build
# - build : compile le programme principal
# - build_tests : compile le programme de test
# - run : compile et lance le programme principal
# - test : compile et lance le programme de test

# Chemin vers l'exécutable principal
BIN := uml

# Options de compilation
OPTIONS := -g -Wall -ansi -pedantic -std=c++11

# Liste des sous-répertoires de src/ contenant des sources à compiler
# 	nb : Ne pas inclure les répertoires contenant des sources déclarant un main
MODULES := Lib

# Déclaration des chemins des programmes (sans le .cpp)
MAIN_PROG := Main
TEST_PROG := Tests

#-------- [Fin partie customisable] ---------------------------------------------------

# Construit les chemins des répertoires des modules
SRC_DIRS := $(addprefix src/, $(MODULES))
ENTRY_POINTS_SRC_DIRS := $(addprefix src/, $(ENTRY_POINTS_MODULES))

# Construit les chemins des répertoires des objets issus des modules
BUILD_DIRS := $(addprefix build/, $(MODULES))

# Construit la liste des chemins des sources de chaque module
SRCS := $(foreach sdir, $(SRC_DIRS), $(wildcard $(sdir)/*.cpp))
# Construit la liste des chemins des objets issus de chaque module
OBJS := $(patsubst src/%.cpp, build/%.o, $(SRCS))
# Construit la liste des fichiers de dépendances des objets issus de chaque module
DEPS := $(OBJS:%.o=%.d)

# Indique à Make que le pattern %.cpp est à rechercher dans tous les modules
# sinon il irait seulement les chercher à la racine du Makefile
vpath %.cpp $(SRC_DIRS) $(ENTRY_POINTS_SRC_DIRS)

# Fonction permettant de générer les cibles du Makefile pour compiler chaque
# fichier source de chaque module
#	nb1 : Le double $ permet à $< et $@ d'être eux-mêmes des macros
#		qui seront évaluées, au lieu d'être de simples textes collés sur place.
#	nb2 : $1 représente le chemin du répertoire des objets issu du module et
#		constitue un argument de la fonction, $< représente le nom du fichier compilé,
#		$@ le nom du fichier objet issu de la compilation.
#	nb3 : On pourrait penser que %.cpp va provoquer une génération de cible pour
#		chaque fichier .cpp du projet tout entier, sans se soucier du module concerné.
#		C'est le cas. Mais peu importe car toutes les cibles ne sont pas utilisées
#		au final. En effet, build/main ne dépend que des cibles ayant pour nom
#		un élément de $(OBJS)
#	nb4 : L'option -MMD génère un fichier .d pour chaque fichier .o qui indique la liste
#		des fichiers headers dont dépend la source compilée
define make-goal
$1/%.o: %.cpp
	g++ $(OPTIONS) -MMD -c $$< -o $$@
endef

# Cible principale
all: build

build: checkdirs $(BIN)

build_tests: checkdirs $(BIN)_tests

run: checkdirs $(BIN)
	./$(BIN)

test: checkdirs $(BIN)_tests
	./$(BIN)_tests

# Cible pour la création de l'exécutable
$(BIN): $(OBJS) build/$(MAIN_PROG).o
	g++ $(OPTIONS) $^ -o $@

$(BIN)_tests: $(OBJS) build/$(TEST_PROG).o
	g++ $(OPTIONS) $^ -o $@

build/$(MAIN_PROG).o: src/$(MAIN_PROG).cpp
	g++ $(OPTIONS) -MMD -c $< -o $@

build/$(TEST_PROG).o: src/$(TEST_PROG).cpp
	g++ $(OPTIONS) -MMD -c $< -o $@

# Cible pour la création des répertoires des objets
checkdirs: $(BUILD_DIRS)
$(BUILD_DIRS):
	@mkdir -p $@

# Cible pour la suppression des fichiers contenus dans build/
# hormis les fichiers cachés (commençant par .)
clean:
	@rm -r build/* $(BIN)

# Appel de la fonction de génération de cibles pour chaque chemin
# de répertoire des objets issus de chaque module
$(foreach bdir, $(BUILD_DIRS), $(eval $(call make-goal, $(bdir))))

# Astuce pour que les fichiers .o soient recompilés si un fichier header
# dont ils dépendent est modifié. Utilise les fichiers .d générés à la
# dernière compilation
-include $(DEPS)