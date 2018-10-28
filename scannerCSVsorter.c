#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "scannerCSVsorter.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


movie* freepointer(movie* nodes){
    if(nodes == NULL){
        return NULL;
    }
    
    free(nodes->color);
    free(nodes->director_name);
    free(nodes->actor_2_name);
    free(nodes->genres);
    free(nodes->actor_1_name);
    free(nodes->movie_title);
    free(nodes->actor_3_name);
    free(nodes->plot_keywords);
    free(nodes->movie_imdb_link);
    free(nodes->language);
    free(nodes->country);
    free(nodes->content_rating);
    free(freepointer(nodes->next));
    
    
    
    return NULL;

}


fields * nodecreator(char** categorytoken, int containsquotes){
    fields * ptr = (fields*)malloc(sizeof(fields));
    
    ptr->color = strdup(categorytoken[0]);
    ptr->director_name = strdup(categorytoken[1]);
    ptr->num_critic_for_reviews = atol(categorytoken[2]);
    ptr->duration = atol(categorytoken[3]); 
    ptr->director_facebook_likes = atol(categorytoken[4]);
    ptr->actor_3_facebook_likes = atol(categorytoken[5]);
    ptr->actor_2_name = strdup(categorytoken[6]);
    ptr->actor_1_facebook_likes = atol(categorytoken[7]);
    ptr->gross = atol(categorytoken[8]);
    ptr->genres = strdup(categorytoken[9]);
    ptr->actor_1_name = strdup(categorytoken[10]);
    ptr->movie_title = strdup(categorytoken[11]);
    ptr->num_voted_users = atol(categorytoken[12]);
    ptr->cast_total_facebook_likes = atol(categorytoken[13]);
    ptr->actor_3_name = strdup(categorytoken[14]);
    ptr->facenumber_in_poster = atol(categorytoken[15]);
    ptr->plot_keywords = strdup(categorytoken[16]);
    ptr->movie_imdb_link = strdup(categorytoken[17]);
    ptr->num_user_for_reviews = atol(categorytoken[18]);
    ptr->language = strdup(categorytoken[19]);
    ptr->country = strdup(categorytoken[20]);
    ptr->content_rating = strdup(categorytoken[21]);
    ptr->budget = atol(categorytoken[22]);
    ptr->title_year = atol(categorytoken[23]);
    ptr->actor_2_facebook_likes = atol(categorytoken[24]);
    ptr->imdb_score = atof(categorytoken[25]);
    ptr->aspect_ratio= atof(categorytoken[26]);
    ptr->movie_facebook_likes = atol(categorytoken[27]);
    if(containsquotes == 0){
        ptr->quotes = 0;
    }
    else{
        ptr->quotes = 1;
    }
    
    ptr->next = NULL;

    return ptr;

}


char* trimmer(char* string){
    char* endpartstring;
    
    
    if(string == NULL){         
        return NULL;
    }
    
    
    while(*string == ' '){
        string = string + 1;
    }
    
    if(*string == ' '){
        return NULL;
    }
    
    endpartstring = string + strlen(string) - 1;
    
    
    while(endpartstring > string && (*endpartstring == ' ')){
        endpartstring = endpartstring - 1;
    }
    
    
    *(endpartstring + 1) = '\0';
    
    return string;
}



