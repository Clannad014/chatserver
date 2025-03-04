#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

class GroupUser : public User
{
public:
    void setGroupRole(string grouprole){this->grourole = grouprole;}
    string getGroupRole(){return this->grourole;}
private:
    string grourole;
};



#endif