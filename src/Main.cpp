#include <iostream>

using namespace std;

#include "Lib/Result.h"
#include "Lib/Interpreter.h"
#include "Lib/Service.h"

int main(int argc, const char *argv[]) {
    Result<Session, string> session_parsing_result = from_args(argv, argc);
    if (session_parsing_result.is_error) {
        cout << "ERROR: " << Unwrap(session_parsing_result.error_value) << endl;
        exit(EXIT_FAILURE);
    }

    Session session = Unwrap(session_parsing_result.success_value);
    Service service = Service(session);

    Maybe<const char*> maybe_error = service.authenticate(session.username, session.password);
    if (!maybe_error.is_absent) {
        cout << "ERROR: " << Unwrap(maybe_error) << endl; 
        exit(EXIT_FAILURE);
    }

    Interpreter interpreter = Interpreter(service);

    string command;
    while(true) {
        cin >> command;
        Result<string, string> message = interpreter.interpret(command);
        if (message.is_error) {
            cout << "ERROR: " << Unwrap(message.error_value) << endl;
        } else {
            cout << Unwrap(message.success_value) << endl;
        }
    }
}