int categorysorter(char* sortbycolumn){
    if(strcmp(sortbycolumn, "color") == 0) {
        return 0;
    }
    else if(strcmp(sortbycolumn, "director_name") == 0) {
        return 1;
    }
    else if(strcmp(sortbycolumn, "num_critic_for_reviews") == 0) {
        return 2;
    }
    else if(strcmp(sortbycolumn, "duration") == 0) {
        return 3;
    }
    else if(strcmp(sortbycolumn, "director_facebook_likes") == 0) {
         return 4;
    }
    else if(strcmp(sortbycolumn, "actor_3_facebook_likes") == 0) {
        return 5;
    }
    else if(strcmp(sortbycolumn, "actor_2_name") == 0) {
        return 6;
    }
    else if(strcmp(sortbycolumn, "actor_1_facebook_likes") == 0) {
        return 7;
    }
    else if(strcmp(sortbycolumn, "gross") == 0) {
        return 8;
    }
    else if(strcmp(sortbycolumn, "genres") == 0) {
        return 9;
    }
    else if(strcmp(sortbycolumn, "actor_1_name") == 0) {
        return 10;
    }
    else if(strcmp(sortbycolumn, "movie_title") == 0) {
        return 11;
    }
    else if(strcmp(sortbycolumn, "num_voted_users") == 0) {
        return 12;
    }
    else if(strcmp(sortbycolumn, "cast_total_facebook_likes") == 0) {
        return 13;
    }
    else if(strcmp(sortbycolumn, "actor_3_name") == 0) {
        return 14;
    }
    else if(strcmp(sortbycolumn, "facenumber_in_poster") == 0) {
        return 15;
    }
    else if(strcmp(sortbycolumn, "plot_keywords") == 0) {
        return 16;
    }
    else if(strcmp(sortbycolumn, "movie_imdb_link") == 0) {
        return 17;
    }
    else if(strcmp(sortbycolumn, "num_user_for_reviews") == 0) {
        return 18;
    }
    else if(strcmp(sortbycolumn, "language") == 0) {
        return 19;
    }
    else if(strcmp(sortbycolumn, "country") == 0) {
        return 20;
    }
    else if(strcmp(sortbycolumn, "content_rating") == 0) {
        return 21;
    }
    else if(strcmp(sortbycolumn, "budget") == 0) {
        return 22;
    }
    else if(strcmp(sortbycolumn, "title_year") == 0) {
        return 23;
    }
    else if(strcmp(sortbycolumn, "actor_2_facebook_likes") == 0) {
        return 24;
    }
    else if(strcmp(sortbycolumn, "imdb_score") == 0) {
        return 25;
    }
    else if(strcmp(sortbycolumn, "aspect_ratio") == 0) {
        return 26;
    }
    else if(strcmp(sortbycolumn, "movie_facebook_likes") == 0) {
        return 27;
    }
    else {
        printf("Fatal Error:  input a proper category title\n");
        exit(0);
    }
}

