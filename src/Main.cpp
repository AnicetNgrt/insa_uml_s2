#include "Lib/Maybe.h"
#include "Lib/Result.h"
#include <iostream>

using namespace std;

Maybe<int> divide_with_maybe(int a, int b) {
    if (b == 0) return None;
    return Some(a/b);
}

enum class DivisionError {
    DIVIDE_BY_ZERO
};

Result<int, DivisionError> divide_with_result(int a, int b) {
    if (b == 0) return Err(DivisionError::DIVIDE_BY_ZERO);
    return Ok(a/b);
}

int main() {
    {
        auto maybe = divide_with_maybe(1, 4);
        if (maybe.is_absent) return 1;
        int value = Unwrap(maybe);
    }

    {
        auto result = divide_with_result(1, 4);
        if (result.is_error) {
            switch (Unwrap(result.error_value)) {
                case DivisionError::DIVIDE_BY_ZERO: cout << "Division by zero" << endl;
            }
            return 1;
        }
        int value = Unwrap(result.success_value);
    }
    
    return 0;
}