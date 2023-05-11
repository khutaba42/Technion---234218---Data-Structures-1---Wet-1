#ifndef _MOVIE_H_
#define _MOVIE_H_

class Movie
{
public:
    Movie(int id) : __id(id),__rating(0), __numOfRaters(0) {}
    ~Movie() = default;
    Movie(const Movie&) = delete;
    Movie& operator=(const Movie&) = delete;
    void rate(int rating)
    {
        __rating = (__rating * __numOfRaters + rating)/ (++__numOfRaters);
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
};

#endif // _MOVIE_H_