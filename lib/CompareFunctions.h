

#include "includes.h"

Comparison compareMovies_By_Rating_Views_reversedID(const std::shared_ptr<Movie>& left, const std::shared_ptr<Movie>& right) {
    if (left->getRating() > right->getRating())
        return Comparison::greater;
    else if (left->getRating() < right->getRating())
        return Comparison::less;
    else // (left->getRating() == right->getRating())
    {
        if(left->getViews() > right->getViews())
            return Comparison::greater;
        else if(left->getViews() < right->getViews())
            return Comparison::less;
        else //(left->getViews() == right->getViews())
        {
            if(*left > *right)
                return Comparison::less;
            else if(*left < *right)
                return Comparison::greater;
            else 
                return Comparison::equal;
        }
    }
}

Comparison CompareMovies_By_ID(const std::shared_ptr<Movie>& left, const std::shared_ptr<Movie>& right)
{
    if(*left < *right)
        return Comparison::less;
    else if(*left > *right)
        return Comparison::greater;
    else 
        return Comparison::equal;
}

Comparison CompareUsers_By_ID(const std::shared_ptr<User>& left, const std::shared_ptr<User>& right)
{
    if(*left < *right)
        return Comparison::less;
    else if(*left > *right)
        return Comparison::greater;
    else 
        return Comparison::equal;
}

Comparison CompareGroups_By_ID(const std::shared_ptr<GroupWatch>& left, const std::shared_ptr<GroupWatch>& right)
{
    if(*left < *right)
        return Comparison::less;
    else if(*left > *right)
        return Comparison::greater;
    else 
        return Comparison::equal;
}
