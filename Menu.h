#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include "Chat.h"

using std::vector;
using std::string;

static bool Terminator = false;

void CreateUser(const string& _username, const string& _password, vector<User>& _UserDB);
const bool FindUser(const string& _username, const vector<User>& _UserDB);
User* GetUser(const string& _username, vector<User>& _UserDB);
void DeleteUser(User* _currentUser, vector<User>& _UserDB, bool& status);
User* Login(const string& _username, const string& _password, vector<User>& _UserDB, bool& status);
void ShowUserDataBase(const vector<User>& _UserDB);
void LoginMenu(User** _currentUser, vector<User>& _UserDB, bool& status);
void UserMenu(vector<Chat>& _ChatDB, vector<User>& _UserDB, User* _currentUser, bool& status);
void CreateMessage(vector<User>& _UserDB, vector<Chat>& _ChatDB, User* _currentUser);
void ShowMessage(const vector<Chat>& _ChatDB, User* _currentUser);

/*----------------------------------------------------------------------*/

void CreateUser(const string& _username, const string& _password, vector<User>& _UserDB)
{
    if (FindUser(_username, _UserDB))
    {
        std::cout << "This user already exists!\n";
    }
    else
    {
        std::cout << "Please enter your name: ";
        string name;
        std::cin >> name;
        std::cout << "User has been created!\n";
        _UserDB.push_back(User(_username, _password, name));
    }
}

void DeleteUser(User* _currentUser, vector<User>& _UserDB, bool& status)
{
    _UserDB.erase(std::find(_UserDB.begin(), _UserDB.end(), User(_currentUser->GetUserName())));
    _currentUser = nullptr;
    status = false;
}

const bool FindUser(const string& _username, const vector<User>& _UserDB)
{
    if (std::find(_UserDB.begin(), _UserDB.end(), User(_username)) != _UserDB.end())
    {
        return true;
    }
    else
    {
        return false;
    }

}

User* GetUser(const string& _username, vector<User>& _UserDB)
{
    if (FindUser(_username, _UserDB))
    {
        auto search_index = std::find(_UserDB.begin(), _UserDB.end(), User(_username));
        return &(_UserDB[search_index - _UserDB.begin()]);
    }
    {
        std::cout << "User not exist!\n";
        return nullptr;
    }

}
void Logout(User* _currentUser, bool& status)
{
    _currentUser = nullptr;
    status = false;
}

User* Login(const string& _username, const string& _password, vector<User>& _UserDB, bool& status)
{
    if (FindUser(_username, _UserDB))
    {
        auto search_index = std::find(_UserDB.begin(), _UserDB.end(), User(_username));
        if (search_index->CheckUser(_username, _password))
        {
            std::cout << "Welcome to CLI Chat!\n";
            status = true;
            return &(_UserDB[search_index - _UserDB.begin()]);
        }
        else
        {
            std::cout << "Wrong Password!\n";
            return nullptr;
        }

    }
    else
    {
        std::cout << "Such user does not exist! Please register or enter correct username!\n";
        return nullptr;
    }
}
void ShowUserDataBase(const vector<User>& _UserDB)
{
    if (_UserDB.empty())
    {
        std::cout << "User data base is empty!\n";
    }
    else
    {
        for (auto user : _UserDB)
        {
            std::cout << user << '\n';
        }
    }    
}

void CreateMessage(vector<User>& _UserDB, vector<Chat>& _ChatDB, User* _currentUser)
{
    string username;
    string message;
    vector<User> recipients;
    char choise = 'Y';
    std::cout << "Do you want to send this message to all users?(Y/N): ";
    std::cin >> choise;
    if (toupper(choise) == 'Y')
    {
        std::cout << "Enter message: \n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, message);
        for (auto reciever : _UserDB)
        {
            if (reciever == *_currentUser)
            {
                continue;
            }
            else
            {
                recipients.push_back(reciever);
            }
        }
        _ChatDB.push_back(Chat(_currentUser, recipients, message));
        
    }
    else if (toupper(choise) == 'N')
    {
        std::cout << "Enter user name: ";
        std::cin >> username;
        if (FindUser(username, _UserDB))
        {
                if (_ChatDB.empty())
            {
                std::cout << "Enter message:\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, message);
                _ChatDB.push_back(Chat(_currentUser, GetUser(username, _UserDB), message));
            }
            else
            {
                for (int i = 0; i < _ChatDB.size(); i++)
                {
                    if ((*_currentUser == _ChatDB[i].GetFromUser() || *_currentUser == _ChatDB[i].GetToUser())
                        && (*GetUser(username, _UserDB) == _ChatDB[i].GetFromUser() || *GetUser(username, _UserDB) == _ChatDB[i].GetToUser()))
                    {
                        std::cout << "Enter message: \n";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, message);
                        _ChatDB[i].AddMessage(message);
                        break;
                    }
                    else
                    {
                        std::cout << "Enter message: \n";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, message);
                        _ChatDB.push_back(Chat(_currentUser, GetUser(username, _UserDB), message));
                        break;
                    }
                }
            }      

        }
        else
        {
            std::cout << "User does not exist!\n";
        }

    }
    else
    {
        std::cout << "Incorrect input!\n";
    }

}

