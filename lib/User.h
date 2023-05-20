#ifndef _USER_H_
#define _USER_H_

#include "includes.h"
#include "User&GroupWatchInclude.h"

class User
{
public:
    // error classes

    class UserAlreadyInGroupException : public std::exception
    {
        const char *what() const noexcept override { return "The user is already in a group, cant add it to more than 1."; }
    };

    User(int id, bool vip = false) : __id(id),
                                     __vip(vip),
                                     __soloViews{0},
                                     __groupViewsWhenJoined{0},
                                     __group(nullptr)
    {
    }
    ~User() = default;
    User(const User &) = delete;
    User &operator=(const User &) = delete;

    bool isInGroup() const
    {
        return (__group != nullptr);
    }

    bool isVIP() const
    {
        return __vip;
    }

    void getInGroup(GroupWatch *group)
    {
        if (isInGroup())
        {
            throw UserAlreadyInGroupException();
        }
        __group = group;

        for (short i = 0; i <= (unsigned long)Genre::NONE; i++)
        {
            __groupViewsWhenJoined[i] = __group->getNumOfViews((Genre)i);
        }
    }

    void removeUserFromGroup()
    {
        if (!isInGroup())
            return;
        for (short i = 0; i < (unsigned long)Genre::NONE; i++)
        {
            __group->addViews((Genre)i, -__soloViews[i]);
        }
        __group->removeUser(std::shared_ptr<User>(this));
        __group = nullptr;
    }

    // possible chang
    int getNumOfViews(Genre genre)
    {
        int viewsWithGroup = 0;
        if (isInGroup())
        {
            viewsWithGroup = __group->getNumOfViews(genre);
            viewsWithGroup -= __groupViewsWhenJoined[(unsigned long)genre];
        }
        return viewsWithGroup + __soloViews[(unsigned long)genre];
    }

    void watch(Genre genre)
    {
        if (genre == Genre::NONE)
            return;
        __soloViews[(unsigned long)genre]++;
        __soloViews[(unsigned long)Genre::NONE]++;
        __group->addViews(genre, 1);
    }

    bool operator==(const User &other) const
    {
        return __id == other.__id;
    }

    bool operator>(const User &other) const
    {
        return __id > other.__id;
    }

    bool operator<(const User &other) const
    {
        return __id < other.__id;
    }

private:
    int __id;
    bool __vip;
    int __soloViews[(unsigned long)Genre::NONE + 1];
    int __groupViewsWhenJoined[(unsigned long)Genre::NONE + 1];
    GroupWatch *__group;
};

#endif // _USER_H_