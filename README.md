# Useful commands

- `make` Compiles the executable
- `make run` Compiles and runs the executable
- `./test.sh` Compiles and runs tests 
- `make perso` Compiles and runs `src/TestsPerso.cpp` which is git-ignored, so you can test code freely inside

# Source directories

- `src/Dependencies` Open-source third-party code we didn't write with proper credits included
- `src/Lib` Business logic
- `src/Utils` Small libraries we made to help us with the project, such as data structures and parsing
- `src/Test` Tests for both business logic and utils
- `src/` Main and Test executables

# In-program commands to try

- `login -u User0 -p 1234`

- `measurements -s Sensor0`
- `measurements -s Sensor0 -t O3`
- `measurements -s Sensor0 -ts "2019-01-21 12:00:00" -t O3`

- `flag-owner -o Provider0 -f unreliable` (permission denied)
- `login -u User1 -p 1234` (login as government)
- `flag-owner -o Provider0 -f unreliable` (again)
- `owner-flag -o Provider0`

- `quality-area -begin "2019-01-21 12:00:00" -end "2019-01-21 13:00:00" -lt 44 -lg 4.6 -rad 10`
- `quality-area -begin "2019-04-17 12:00:00" -end "2019-04-19 13:00:00" -lt 44 -lg 4.6 -rad 10`
- `quality-area -begin "2019-01-21 12:00:00" -end "2019-01-21 13:00:00" -lt 44 -lg 4.6 -rad 1`