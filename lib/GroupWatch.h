#ifndef _GROUP_WATCH_H_
#define _GROUP_WATCH_H_

#include "User.h"
#include "AVLTree.h"

class GroupWatch
{
public:
    GroupWatch(int id) : __id(id),
                         __numOfMoviesWatched{0}
    {
    }
    ~GroupWatch() = default;
    GroupWatch(const GroupWatch&) = delete;
    GroupWatch& operator=(const GroupWatch&) = delete;

    /**
     * 
     * @throw User::UserAlreadyInGroupException
    */
    void addUser(User& user)
    {
        user.getInGroup(this);
        __users.insert(&user);
    }

    void watch(Genre genre)
    {
        __numOfMoviesWatched[(unsigned long)genre]++;
    }

    int getNumOfViews(Genre genre)
    {
        if (genre == Genre::NONE)
        {
            int sum = 0;
            for (int i = 0; i < (unsigned long)Genre::NONE; i++)
            {
                sum += __numOfMoviesWatched[i];
            }
            return sum;
        }
        return __numOfMoviesWatched[(unsigned long)genre];
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
    int __numOfMoviesWatched[(unsigned long)Genre::NONE];
    AVLTree<User *> __users;
};

#endif // _GROUP_WATCH_H_