void ShowMessage(const vector<Chat>& _ChatDB, User* _currentUser)
{
    for (auto checkMessage : _ChatDB)
    {
        if (*_currentUser == checkMessage.GetFromUser() || *_currentUser == checkMessage.GetToUser())
        {
            checkMessage.ShowMessages();
        }
        else if (checkMessage.HasRecipient(_currentUser->GetUserName()))
        {
            checkMessage.ShowMessages();
        }
    }

}

void LoginMenu(User** _currentUser, vector<User>& _UserDB, bool& status)
{
    string username, password;
    int menuOperator = 0;
    std::cout << "Welcome to CLI Chat, please choose your option:\n";
    std::cout << "\t0 - Exit\n"
        << "\t1 - Login\n"
        << "\t2 - Create User\n"
        << "\t3 - Forgot Password\n"
        << "\t4 - Show User Data Base\n";    
      
    while (!(std::cin >> menuOperator))
    {

        std::cin.clear();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Please try again with valid input: ";
    }

    switch (menuOperator)
    {
    case 0:
        std::cout << "Closing application!\n";
        Terminator = true;
        break;
    case 1:
        std::cout << "Please enter username: ";
        std::cin >> username;
        std::cout << "Please enter password: ";
        std::cin >> password;
        *_currentUser = Login(username, password, _UserDB, status);
        break;
    case 2:
        std::cout << "Please enter username: ";
        std::cin >> username;
        std::cout << "Please enter password: ";
        std::cin >> password;        
        CreateUser(username, password, _UserDB);
        break;
    case 4:
        ShowUserDataBase(_UserDB);
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }
}

void UserMenu(vector<Chat>& _ChatDB, vector<User>& _UserDB, User* _currentUser, bool& status)
{
    string username, password;
    int menuOperator;
    
    std::cout << "Welcome user: " << _currentUser->GetName() << "\nPlease choose your option:\n";
    std::cout << "\t1 - Show Messages\n"
        << "\t2 - Send Message\n"
        << "\t3 - Logout\n"
        << "\t4 - Delete User\n"
        << "\t5 - Change Password\n"
        << "\t0 - Exit\n";
    std::cin >> menuOperator;
    switch (menuOperator)
    {
    case 0:
        Terminator = true;
        break;
    case 1:
        std::cout << "Messages: \n";
        ShowMessage(_ChatDB, _currentUser);
        break;
    case 2:
        std::cout << "Send message\n";
        CreateMessage(_UserDB, _ChatDB, _currentUser);
        break;
    case 3:
        std::cout << "You has logout\n";
        Logout(_currentUser, status);
        break;
    case 4:
    {
        std::cout << "Are you sure you want to delete your user?(Y/N): ";
        char choise = 'Y';        
        std::cin >> choise;
        if (toupper(choise) == 'Y')
        {
            DeleteUser(_currentUser, _UserDB, status);
            std::cout << "User has been successfuly deleted!\n";
            break;
        }        
        break;
    }
    case 5:
        std::cout << "Please enter old password: ";
        std::cin >> password;
        if (password == _currentUser->GetPassword())
        {
            std::cout << "Please enter new password: ";
            std::cin >> password;
            _currentUser->SetPassword(password);
            std::cout << "Your password has been changed to: " << _currentUser->GetPassword() << std::endl;
        }
        else
        {
            std::cout << "Wrong old password!\n";
        }
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }

}
