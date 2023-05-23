#include "../lib/Movie_User_GroupWatch.h"


User::User(int id, bool vip)
    : __id(id), __vip(vip), __isInGroup(false), __group(nullptr)
{
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __soloViews[i] = 0;
        __groupViewsWhenJoined[i] = 0;
    }
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
    if (__isInGroup)
    {
        throw UserAlreadyInGroupException();
    }
    __group = group;
    __isInGroup = true;
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __groupViewsWhenJoined[i] = __group->getNumOfGroupMovies(static_cast<Genre>(i));
    }
}

int User::getNumOfViewsBeforeJoining(Genre genre) const
{
    return __groupViewsWhenJoined[static_cast<unsigned long>(genre)];
}

/**
 *
 * @brief Compares ID's between Users.
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void User::removeUserFromGroup()
{
    if (!__isInGroup)
    {
        return;
    }
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __group->addNumOfCurrUsersViews(-__soloViews[i], static_cast<Genre>(i));
        __soloViews[i] += (__group->getNumOfGroupMovies(static_cast<Genre>(i)) - getNumOfViewsBeforeJoining(static_cast<Genre>(i)));
    }
    __group->removeUser(std::shared_ptr<User>(new User(*this)));
    __group = nullptr;
    __isInGroup = false;
}

/**
 *
 * @brief Compares ID's between Users.
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void User::prepareUsersForEntireGroupDeletion()
{
    if (!__isInGroup)
    {
        return;
    }
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __groupViewsWhenJoined[i] = 0;
        __soloViews[i] += (__group->getNumOfGroupMovies(static_cast<Genre>(i)) - getNumOfViewsBeforeJoining(static_cast<Genre>(i)));
    }
    __group = nullptr;
    __isInGroup = false;
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
    if (__isInGroup)
    {
        viewsWithGroup = __group->getNumOfGroupMovies(genre);
        viewsWithGroup -= __groupViewsWhenJoined[static_cast<unsigned long>(genre)];
    }
    int res = 0;
    if(genre == Genre::NONE)
    {
        for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
        {
            res+=__soloViews[i];
        }
    }
    else {
        res += __soloViews[static_cast<unsigned long>(genre)];
    }
    return viewsWithGroup + res;
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
    if (__isInGroup)
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