void sortingfile(char* fileName, char** argv, int sortingtypes, char* path){
    
    fields * moviehead = NULL;
    
    
    
    FILE* pointerfile = fopen(fileName, "r");

    if(pointerfile == NULL){
        printf("Fatal Error: The file does not exist.\n");
        exit(0);
    }
    char* categories = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";
    char* line = (char*)malloc(sizeof(char)*500);
    memset(line, '\0', 500);
    
    fgets(line, 500, pointerfile);
    
    
    line[strcspn(line, "\n\r")] = '\0';
    
    if(strcmp(line, categories) !=  0){
       
        exit(0);
    }
   
    char* filenewname = (char*)malloc(sizeof(char)*((strlen(path) + strlen(fileName) + 30)));
    
    int fileendname = strlen(fileName) - 4;
    
    char* filenametran = (char*)malloc((sizeof(char)*(strlen(fileName) + 1)));
    filenametran = memcpy(filenametran, fileName, fileendname);
    
    filenametran[fileendname + 1] = '\0';
    
    int endOfoutputpath = strlen(path);
    memcpy(filenewname, path, endOfoutputpath);
    strcat(filenewname, "/");
    strcat(filenewname, filenametran);
    strcat(filenewname, "-sorted-");
    strcat(filenewname, argv[2]);
    strcat(filenewname, ".csv");
    
    int i;
    int j;
    int containsquotes = 0;
    FILE* finalfile = fopen(filenewname, "r");
    if(finalfile != NULL){
        
        exit(0);
    }
    else{
       
        finalfile = fopen(filenewname, "w");
    }
    
    if(line == NULL){
        printf("Fatal Error: The input file is blank.\n");
        exit(0);
    }

    
    fields * backmovies = NULL;
    backmovies = (fields*)malloc(sizeof(fields));

    char* temporarytoken;
    char* quotestotokens;
    char* temporaryline = (char*)malloc(sizeof(char)*500);
    char* parsesegment1;
    char* movietitletoken;
    char* parsesegment3;

    
    char** categorytoken = (char**)(malloc(sizeof(char*) * 28));

    
    for(i = 0; i < 28; i++){
        categorytoken[i] = (char*)(malloc(sizeof(char) * 70));
        for(j = 0; j < 70; j++){
            categorytoken[i][j] = '\0';
        }
    }

    while(fgets(line, 500, pointerfile) != NULL){

       

        memcpy(temporaryline, line, 500);

        

        temporarytoken = strsep(&line, "\"");

        
        if (strcmp(temporarytoken, temporaryline) == 0){
            temporarytoken = strsep(&temporaryline, ",");

            for(i = 0; i < 28 && temporarytoken != NULL; i++){
    
                temporarytoken = trimmer(temporarytoken);

                
    
                if(strcmp(temporarytoken, "") == 0){
                    
                    categorytoken[i][0] = '-';
                    categorytoken[i][1] = '1';
                }
                else{
                    categorytoken[i] = temporarytoken;
                }
                
    
                temporarytoken = strsep(&temporaryline, ",");
            }
        }
        else{
            parsesegment1 = temporarytoken;
            movietitletoken = strsep(&line, "\"");
            parsesegment3 = strsep(&line, "\"");

            quotestotokens = strsep(&parsesegment1, ",");

            for(i = 0; i < 12 && quotestotokens != NULL; i++){
                quotestotokens = trimmer(quotestotokens);
    
                categorytoken[i] = quotestotokens;
    
                quotestotokens = strsep(&parsesegment1, ",");
    
            }

            
            movietitletoken = trimmer(movietitletoken);
            containsquotes = 1;

            categorytoken[11] = movietitletoken;

            quotestotokens = strsep(&parsesegment3, ",");
            
            quotestotokens = strsep(&parsesegment3, ",");

            for(i = 12; i < 28 && quotestotokens != NULL; i++){
    
                quotestotokens = trimmer(quotestotokens);
    
                categorytoken[i] = quotestotokens;
    
                quotestotokens = strsep(&parsesegment3, ",");
            }
        }

        

        if(moviehead == NULL){
            moviehead = nodecreator(categorytoken, containsquotes);

            backmovies = moviehead;
        }

        else{
            
            backmovies->next = nodecreator(categorytoken, containsquotes);

            backmovies = backmovies->next;
        }   
    
        for(i = 0; i < 28; i++){
            for(j = 0; j < 70; j++){
                categorytoken[i][j] = '\0';
            }
        }

        
        line = realloc(line, 500);
        temporaryline = realloc(temporaryline, 500);
        containsquotes = 0;
    }

   
    mergesort(&moviehead, sortingtypes);

    fields* currentpointer = (fields*)malloc(sizeof(fields));
    currentpointer = moviehead;
    
   
    nodeprinting(currentpointer, finalfile);

   
    freepointer(moviehead);
    free(backmovies);
    
}


