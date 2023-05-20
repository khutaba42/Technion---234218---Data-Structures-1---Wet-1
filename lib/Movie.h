#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "includes.h"

class Movie
{
public:
    Movie(int id, Genre genre, int views, bool vipOnly) : __id(id), __rating(0), __numOfRaters(0),
                                                      __genre(genre), __numOfViews(views), __vipOnly(vipOnly) {}
    
    Movie(int id) : __id(id) {}

    ~Movie() = default;
    Movie(const Movie &) = delete;
    Movie &operator=(const Movie &) = delete;
    void rate(int rating)
    {
        __rating = (__rating * __numOfRaters + rating) / (++__numOfRaters);
    }

    int getRating() const
    {
        return __rating;
    }

    int getViews() const
    {
        return __numOfViews;
    }

    Genre getGenre() const{
        return __genre;
    }

    int getID() const{
        return __id;
    }

    bool isVIPOnly() const {
        return __vipOnly;
    }

    void user_watch()
    {
        __numOfViews++;
    }

    void group_watch(const GroupWatch &group)
    {
        __numOfViews += group.getNumOfUsers();
    }

    bool operator==(const Movie &other) const
    {
        return __id == other.__id;
    }

    bool operator>(const Movie &other) const
    {
        return __id > other.__id;
    }

    bool operator<(const Movie &other) const
    {
        return __id < other.__id;
    }

private:
    int __id;
    int __rating;
    int __numOfRaters;
    Genre __genre;
    int __numOfViews;
    bool __vipOnly;
};

#endif // _MOVIE_H_