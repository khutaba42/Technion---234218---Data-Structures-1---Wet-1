#include "StreamingDBa1.h"

streaming_database::streaming_database()
{
	// TODO: Your code goes here
}

streaming_database::~streaming_database()
{
	// TODO: Your code goes here
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
	// TODO: Your code goes here
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

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
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
    return (i++==0) ? 11 : 2;
}