void directorytraversal(char* path, char** argv, int sortingtypes, int sumprocess, int printed, int newoutputdir, char* outputpath){
    int printedpid = printed;
    
    int allprocesses = sumprocess;
    int status;
    
    int pid = -1;
    DIR* pointertodirec;
    struct dirent* currobject;
    struct stat buffer;
   if((pointertodirec = opendir(path)) == NULL){
        printf("Fatal Error: The directory does not exist.\n");
        exit(0);
    }
    chdir(path);
   while((currobject = readdir(pointertodirec)) != NULL){
        
        lstat(currobject->directoryname, &buffer);
        if((strcmp(currobject->directoryname, "..") == 0) || (strcmp(currobject->directoryname, ".") == 0)){
                    continue;
        }
        if(S_ISDIR(buffer.st_mode)){
            if(printedpid != getpid()) {
                if(pid == -1){
                    printf("%d,", getpid());
                    printedpid = getpid();
                }
            }
            fflush(0);
            pid = fork();
            if(pid != 0) {
                allprocesses++;
                }
            if(pid == 0) {
                allprocesses = 0;
                if(printedpid != getpid()) {
                    printf("%d,", getpid());
                    printedpid = getpid();
                }
                
                if((strcmp(currobject->directoryname, "..") == 0) || (strcmp(currobject->directoryname, ".") == 0)){
                    exit(allprocesses);
                }
                
                char* newPath = NULL;
                newPath = getcwd(newPath, 1024);
                
                strcat(newPath, "/");
                strcat(newPath, currobject->directoryname);
                directorytraversal(newPath, argv, sortingtypes, allprocesses, printedpid, newoutputdir, outputpath);
                printf("\nis it here\n");
            }
            
            
        }
        else{
            if(strstr(currobject->directoryname, "-sorted")){
                    continue;
            }
            else if((strstr(currobject->directoryname, ".csv")) == NULL){
                    continue;
            }
            else{
                if(printedpid != getpid()) {
                    if(pid == -1){
                        printf("%d,", getpid());
                        printedpid = getpid();
                    }
                }
                fflush(0);
                pid = fork();
                if(pid != 0) {
                    allprocesses++;
                }
                if(pid == 0) {
                    allprocesses = 0;
                }
                
                if(pid == 0){
                    if(printedpid != getpid()) {
                        printf("%d,", getpid());
                        printedpid = getpid();
                    }
                    if(newoutputdir == 1){
                        sortingfile(currobject->directoryname, argv, sortingtypes, outputpath);
                    }
                    else{
                        sortingfile(currobject->directoryname, argv, sortingtypes, ".");
                    }
                }
            }
        }
        }
    chdir("..");
    closedir(pointertodirec);
    int i = 0;
    int total = allprocesses;
    for(i = 0; i < total; i++) {
        wait(&status);
        if(WIFEXITED(status)) {
            allprocesses += WEXITSTATUS(status);

        }
    }
    
    wait(NULL);
    
    exit(allprocesses);
    
}


