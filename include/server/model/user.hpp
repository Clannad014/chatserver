#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

//匹配User表的ORM类
class User
{
public:
    User(int id = -1, string name = "", string password = "", string state = "offline",int avatarIndex = 0)
    {
        this->id = id;
        this->name = name;
        this->password = password;
        this->state = state;
        this->avatarIndex = avatarIndex;
    }
    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPassword(string password) { this->password = password; }
    void setState(string state) { this->state = state; }
    void setAvatar(int avatarIndex) {this->avatarIndex = avatarIndex;}

    int getId() { return this->id; }
    string getName() { return this->name; }
    string getPassword() { return this->password; }
    string getState() { return this->state; }
    int getAvatar() {return this->avatarIndex;}

protected:
    int id;
    string name;
    string password;
    string state;
    int avatarIndex;
};

#endif
