#ifndef _GROUP_WATCH_H_
#define _GROUP_WATCH_H_

#include "includes.h"
#include "User&GroupWatchInclude.h"
class GroupWatch
{
public:
    GroupWatch(int id) : __id(id),
                         __groupViews{0},
                         __allViews{0},
                         __numOfUsers(0)
    {
    }
    ~GroupWatch() = default;
    GroupWatch(const GroupWatch&) = delete;
    GroupWatch& operator=(const GroupWatch&) = delete;

    /**
     * 
     * @throw User::UserAlreadyInGroupException
    */
    void addUser(std::shared_ptr<User> user)
    {
        user->getInGroup(this);
        for (short i = 0; i < (unsigned long)Genre::NONE; i++)
        {
            addViews((Genre)i, user->getNumOfViews((Genre)i));
        }
        __users.insert(user);
    }

    void removeUser(std::shared_ptr<User> user)
    {
        __users.remove(user);
    }

    int getNumOfUsers() const
    {
        return __numOfUsers;
    }

    void addViews(Genre genre, int amount)
    {
        __allViews[(unsigned long)genre] += amount;
    }

    void watch(Genre genre)
    {
        if(genre == Genre::NONE)
            return;
        __groupViews[(unsigned long)genre]++;
        __allViews[(unsigned long)genre]++;
        __groupViews[(unsigned long)Genre::NONE]++;
    }

    int getNumOfViews(Genre genre)
    {
        return __groupViews[(unsigned long)genre];
    }

    bool operator==(const GroupWatch &other) const
    {
        return __id == other.__id;
    }

    bool operator>(const GroupWatch &other) const
    {
        return __id > other.__id;
    }

    bool operator<(const GroupWatch &other) const
    {
        return __id < other.__id;
    }

private:
    int __id;
    int __groupViews[(unsigned long)Genre::NONE + 1];
    int __allViews[(unsigned long)Genre::NONE];
    int __numOfUsers;
    AVLTree<std::shared_ptr<User>, CompareUsers_By_ID> __users;//define a new class called our shared_ptr
};

#endif // _GROUP_WATCH_H_