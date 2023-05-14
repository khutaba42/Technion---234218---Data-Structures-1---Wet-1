#ifndef _USER_H_
#define _USER_H_

#include "wet1util.h"
#include "GroupWatch.h"
#include <exception>

class User
{
public:
    // error classes 

    class UserAlreadyInGroupException : public std::exception
    {
        const char *what() const noexcept override { return "The user is already in a group, cant add it to more than 1."; }
    };

    User(int id) : __id(id),
                   __numOfSoloMoviesWatched{0},
                   __numOfGroupMoviesWatchedWhenJoined{0},
                   __group(nullptr)
    {
    }
    ~User() = default;
    User(const User &) = delete;
    User &operator=(const User &) = delete;

    bool isInGroup() const { return (__group != nullptr); }

    void getInGroup(GroupWatch* group)
    {
        if(isInGroup())
        {
            throw UserAlreadyInGroupException();
        }
        __group = group;
        
        __numOfGroupMoviesWatchedWhenJoined[(unsigned long)Genre::ACTION] = __group->getNumOfViews(Genre::ACTION);
        __numOfGroupMoviesWatchedWhenJoined[(unsigned long)Genre::COMEDY] = __group->getNumOfViews(Genre::COMEDY);
        __numOfGroupMoviesWatchedWhenJoined[(unsigned long)Genre::DRAMA] = __group->getNumOfViews(Genre::DRAMA);
        __numOfGroupMoviesWatchedWhenJoined[(unsigned long)Genre::FANTASY] = __group->getNumOfViews(Genre::FANTASY);
    }

    // possible chang
    int getNumOfViews(Genre genre)
    {
        int viewsWithGroup = 0;
        if (isInGroup())
        {
            viewsWithGroup = __group->getNumOfViews(genre);
            if (genre == Genre::NONE)
            {
                for (int i = 0; i < (unsigned long)Genre::NONE; i++)
                {
                    viewsWithGroup -= __numOfGroupMoviesWatchedWhenJoined[i];
                }
            }
            else
            {
                viewsWithGroup -= __numOfGroupMoviesWatchedWhenJoined[(unsigned long)genre];
            }
        }

        if (genre == Genre::NONE)
        {
            int sum = 0;
            for (int i = 0; i < (unsigned long)Genre::NONE; i++)
            {
                sum += __numOfSoloMoviesWatched[i];
            }
            return viewsWithGroup + sum;
        }
        return viewsWithGroup + __numOfSoloMoviesWatched[(unsigned long)genre];
    }

    void watch(Genre genre)
    {
        __numOfSoloMoviesWatched[(unsigned long)genre]++;
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
    int __numOfSoloMoviesWatched[(unsigned long)Genre::NONE];
    int __numOfGroupMoviesWatchedWhenJoined[(unsigned long)Genre::NONE];
    GroupWatch *__group;
};

#endif // _USER_H_