#ifndef _OUR_UTILITY_FUNCTIONS_H_
#define _OUR_UTILITY_FUNCTIONS_H_

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
};

#endif //_OUR_UTILITY_FUNCTIONS_H_
