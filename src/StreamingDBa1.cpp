#include "../lib/StreamingDBa1.h"

streaming_database::streaming_database()
{
	// default constructor
}

streaming_database::~streaming_database()
{
	// default destructor
}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	if (movieId <= 0 || genre == Genre::NONE || views < 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<Movie> movie(new Movie(movieId, genre, views, vipOnly));
		if (!__movies_By_ID[(unsigned long)Genre::NONE].insert(movie))
			return StatusType::FAILURE;
		__movies_By_ID[(unsigned long)genre].insert(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].insert(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)genre].insert(movie);
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

StatusType streaming_database::remove_movie(int movieId)
{
	if (movieId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<Movie> temp(new Movie(movieId));
		std::shared_ptr<Movie> toDelete = __movies_By_ID[(unsigned long)Genre::NONE].find(temp);
		__movies_By_ID[(unsigned long)Genre::NONE].remove(toDelete);
		__movies_By_ID[(unsigned long)toDelete->getGenre()].remove(toDelete);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].remove(toDelete);
		__movies_By_Rating_Views_reversedID[(unsigned long)toDelete->getGenre()].remove(toDelete);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
	if (userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> user(new User(userId, isVip));
		__users.insert(user);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::ElementAlreadyExistsException &)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
	if (userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> temp(new User(userId));
		std::shared_ptr<User> toDelete = __users.find(temp);
		toDelete->removeUserFromGroup();
		__users.remove(toDelete);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<Movie>>::NoSuchElementException &)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
	if (groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<GroupWatch> group(new GroupWatch(groupId));
		__groups.insert(group);
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

StatusType streaming_database::remove_group(int groupId)
{
	if (groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<GroupWatch> toDelete(new GroupWatch(groupId));
		__groups.remove(toDelete);
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

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	if (userId <= 0 || groupId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user = __users.find(tempUser);

		std::shared_ptr<GroupWatch> tempGroup(new GroupWatch(userId));
		std::shared_ptr<GroupWatch> group = __groups.find(tempGroup);
		group->addUser(user);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const AVLTree<std::shared_ptr<User>>::NoSuchElementException &)
	{
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

StatusType streaming_database::user_watch(int userId, int movieId)
{
	if (userId <= 0 || movieId <= 0)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<User> tempUser(new User(userId));
		std::shared_ptr<User> user = __users.find(tempUser);

		std::shared_ptr<Movie> tempMovie(new Movie(movieId));
		std::shared_ptr<Movie> movie = __movies_By_ID[(unsigned long)Genre::NONE].find(tempMovie);

		user->watch(movie->getGenre());
		movie->user_watch();
		// now we need to update the trees
		__movies_By_Rating_Views_reversedID[(unsigned long)movie->getGenre()].remove(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)movie->getGenre()].insert(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].remove(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].insert(movie);
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

StatusType streaming_database::group_watch(int groupId, int movieId)
{
	if (groupId <= 0 || movieId <= 0)
		return StatusType::INVALID_INPUT;

	try
	{
		std::shared_ptr<GroupWatch> tempGroup(new GroupWatch(groupId));
		std::shared_ptr<GroupWatch> group = __groups.find(tempGroup);

		std::shared_ptr<Movie> tempMovie(new Movie(movieId));
		std::shared_ptr<Movie> movie = __movies_By_ID[(unsigned long)Genre::NONE].find(tempMovie);

		group->watch(movie->getGenre());
		movie->group_watch(*group);

		// now we need to update the trees
		__movies_By_Rating_Views_reversedID[(unsigned long)movie->getGenre()].remove(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)movie->getGenre()].insert(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].remove(movie);
		__movies_By_Rating_Views_reversedID[(unsigned long)Genre::NONE].insert(movie);
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

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
	try
	{
		return output_t<int>(__movies_By_ID[(unsigned long)genre].getSize());
	}
	catch(const std::bad_alloc& e)
	{
		return output_t<int>(StatusType::ALLOCATION_ERROR);
	}
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	if(output == nullptr)
		return StatusType::INVALID_INPUT;
	
	try
	{
		// we need a function to raverce the avl tree
	}
	catch(const std::bad_alloc& e)
	{

	}
	
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	// TODO: Your code goes here
	return 2008;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	// TODO: Your code goes here
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}
