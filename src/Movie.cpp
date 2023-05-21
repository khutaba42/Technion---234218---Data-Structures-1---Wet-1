#include "../lib/Movie_User_GroupWatch.h"

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
Movie::Movie(int id, Genre genre, int views, bool vipOnly) 
    : __id(id), __rating(0), __numOfRaters(0), __genre(genre), __numOfViews(views), __vipOnly(vipOnly) {
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
Movie::Movie(int id) 
    : __id(id) {
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void Movie::rate(int rating) {
    __rating = (__rating * __numOfRaters + rating);
    __rating /= ++__numOfRaters;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int Movie::getRating() const {
    return __rating;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int Movie::getViews() const {
    return __numOfViews;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
Genre Movie::getGenre() const {
    return __genre;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
int Movie::getID() const {
    return __id;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool Movie::isVIPOnly() const {
    return __vipOnly;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
void Movie::add_views(int amountOfViews) {
    __numOfViews += amountOfViews;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool Movie::operator==(const Movie& other) const {
    return __id == other.__id;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool Movie::operator>(const Movie& other) const {
    return __id > other.__id;
}

/**
 * @brief
 * Time_Complexity: O(1)
 * Space_Complexity: O(1)
 */
bool Movie::operator<(const Movie& other) const {
    return __id < other.__id;
}
