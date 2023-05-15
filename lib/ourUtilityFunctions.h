#ifndef _OUR_UTILITY_FUNCTIONS_H_
#define _OUR_UTILITY_FUNCTIONS_H_

#include "wet1util.h"
// our implementation of some simple functions
namespace our
{

    template <typename TYPE>
    void swap(TYPE &first, TYPE &second)
    {
        TYPE temp = first;
        first = second;
        second = temp;
    }

    // assumes TYPE has operator <
    template <typename TYPE>
    inline TYPE max(const TYPE &first, const TYPE &second)
    {
        return (first < second) ? (second) : (first);
    }

    // assumes TYPE has operator <
    template <typename TYPE>
    inline TYPE min(const TYPE &first, const TYPE &second)
    {
        return (second < first) ? (second) : (first);
    }

    // assumes TYPE has operator <
    // assumes TYPE allows conversion from 0
    // assumes TYPE has a unary operator - (i.e. -var)
    template <typename TYPE>
    inline TYPE abs(const TYPE &var)
    {
        return (var < 0) ? (-var) : (var);
    }

    // grouping of the genres, assumes theres at least 1 in the enums class
    template <typename T>
    struct GenreGrouping
    {
        T genre[(unsigned long)Genre::NONE + 1];
        GenreGrouping(const T& data = 0) : genre{0}
        {
            for (short i = 0; i < (unsigned long)Genre::NONE; i++)
            {
                genre[i] = data;
                genre[(unsigned long)Genre::NONE] += genre[i];
            }

        }
        GenreGrouping(const GenreGrouping& src) {
            for (short i = 0; i < (unsigned long)Genre::NONE; i++)
            {
                genre[i] = src.genre[i];
            }
        }
        T& operator=(const GenreGrouping& src) {
            for (short i = 0; i < (unsigned long)Genre::NONE; i++)
            {
                genre[i] = src.genre[i];
            }
        }
        const T &operator[](Genre genre)
        { /* (genre[Genre::ACTION] + genre[Genre::COMEDY] + genre[Genre::DRAMA] + genre[Genre::FANTASY];) */
            if (genre != Genre::NONE) 
                return this->genre[(unsigned long)genre];
            T sum = this->genre[0];
            for (short i = 1; i < (unsigned long)Genre::NONE; i++)
            {
                sum += this->genre[i];
            }
            return sum;
        }
        // since the c'tor is NOT explicit then theres auto conversion
        friend T operator+(const T &left, const T &right) {
            T sum = left.genre[0] + right.genre[0];
            for (short i = 1; i < (unsigned long)Genre::NONE; i++)
            {
                sum += left.genre[i] + right.genre[i];
            }
            return sum;
        }

    };
};



#endif //_OUR_UTILITY_FUNCTIONS_H_
