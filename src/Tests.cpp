#include "Lib/DatabaseCSV.h"
#include "Lib/User.h"

#include <iostream>

using namespace std;

int main() {
  Database<User>* db = new DatabaseCSV<User>("users.test.csv");

	auto autoriz = UserPermissionLevel::GOVERNMENT;
	auto filter = [&](const User &user) -> bool {
		return user.get_permission_level() == autoriz;
	};
  auto user_stream = db->filter_and_stream(filter);

  Maybe<User> maybe_user = None;
  while(some((maybe_user = user_stream->receive()))) {
    User user = Unwrap(maybe_user);
    cout << user.get_username() << endl;
  }

  delete user_stream;
  return 0;
}