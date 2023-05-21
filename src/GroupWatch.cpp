#include "Movie_User_GroupWatch.h"


GroupWatch::GroupWatch(int id)
    : __id(id), __numOfVipUsers(0), __numOfUsers(0)
{
    for (short i = 0; i <= static_cast<unsigned long>(Genre::NONE); i++)
    {
        __numOfCurrUsersViews[i] = 0;
        __groupViews[i] = 0;
    }
}

GroupWatch::~GroupWatch() {
    __users.in_order_traversal(
        [](std::shared_ptr<User>& user){
            user->prepareUsersForEntireGroupDeletion();
        }
    );
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void GroupWatch::addUser(std::shared_ptr<User> user)
{
    user->getInGroup(this);/*
    for (short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        addViews(static_cast<Genre>(i), user->getNumOfViews(static_cast<Genre>(i)));
    }*/
    __users.insert(user);
    if (user->isVIP())
    {
        __numOfVipUsers++;
    }
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool GroupWatch::isVIP() const
{
    return __numOfVipUsers > 0;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool GroupWatch::isEmpty()
{
    return __users.isEmpty();
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void GroupWatch::removeUser(std::shared_ptr<User> user)
{
    __users.remove(user);
    if (user->isVIP())
    {
        __numOfVipUsers--;
    }
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int GroupWatch::getNumOfUsers() const
{
    return __numOfUsers;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
Genre GroupWatch::getFavGenre() const
{
    int favGenre = 0, maxGenreViews = 0;

    for (int i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        if (__numOfCurrUsersViews[i] > maxGenreViews)
        {
            maxGenreViews = __numOfCurrUsersViews[i];
            favGenre = i;
        }
    }
    return static_cast<Genre>(favGenre);
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void GroupWatch::addViews(Genre genre, int amount)
{
    __allViews[static_cast<unsigned long>(genre)] += amount;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void GroupWatch::watch(Genre genre)
{
    if (genre == Genre::NONE)
    {
        return;
    }
    __numOfCurrUsersViews[static_cast<unsigned long>(genre)] += getNumOfUsers();
    __numOfCurrUsersViews[static_cast<unsigned long>(Genre::NONE)] += getNumOfUsers();





/*

    __groupViews[static_cast<unsigned long>(genre)]++;
    __allViews[static_cast<unsigned long>(genre)]++;
    __groupViews[static_cast<unsigned long>(Genre::NONE)]++;
    */
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void GroupWatch::addNumOfCurrUsersViews(int amount, Genre genre)
{
    if (genre == Genre::NONE)
    {
        return;
    }
    __numOfCurrUsersViews[static_cast<unsigned long>(genre)] += amount;
    __numOfCurrUsersViews[static_cast<unsigned long>(Genre::NONE)] += amount;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int GroupWatch::getNumOfViews(Genre genre)
{
    return __groupViews[static_cast<unsigned long>(genre)];
}

/**
 *
 * @brief Compares ID's between Groups
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool GroupWatch::operator==(const GroupWatch &other) const
{
    return __id == other.__id;
}

/**
 *
 * @brief Compares ID's between Groups
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool GroupWatch::operator>(const GroupWatch &other) const
{
    return __id > other.__id;
}

/**
 *
 * @brief Compares ID's between Groups
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool GroupWatch::operator<(const GroupWatch &other) const
{
    return __id < other.__id;
}