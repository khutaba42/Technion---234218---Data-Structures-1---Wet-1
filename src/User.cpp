#include "../lib/Movie_User_GroupWatch.h"


User::User(int id, bool vip)
    : __id(id), __vip(vip), __group(nullptr)
{
    for (short i = 0; i <= static_cast<unsigned long>(Genre::NONE); i++)
    {
        __soloViews[i] = 0;
        __groupViewsWhenJoined[i] = 0;
    }
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool User::isInGroup() const
{
    return (__group != nullptr);
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool User::isVIP() const
{
    return __vip;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 * @throw
 * UserAlreadyInGroupException
 */
void User::getInGroup(GroupWatch *group)
{
    if (isInGroup())
    {
        throw UserAlreadyInGroupException();
    }
    __group = group;

    for (short i = 0; i <= static_cast<unsigned long>(Genre::NONE); i++)
    {
        __groupViewsWhenJoined[i] = __group->getNumOfViews(static_cast<Genre>(i));
    }
}

/**
 *
 * @brief Compares ID's between Users.
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void User::removeUserFromGroup()
{
    if (!isInGroup())
    {
        return;
    }
    for (short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __group->addNumOfCurrUsersViews(-__soloViews[i], static_cast<Genre>(i));
    }
    __group->removeUser(std::shared_ptr<User>(this));
    __group = nullptr;
}

/**
 *
 * @brief Compares ID's between Users.
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void User::prepareUsersForEntireGroupDeletion()
{
    if (!isInGroup())
    {
        return;
    }
    for (short i = 0; i <= static_cast<unsigned long>(Genre::NONE); i++)
    {
        __groupViewsWhenJoined[i] = 0;
    }
    //__group->removeUser(std::shared_ptr<User>(this));
    __group = nullptr;
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int User::getNumOfViews(Genre genre)
{
    int viewsWithGroup = 0;
    if (isInGroup())
    {
        viewsWithGroup = __group->getNumOfViews(genre);
        viewsWithGroup -= __groupViewsWhenJoined[static_cast<unsigned long>(genre)];
    }
    return viewsWithGroup + __soloViews[static_cast<unsigned long>(genre)];
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void User::watch(Genre genre)
{
    if (genre == Genre::NONE)
    {
        return;
    }
    __soloViews[static_cast<unsigned long>(genre)]++;
    __soloViews[static_cast<unsigned long>(Genre::NONE)]++;
    if (isInGroup())
    {
        __group->addNumOfCurrUsersViews(1, genre);
    }
    
    // why do we need to update the group? he watched it alone ? __group->addViews(genre, 1);
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool User::operator==(const User &other) const
{
    return __id == other.__id;
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool User::operator>(const User &other) const
{
    return __id > other.__id;
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool User::operator<(const User &other) const
{
    return __id < other.__id;
}