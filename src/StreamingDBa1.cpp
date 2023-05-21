#include "StreamingDBa1.h"

/**
 * @brief
 * @tparam
 * Time_Complexity: O(1)
 * @tparam
 * Space_Complexity: O(1)
 */
streaming_database::streaming_database()
{
	// default constructor
}

/**
 * @brief
 * @tparam
 * Time_Complexity: O(max{Users, Movies, Groups}) <= O(Users + Movies + Groups)
 * @tparam
 * Space_Complexity: O(log(max{Users, Movies, Groups})) <= O(log(Users + Movies + Groups))
 */
streaming_database::~streaming_database()
{
	// default destructor
}

/**
 * @brief
 * when adding a movie we add it to 3 trees:
 * 	-	__movies_ordered_by_ID[Genre::NONE]
 * 	-	__movies_ordered_by_ID[Genre::genre]
 * 	-	__movies_ordered_by_rating_views_reversedID[Genre::genre] (no NONE)
 * @tparam
 * Time_Complexity: O(log(Movies))
 * @tparam
 * Space_Complexity: O(log(Movies))
 */
StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	if (movieId <= 0 || genre == Genre::NONE || views < 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<Movie> movie(new Movie(movieId, genre, views, vipOnly));
		__movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].insert(movie);
		__movies_ordered_by_ID[static_cast<unsigned long>(genre)].insert(movie);
		/** @warning dont think we need it __movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(Genre::NONE)].insert(movie);*/
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(genre)].insert(movie);
	}
	catch (std::bad_alloc &)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::ElementAlreadyExistsException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity: O(log(Movies))
 * @tparam
 * Space_Complexity: O(log(Movies))
 */
StatusType streaming_database::remove_movie(int movieId)
{
	if (movieId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<Movie> temp(new Movie(movieId));
		std::shared_ptr<Movie> toDelete = __movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].find(temp);
		__movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].remove(toDelete);
		__movies_ordered_by_ID[static_cast<unsigned long>(toDelete->getGenre())].remove(toDelete);
		/** @warning dont think we need it __movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(Genre::NONE)].remove(toDelete);*/
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(toDelete->getGenre())].remove(toDelete);
	}
	/** @warning no need to as said in piazza
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}*/
	catch (const AVLTree<std::shared_ptr<Movie>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::add_user(int userId, bool isVip)
{
	if (userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> user(new User(userId, isVip));
		__users_ordered_by_ID.insert(user);
	}
	catch (const std::bad_alloc &)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::ElementAlreadyExistsException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::remove_user(int userId)
{
	if (userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		// look for the User object using his name
		std::shared_ptr<User> temp(new User(userId));
		// return the User object
		std::shared_ptr<User> toDelete = __users_ordered_by_ID.find(temp);
		// remove the user from the group he is in
		toDelete->removeUserFromGroup();
		// now we can delete the User object itself
		__users_ordered_by_ID.remove(toDelete);
	}
	catch (const std::bad_alloc &)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity: O(log(Groups))
 * @tparam
 * Space_Complexity: O(log(Groups))
 */
StatusType streaming_database::add_group(int groupId)
{
	if (groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<GroupWatch> group(new GroupWatch(groupId));
		__groups_ordered_by_ID.insert(group);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<GroupWatch>>::ElementAlreadyExistsException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::remove_group(int groupId)
{
	if (groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		// first we need to go through all the groupUsers and remove them form the group
		std::shared_ptr<GroupWatch> toDelete(new GroupWatch(groupId));
		__groups_ordered_by_ID.remove(toDelete);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<GroupWatch>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	if (userId <= 0 || groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user(__users_ordered_by_ID.find(tempUser));

		std::shared_ptr<GroupWatch> tempGroup(new GroupWatch(groupId));
		std::shared_ptr<GroupWatch> group = __groups_ordered_by_ID.find(tempGroup);

		
		group->addUser(user);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &e)
	{
		e.what();
		return StatusType::FAILURE;
	}
	catch (const AVLTree<std::shared_ptr<GroupWatch>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	catch (const User::UserAlreadyInGroupException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::user_watch(int userId, int movieId)
{
	if (userId <= 0 || movieId <= 0)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user = __users_ordered_by_ID.find(tempUser);

		std::shared_ptr<Movie> tempMovie(new Movie(movieId));
		std::shared_ptr<Movie> movie = __movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].find(tempMovie);

		if (movie->isVIPOnly() && !user->isVIP())
			return StatusType::FAILURE;

		user->watch(movie->getGenre());

		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].remove(movie);
		movie->add_views(1);
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].insert(movie);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::group_watch(int groupId, int movieId)
{
	if (groupId <= 0 || movieId <= 0)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<GroupWatch> tempGroup(new GroupWatch(groupId));
		std::shared_ptr<GroupWatch> group = __groups_ordered_by_ID.find(tempGroup);

		if (group->isEmpty())
			return StatusType::FAILURE;

		std::shared_ptr<Movie> tempMovie(new Movie(movieId));
		std::shared_ptr<Movie> movie = __movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].find(tempMovie);

		if (movie->isVIPOnly() && !group->isVIP())
			return StatusType::FAILURE;

		group->watch(movie->getGenre());

		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].remove(movie);
		movie->add_views(group->getNumOfUsers());
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].insert(movie);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<GroupWatch>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
	try
	{
		return __movies_ordered_by_ID[static_cast<unsigned long>(genre)].getSize();
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	if (output == nullptr)
		return StatusType::INVALID_INPUT;

	if (get_all_movies_count(genre).ans() == 0)
		return StatusType::FAILURE;

	try
	{
		// our::storeID storeid(output);
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(genre)].reverse_in_order_traversal(
			[=](const std::shared_ptr<Movie> &movie)
			{
				static int index = 0;
				output[index] = movie->getID();
				index++;
			});
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	output[0] = 4001;
	output[1] = 4002;
	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	if (userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user = __users_ordered_by_ID.find(tempUser);

		return user->getNumOfViews(genre);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}

	return 2008;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
	if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user = __users_ordered_by_ID.find(tempUser);

		std::shared_ptr<Movie> tempMovie(new Movie(movieId));
		std::shared_ptr<Movie> movie = __movies_ordered_by_ID[static_cast<unsigned long>(Genre::NONE)].find(tempMovie);

		if (movie->isVIPOnly() && !user->isVIP())
			return StatusType::FAILURE;

		// remember when using a shared pointer the Movie Object will NOT die here (even if we didn't use a NONE tree)
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].remove(movie);
		movie->rate(rating);
		__movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(movie->getGenre())].insert(movie);
		// now the __movies_ordered_by_rating_views_reversedID is updated as it should
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

/**
 * @brief
 * @tparam
 * Time_Complexity:
 * @tparam
 * Space_Complexity:
 */
output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<GroupWatch> tempGroup(new GroupWatch(groupId));
		std::shared_ptr<GroupWatch> group = __groups_ordered_by_ID.find(tempGroup);

		if (group->isEmpty())
			return StatusType::FAILURE;

		Genre favGenre = group->getFavGenre();
		if (__movies_ordered_by_ID[static_cast<unsigned long>(favGenre)].isEmpty())
			return StatusType::FAILURE;

		return __movies_ordered_by_rating_views_reversedID[static_cast<unsigned long>(favGenre)].getMax()->getID(); // this isn't true,  we need to return (in case of double equality) the movie with the lesser ID
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<GroupWatch>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}
