#include <iostream>

using namespace std;

#include "Lib/Result.h"
#include "Lib/Interpreter.h"
#include "Lib/Service.h"

int main(int argc, const char *argv[]) {
    Result<Session, string> session_parsing_result = from_args(argv, argc);
    if (failure(session_parsing_result)) {
        cout << "ERROR: " << Unwrap(session_parsing_result.error_value) << endl;
        exit(EXIT_FAILURE);
    }

    Session session = Unwrap(session_parsing_result.success_value);
    Service service = Service(session);

    Maybe<const char*> maybe_error = service.authenticate(session.username, session.password);
    if (some(maybe_error)) {
        cout << "ERROR: " << Unwrap(maybe_error) << endl; 
        exit(EXIT_FAILURE);
    }

    Interpreter interpreter = Interpreter(service);

    string command;
    while(true) {
        cin >> command;
        Result<string, string> message = interpreter.interpret(command);
        if (failure(message)) {
            cout << "ERROR: " << Unwrap(message.error_value) << endl;
        } else if (success(message)) {
            cout << Unwrap(message.success_value) << endl;
        }
    }
}