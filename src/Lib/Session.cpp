#include "Session.h"

Session::Session(string username, string password,
                 IPasswordHasher &hasher,
                 unordered_map<string, string> &files) {
  // TODO
}

Session::~Session() {
  // TODO
}

string Session::get_username() const {
  // TODO
}

string Session::get_password_hash() const {
  // TODO
}

Maybe<User const *> Session::get_authed_user() const {
  // TODO
}

Maybe<fstream> Session::get_file(string key) const {
  // TODO
}

Maybe<UserFlag> Session::get_user_flag(User const *user) const {
  // TODO
}

void Session::set_authed_user(User const *user) {
  // TODO
}

void Session::set_user_flag(User const *user, UserFlag flag) {
  // TODO
}
