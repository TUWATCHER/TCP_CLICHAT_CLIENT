#pragma once
#include <string>

using std::string;

class User
{
    public:
        User() = default;
        User(string username);
        User(string user, string pass);
        User(string user, string pass, string name);
        User(const User& user);
        ~User() = default;
        string GetUserName();
        string GetName();
        void SetUserName(const string username);
        void SetName(const string name);
        const string GetPassword() const;
        void SetPassword(const string password);
        bool CheckUser(string username, string password);
        bool operator==(const User& user) const;
        User& operator=(const User& user);
        friend std::ostream& operator<<(std::ostream& os, const User& user);
        
    private:
        string _username;
        string _name;
        string _password;
};