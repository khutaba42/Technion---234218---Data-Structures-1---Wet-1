#ifndef _INCLUDES_H_
#define _INCLUDES_H_


#include "wet1util.h"
#include <memory>
#include "AVLTree.h"
#include "Movie_User_GroupWatch.h"

/**
 * Compare Functions for the Trees
*/

inline Comparison Compare_shared_ptr_to_movies_by_ID(const std::shared_ptr<Movie> &left, const std::shared_ptr<Movie> &right)
{
    if (*left < *right)
        return Comparison::less;
    else if (*left > *right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

inline Comparison Compare_shared_ptr_to_movies_by_rating_views_reversedID(const std::shared_ptr<Movie> &left, const std::shared_ptr<Movie> &right)
{
    if (left->getRating() > right->getRating())
        return Comparison::greater;
    else if (left->getRating() < right->getRating())
        return Comparison::less;
    else // (left->getRating() == right->getRating())
    {
        if (left->getViews() > right->getViews())
            return Comparison::greater;
        else if (left->getViews() < right->getViews())
            return Comparison::less;
        else //(left->getViews() == right->getViews())
        {
            if (*left > *right)
                return Comparison::less;
            else if (*left < *right)
                return Comparison::greater;
            else
                return Comparison::equal;
        }
    }
}

inline Comparison Compare_shared_ptr_to_groups_by_ID(const std::shared_ptr<GroupWatch> &left, const std::shared_ptr<GroupWatch> &right)
{
    if (*left < *right)
        return Comparison::less;
    else if (*left > *right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

inline Comparison Compare_shared_ptr_to_users_by_ID(const std::shared_ptr<User> &left, const std::shared_ptr<User> &right)
{
    if (*left < *right)
        return Comparison::less;
    else if (*left > *right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

#endif // _INCLUDES_H_