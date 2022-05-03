#include <iostream>

using namespace std;

#include "Lib/Interpreter.h"
#include "Lib/Service.h"

int main(int argc, char *argv[]) {
    Session session = from_args(argv, argc);
    Service service = Service(session);
    Interpreter interpreter = Interpreter(service);
    
}