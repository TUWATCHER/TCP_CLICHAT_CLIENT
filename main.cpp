#include <iostream>
#include "Menu.h"


int main()
{
    bool HasLoggedIn = false;    
    vector<User> UserDataBase;
    vector<Chat> ChatDataBase;
    User* currentUser = nullptr;
    
    while (!Terminator)
    {
        if (HasLoggedIn)
        {
            UserMenu(ChatDataBase, UserDataBase, currentUser, HasLoggedIn);
        }
        else
        {
            LoginMenu(&currentUser, UserDataBase, HasLoggedIn);
        }
      
    }
    return 0;
}
