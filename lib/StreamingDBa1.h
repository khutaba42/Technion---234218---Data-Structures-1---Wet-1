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

#include "wet1util.h"
#include <memory>
#include "includes.h"
class streaming_database {
private:
	/* 	these trees holds in a shared pointer to all the movies and they are ordered by ID
		we have 5 trees the first 4 are genre specific and the last holds in ALL the movies */
	AVLTree<std::shared_ptr<Movie>, CompareMovies_By_ID> __movies_By_ID[(unsigned long) Genre::NONE + 1];
	/* 	these trees holds in a shared pointer to all the movies and they are ordered by:
			Rating 	- from low  to high
			Views 	- from low  to high
			ID 		- from high to low
		we have 5 trees the first 4 are genre specific and the last holds in ALL the movies */
	AVLTree<std::shared_ptr<Movie>, compareMovies_By_Rating_Views_reversedID> __movies_By_Rating_Views_reversedID[(unsigned long) Genre::NONE + 1];
	AVLTree<std::shared_ptr<User>, CompareUsers_By_ID> __users;
	AVLTree<std::shared_ptr<GroupWatch>, CompareGroups_By_ID> __groups;

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

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
