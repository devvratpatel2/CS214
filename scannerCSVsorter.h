#ifndef SORTER_H
#define SORTER_H
	typedef struct fields
	{
		char* color;
		char* director_name;
		long num_critic_for_reviews;
		long duration;
		long director_facebook_likes;
		long actor_3_facebook_likes;
		char* actor_2_name;
		long actor_1_facebook_likes;
		long gross;
		char* genres;
		char* actor_1_name;
		char* movie_title;
		long num_voted_users;
		long cast_total_facebook_likes;
		char* actor_3_name;
		long facenumber_in_poster;
		char* plot_keywords;
		char* movie_imdb_link;
		long num_user_for_reviews;
		char* language;
		char* country;
		char* content_rating;
		long budget;
		long title_year;
		long actor_2_facebook_likes;
		double imdb_score;
		double aspect_ratio;
		long movie_facebook_likes;
		int quotes;

		struct movies * next;
	}fields; 
	void nodeprinting(fields * currentpointer, FILE* finalfile);
	void mergesort(fields ** front, int sortingtypes);
	void sortingfile(char* fileName, char** argv, int sortingtypes, char* path);
	void directorytraversal(char* path, char** argv, int sortingtypes, int sumprocess, int printed, int newoutputdir, char* outputpath);
	fields* freepointer(fields* nodes);
	fields* nodecreator(char** categorytoken, int containsquotes);
	char* trimmer(char* string);
	int categorysorter(char* sortbycolumn);
	
	fields * sorted(fields * first, fields * second, int sortingtypes);
	void spiliting(fields * source, fields ** first, fields ** second);
#endif