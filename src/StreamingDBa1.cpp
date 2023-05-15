#include "StreamingDBa1.h"

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
	if(userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> user(new User(userId, isVip));
		__users.insert(user);
	}
	catch(const std::bad_alloc& e)
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
	if(userId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		std::shared_ptr<User> temp(new User(userId));
		std::shared_ptr<User> toDelete = __users.find(temp);
		toDelete->removeUserFromGroup();
		__users.remove(toDelete);
	}
	catch(const std::bad_alloc& e)
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
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId, int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
	// TODO: Your code goes here
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	// TODO: Your code goes here
	output[0] = 4001;
	output[1] = 4002;
	return StatusType::SUCCESS;
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