void nodeprinting(fields * currentpointer, FILE* finalfile)
{
    fprintf(finalfile, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");

    int i;
    while(currentpointer != NULL){
        
        for(i = 0; i < 28; i++){
            switch(i){
                case 0: 
            
                    if(strcmp(currentpointer->color, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->color);
                    break;
                case 1: 
                    if(strcmp(currentpointer->director_name, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->director_name); 
                    break;
                case 2: 
                    if(currentpointer->num_critic_for_reviews == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->num_critic_for_reviews); 
                    break;
                case 3: 
                    if(currentpointer->duration == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->duration); 
                    break;
                case 4: 
                    if(currentpointer->director_facebook_likes == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->director_facebook_likes); 
                    break;
                case 5: 
                    if(currentpointer->actor_3_facebook_likes == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->actor_3_facebook_likes); 
                    break;
                case 6: 
                    if(strcmp(currentpointer->actor_2_name, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->actor_2_name); 
                    break;
                case 7: 
                    if(currentpointer->actor_1_facebook_likes == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->actor_1_facebook_likes); 
                    break;
                case 8:
                    if(currentpointer->gross == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->gross); 
                    break;
                case 9: 
                    if(strcmp(currentpointer->genres, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->genres); 
                    break;
                case 10: 
                    if(strcmp(currentpointer->actor_1_name, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->actor_1_name); 
                    break;
                case 11:  
                    if(strcmp(currentpointer->movie_title, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    if(currentpointer->quotes == 1){
                        fprintf(finalfile,"\"%s\",", currentpointer->movie_title);
                    } 
                    else{
                        fprintf(finalfile,"%s,", currentpointer->movie_title);
                    }
                    break;
                case 12: 
                    if(currentpointer->num_voted_users == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->num_voted_users); 
                    break;
                case 13: 
                    if(currentpointer->cast_total_facebook_likes == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->cast_total_facebook_likes);
                    break;
                case 14: 
                    if(strcmp(currentpointer->actor_3_name, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->actor_3_name);
                    break;
                case 15:  
                    if(currentpointer->facenumber_in_poster == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->facenumber_in_poster); 
                    break;
                case 16: 
                    if(strcmp(currentpointer->plot_keywords, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->plot_keywords); 
                    break;
                case 17: 
                    if(strcmp(currentpointer->movie_imdb_link, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->movie_imdb_link); 
                    break;
                case 18: 
                    if(currentpointer->num_user_for_reviews == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->num_user_for_reviews); 
                    break;
                case 19: 
                    if(strcmp(currentpointer->language, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->language); 
                    break;
                case 20: 
                    if(strcmp(currentpointer->country, "-1") == 0){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->country); 
                    break;
                case 21: 
                    if(strcmp(currentpointer->content_rating, "-1") == 0 || (strcmp(currentpointer->content_rating,"-1-1") == 0)){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%s,", currentpointer->content_rating); 
                    break;
                case 22: 
                    if(currentpointer->budget == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->budget); 
                    break;
                case 23: 
                    if(currentpointer->title_year == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->title_year); 
                    break;
                case 24: 
                    if(currentpointer->actor_2_facebook_likes == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%ld,", currentpointer->actor_2_facebook_likes); 
                    break;
                case 25:  
                    if(currentpointer->imdb_score == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%g,", currentpointer->imdb_score); 
                    break;
                case 26: 
                    if(currentpointer->aspect_ratio == -1){
                        fprintf(finalfile,",");
                        break;
                    }
                    fprintf(finalfile,"%g,", currentpointer->aspect_ratio); 
                    break;
                case 27: 
                    if(currentpointer->movie_facebook_likes == -1){
                        fprintf(finalfile,"\n");
                        break;
                    }
                    fprintf(finalfile,"%ld\n", currentpointer->movie_facebook_likes); 
                    break;
                default: 
                    printf("Fatal Error: The category you would like to sort by is not present.\n");
                    exit(0);
            }
        }
            currentpointer = currentpointer->next;
    }   
}

int main(int argc, char ** argv) {
    int allprocesses = 0;
    int status;
    int newoutputdir = 0;
    
    int initPID = getpid();
    printf("Initial PID: %d\n", initPID);
    printf("PIDS of all child processes: ");

    
    if((argc == 1) || ((argc % 2) == 0)){
        printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
        exit(0);
    }
    
    if (strcmp(argv[1], "-c") != 0){
        printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
        exit(0);
    }
    
    int sortingtypes = -1;
    sortingtypes = categorysorter(argv[2]);
    
    int forkPid;
    
    DIR* searchDir = NULL;
    DIR* outputDir = NULL;
    char* outputpath = NULL;
    
    
    if((argc == 5) && (argv[3] != NULL)){
        if((strcmp(argv[3], "-d") != 0) && (strcmp(argv[3], "-o") != 0)){
            printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
            exit(0);
        }
        else if(strcmp(argv[3], "-o") == 0) {
            if(argv[4] == NULL){
                printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> -o <outputdir>\n");
                exit(0);
            }
            else{
                outputDir = opendir(argv[4]);
                if(outputDir == NULL){
                    printf("The file directory does not exist\n");
                    exit(0);
                }
                newoutputdir = 1;
                if(strcmp(argv[4], "./") == 0){
                    
                    outputpath = getcwd(outputpath, 1024);
                }
                else if(strstr(argv[4], "../")){
                    outputpath = getcwd(outputpath, 1024);
                    outputpath = strcat(outputpath, "/");
                    outputpath = strcat(outputpath, argv[4]);
                    
                }
                else{
                    outputpath = strdup(argv[4]);
                }
                
            }
        }
        
        else{
            if(argv[4] == NULL){
                printf("Fatal Error:  use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
                exit(0);
            }
            else{
                searchDir = opendir(argv[4]);
                if(searchDir == NULL){
                    printf("The file directory does not exist\n");
                    exit(0);
                }
                fflush(0);
                forkPid = fork();
                if(forkPid != 0) {
                    allprocesses++;
                    
                }
                if(forkPid == 0) {
                    allprocesses = 0;
                    
                }
                if(forkPid == 0){
                    
                    directorytraversal(argv[4], argv, sortingtypes, allprocesses, 0, newoutputdir, ".");
                }
            }
        }
    }

    if((argc == 7) && (argv[5] != NULL)){
        if(strcmp(argv[5], "-o") != 0){
            printf("Fatal Error:  use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
            exit(0);
        }
        else if(strcmp(argv[3], "-d") != 0){
            printf("Fatal Error: = use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
            exit(0);
        }
        else{
            if(argv[6] == NULL){
                printf("Fatal Error: use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
                exit(0);
            }
            else{
                outputDir = opendir(argv[6]);
                if(outputDir == NULL){
                    printf("The file directory does not exist\n");
                    exit(0);
                }
                newoutputdir = 1;
                
                if(strcmp(argv[6], "./") == 0){
                    
                    outputpath = getcwd(outputpath, 1024);
                }
                else if(strstr(argv[6], "../")){
                    outputpath = getcwd(outputpath, 1024);
                    outputpath = strcat(outputpath, "/");
                    outputpath = strcat(outputpath, argv[6]);
                }
                else{
                    outputpath = strdup(argv[6]);
                }
            }

            if(argv[4] == NULL){
                printf("Fatal Error:  use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
                exit(0);
            }
            else{
                searchDir = opendir(argv[4]);
                if(searchDir == NULL){
                    printf("The file directory does not exist\n");
                    exit(0);
                }
                fflush(0);
                forkPid = fork();
                if(forkPid != 0) {
                    allprocesses++;
                    
                }
                if(forkPid == 0) {
                    allprocesses = 0;
                    
                }
                if(forkPid == 0){
                    
                    directorytraversal(argv[4], argv, sortingtypes, allprocesses, 0, newoutputdir, outputpath);
                }
            }
        }
    }

    char* cwd = NULL;
    
    if(searchDir == NULL){
        cwd = getcwd(cwd, 1024);
        if(cwd == NULL) {
            printf("Something wrong with default directory. \n");
            exit(0);
        }
        
        searchDir = opendir(cwd);
        if(searchDir == NULL){
            printf("Something wrong with default directory.\n");
            exit(0);
        }
        
        fflush(0);
        forkPid = fork();
        if(forkPid != 0) {
            allprocesses++;
            
        }
        if(forkPid == 0) {
            allprocesses = 0;
            
        }
        
        if(forkPid == 0){
            if(outputpath == NULL){
                outputpath = ".";
            }
            
            
            directorytraversal(cwd, argv, sortingtypes, allprocesses, 0, newoutputdir, outputpath);
        }
    }
    
    int i = 0;
    int total = allprocesses;
    for(i = 0; i < total; i++) {
        wait(&status);
        if(WIFEXITED(status)) {
            allprocesses += WEXITSTATUS(status);
        
        }
    }
    wait(NULL);
    if(cwd != NULL){
        free(cwd);
    }
    fflush(0);
    if(getpid() == initPID){
        allprocesses++;
        printf("\nTotal processes: %d\n", allprocesses);
    }
    
    exit(0);
}