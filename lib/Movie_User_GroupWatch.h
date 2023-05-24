#ifndef _MOVIE___USER___GROUP_WATCH_INCLUDE_TOGETHER_H_
#define _MOVIE___USER___GROUP_WATCH_INCLUDE_TOGETHER_H_

#include "wet1util.h"
#include "AVLTree.h"
#include <exception>
#include <memory>

class Movie
{
public:
    Movie(int id, Genre genre, int views, bool vipOnly);

    Movie(int id);

    ~Movie() = default;
    Movie(const Movie &) = default;
    Movie &operator=(const Movie &) = default;

    void rate(double rating);
    int getRating() const;
    int getViews() const;
    Genre getGenre() const;
    int getID() const;
    bool isVIPOnly() const;
    void add_views(int amountOfViews);
    bool operator==(const Movie &other) const;
    bool operator>(const Movie &other) const;
    bool operator<(const Movie &other) const;

private:
    int __id;
    double __rating;
    int __numOfRaters;
    Genre __genre;
    int __numOfViews;
    bool __vipOnly;
};

class GroupWatch;
class User;

class User
{
public:
    // Error classes
    class UserAlreadyInGroupException : public std::exception
    {
        const char *what() const noexcept override
        {
            return "The user is already in a group, can't add it to more than 1.";
        }
    };

    User(int id, bool vip = false);
    ~User() = default;
    User(const User &) = default;
    User &operator=(const User &) = default;

    bool isVIP() const;
    void getInGroup(GroupWatch *group);
    int getNumOfViewsBeforeJoining(Genre genre) const;
    void removeUserFromGroup();
    int getNumOfViews(Genre genre);
    void prepareUsersForEntireGroupDeletion();
    void watch(Genre genre);
    bool operator==(const User &other) const;
    bool operator>(const User &other) const;
    bool operator<(const User &other) const;

private:
    int __id;
    bool __vip;
    bool __isInGroup;
    int __soloViews[static_cast<unsigned long>(Genre::NONE)];
    int __groupViewsWhenJoined[static_cast<unsigned long>(Genre::NONE)];
    GroupWatch *__group;
};

class GroupWatch
{
public:
    GroupWatch(int id);
    ~GroupWatch() = default;
    GroupWatch(const GroupWatch &) = default;
    GroupWatch &operator=(const GroupWatch &) = default;
    void clearUsers();
    void addUser(std::shared_ptr<User> user);
    bool isVIP() const;
    bool isEmpty();
    void removeUser(std::shared_ptr<User> user);
    int getNumOfUsers() const;
    Genre getFavGenre() const;
    void addViews(Genre genre, int amount);
    void watch(Genre genre);

    void addNumOfCurrUsersViews(int amount, Genre genre);

    int getNumOfGroupMovies(Genre genre);
    bool operator==(const GroupWatch &other) const;
    bool operator>(const GroupWatch &other) const;
    bool operator<(const GroupWatch &other) const;

private:
    int __id;
    int __numOfVipUsers;
    int __numOfUsers;
    // to determine in O(1) whats the most loved Genre, beware that this should change everytime that a group user waches something in or out of the group
    int __usersSoloViews[static_cast<unsigned long>(Genre::NONE)];
    int __groupMoviesWatched[static_cast<unsigned long>(Genre::NONE)];
    int __allViews[static_cast<unsigned long>(Genre::NONE)];

    
    static Comparison compare_shared_ptr_to_users_by_ID_for_Group_Watch(const std::shared_ptr<User> &left, const std::shared_ptr<User> &right)
    {
        if (*left < *right)
            return Comparison::less;
        else if (*left > *right)
            return Comparison::greater;
        else
            return Comparison::equal;
    }

    AVLTree<std::shared_ptr<User>, 
            compare_shared_ptr_to_users_by_ID_for_Group_Watch> 
        __users;

};

#endif // _MOVIE___USER___GROUP_WATCH_INCLUDE_TOGETHER_H_