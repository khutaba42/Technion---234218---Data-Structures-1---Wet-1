#include "../lib/Movie_User_GroupWatch.h"

GroupWatch::GroupWatch(int id)
    : __id(id), __numOfVipUsers(0), __numOfUsers(0)
{
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __usersSoloViews[i] = 0;
        __groupMoviesWatched[i] = 0;
        __allViews[i] = 0;
    }
}
struct FunctionOpj
{
    void operator()(std::shared_ptr<User> &user)
    {
        user->prepareUsersForEntireGroupDeletion();
    }
};

void GroupWatch::clearUsers()
{
    struct FunctionOpj remove;
    __users.in_order_traversal(remove);
    __numOfUsers = 0;
}

/**
 *
 * @brief Compares ID's between Users
 * Time_Complexity: O(n_groupUsers) <= O(Users)
 * Space_Complexity: O(1)
 */
void GroupWatch::addUser(std::shared_ptr<User> user)
{
    user->getInGroup(this);
    __users.insert(user);
    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        __usersSoloViews[i] += user->getNumOfViews(static_cast<Genre>(i));
    }
    if (user->isVIP())
    {
        __numOfVipUsers++;
    }
    __numOfUsers++;
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

    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        //__usersSoloViews[i] -= user->getNumOfViews(static_cast<Genre>(i));
        __allViews[i] -= (__groupMoviesWatched[i] - user->getNumOfViewsBeforeJoining(static_cast<Genre>(i)));
    }
    __numOfUsers--;
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

    for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
    {
        if (__usersSoloViews[i] + __allViews[i] > maxGenreViews)
        {
            maxGenreViews = __usersSoloViews[i] + __allViews[i];
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
void GroupWatch::watch(Genre genre)
{
    if (genre == Genre::NONE)
    {
        return;
    }
    __allViews[static_cast<unsigned long>(genre)] += getNumOfUsers();
    __groupMoviesWatched[static_cast<unsigned long>(genre)]++;
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
    __usersSoloViews[static_cast<unsigned long>(genre)] += amount;
}

/**
 *
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int GroupWatch::getNumOfGroupMovies(Genre genre)
{
    int res = 0;
    if(genre == Genre::NONE)
    {
        for (unsigned short i = 0; i < static_cast<unsigned long>(Genre::NONE); i++)
        {
            res += __groupMoviesWatched[i];
        }
    }
    else {
        return __groupMoviesWatched[static_cast<unsigned long>(genre)];
    }
    return res;
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