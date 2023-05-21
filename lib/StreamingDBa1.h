//
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
//
// Recommended TAB size to view this file: 8.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "StreamingDBa1_meta.h"

class streaming_database
{
private:
    /**
     * @brief
     * these trees holds in a shared pointer to all the movies and they are ordered by ID
     * we have 5 trees the first 4 are genre specific and the last holds in ALL the movies 
     * @warning
     * this can only change when we add or remove a movie
    */
    AVLTree<std::shared_ptr<Movie>,
            Compare_shared_ptr_to_movies_by_ID>
        __movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE) + 1];
    /** 
     * @brief
     * these trees holds in a shared pointer to each Genre (without NONE) of all the movies and they are ordered by:
     *      Rating 	- from low  to high
     *      Views 	- from low  to high
     *      ID 		- from high to low
     *      the trick here is that the Max element in this tree is the lowest ID between the HIGHEST Views and the HIGHEST Rating
     *  we have 5 trees the first 4 are genre specific and the last holds in ALL the movies 
     * 
     * @warning
     * these trees should change (reorder their elements) under any circumstance that the rating or views of a movies changes
    */
    AVLTree<std::shared_ptr<Movie>,
            Compare_shared_ptr_to_movies_by_rating_views_reversedID>
        __movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(Genre::NONE) + 1];
    /**
     * @brief
    */
    AVLTree<std::shared_ptr<User>,
            Compare_shared_ptr_to_users_by_ID>
        __users_ordered_by_ID;
    /**
     * @brief
    */
    AVLTree<std::shared_ptr<GroupWatch>,
            Compare_shared_ptr_to_groups_by_ID>
        __groups_ordered_by_ID;

public:
    // <DO-NOT-MODIFY> {

    streaming_database();

    virtual ~streaming_database();

    StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);

    StatusType remove_movie(int movieId);

    StatusType add_user(int userId, bool isVip);

    StatusType remove_user(int userId);

    StatusType add_group(int groupId);

    StatusType remove_group(int groupId);

    StatusType add_user_to_group(int userId, int groupId);

    StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId, int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);

    output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);

    output_t<int> get_group_recommendation(int groupId);

    // } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
