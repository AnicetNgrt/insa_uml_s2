/*
   @file Session.h
*/

#pragma once

#include "ISession.h"
#include <string>

class Session : public ISession {
public:
  Session(string username, string password, IPasswordHasher &hasher,
          unordered_map<string, string> &files);
  ~Session();

  string get_username() const override;
  string get_password_hash() const override;
  Maybe<User const &> get_authed_user() const override;
  Maybe<fstream> get_file(string key) const override;
  Maybe<UserFlag> get_user_flag(const User const &user) const override;

  void set_authed_user(const User const &user) override;
  void set_user_flag(const User const &user, UserFlag flag) override;

private:
};