#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"


void mergesort(fields **faddress, int sortingtypes)
{
    fields * front = faddress;
    fields * first = NULL;
    fields * second = NULL;
    if((front == NULL) || (front->next == NULL))
    {
        return;
    }
    spliting(front, &first, &second);
    mergesort(&first, sortingtypes);
    mergesort(&second, sortingtypes);
    faddress = sorted(first, second, sortingtypes);

    return;
}

fields * sorted(fields * first, fields * second, int sortingtypes)
{
    fields * output = NULL;
    if(first == NULL)
    {
        return second;
    }
    else if(second == NULL)
    {
        return first;
    }
    int isNum = 0;
    char * fStrVal = "";
    char * sStrVal = "";
    double fNumVal = 0;
    double sNumVal = 0;
    
    switch(sortingtypes)
    {
        case 0: fStrVal = first->color; sStrVal = second->color; break;
        case 1: fStrVal = first->director_name; sStrVal = second->director_name; break;
        case 2: isNum = 1; fNumVal = first->num_critic_for_reviews; sNumVal = second->num_critic_for_reviews; break;
        case 3: isNum = 1; fNumVal = first->duration; sNumVal = second->duration; break;
        case 4: isNum = 1; fNumVal = first->director_facebook_likes; sNumVal = second->director_facebook_likes; break;
        case 5: isNum = 1; fNumVal = first->actor_3_facebook_likes; sNumVal = second->actor_3_facebook_likes; break;
        case 6: fStrVal = first->actor_2_name; sStrVal = second->actor_2_name; break;
        case 7: isNum = 1; fNumVal = first->actor_1_facebook_likes; sNumVal = second->actor_1_facebook_likes; break;
        case 8: isNum = 1; fNumVal = first->gross; sNumVal = second->gross; break;
        case 9: fStrVal = first->genres; sStrVal = second->genres; break;
        case 10: fStrVal = first->actor_1_name; sStrVal = second->actor_1_name; break;
        case 11: fStrVal = first->movie_title; sStrVal = second->movie_title; break;
        case 12: isNum = 1; fNumVal = first->num_voted_users; sNumVal = second->num_voted_users; break;
        case 13: isNum = 1; fNumVal = first->cast_total_facebook_likes; sNumVal = second->cast_total_facebook_likes; break;
        case 14: fStrVal = first->actor_3_name; sStrVal = second->actor_3_name; break;
        case 15: isNum = 1; fNumVal = first->facenumber_in_poster; sNumVal = second->facenumber_in_poster; break;
        case 16: fStrVal = first->plot_keywords; sStrVal = second->plot_keywords; break;
        case 17: fStrVal = first->movie_imdb_link; sStrVal = second->movie_imdb_link; break;
        case 18: isNum = 1; fNumVal = first->num_user_for_reviews; sNumVal = second->num_user_for_reviews; break;
        case 19: fStrVal= first->language; sStrVal = second->language; break;
        case 20: fStrVal = first->country; sStrVal = second->country; break;
        case 21: fStrVal = first->content_rating; sStrVal = second->content_rating; break;
        case 22: isNum = 1; fNumVal = first->budget; sNumVal = second->budget; break;
        case 23: isNum = 1; fNumVal = first->title_year; sNumVal = second->title_year; break;
        case 24: isNum = 1; fNumVal = first->actor_2_facebook_likes; sNumVal = second->actor_2_facebook_likes; break;
        case 25: isNum = 1; fNumVal = first->imdb_score; sNumVal = second->imdb_score; break;
        case 26: isNum = 1; fNumVal = first->aspect_ratio; sNumVal = second->aspect_ratio; break;
        case 27: isNum = 1; fNumVal = first->movie_facebook_likes; sNumVal = second->movie_facebook_likes; break;
        default: printf("Fatal Error: Something went wrong with selecting the category to sort by.\n");
    }
    if(isNum == 0)
    {
        if (strcmp(fStrVal, "-1") == 0) {
            output = first;
            output->next = sorted(first->next, second, sortingtypes);
        }
        else if (strcmp(sStrVal, "-1") == 0) {
            output = second;
            output->next = sorted(first, second->next, sortingtypes);
        }
        else
        { 
            if(strcmp(fStrVal, sStrVal) <= 0)
            {
                output = first;
                output->next = sorted(first->next, second, sortingtypes);
            }
            else
            {
                output = second;
                output->next = sorted(first, second->next, sortingtypes);
            }
        }
    }
    else if(isNum == 1)
    {
        if (fNumVal == -1){
            output = first;
            output->next = sorted(first->next, second, sortingtypes);
        }
        else if (sNumVal == -1) {
            output = second;
            output->next = sorted(first, second->next, sortingtypes);
        }
        else {
            if(fNumVal <= sNumVal)
            {
                output = first;
                output->next = sorted(first->next, second, sortingtypes);
            }
            else
            {
                output = second;
                output->next = sorted(first, second->next, sortingtypes);
            }
        }
    }
    return output;
}

void spliting(fields * front, fields ** first, fields ** second)
{
    fields * ekum;
    fields * duwam;
    if(front == NULL || front->next == NULL)
    {
        *first = front;
        *second = NULL;
    }
    else {
        duwam = front;
        ekum = front->next;
        while(ekum != NULL)
        {
            ekum = ekum->next;
            if(ekum != NULL)
            {
                duwam = duwam->next;
                ekum = ekum->next;
            }
        }
        *first = front;
        *second = duwam->next;
        duwam->next = NULL;
    }
    return;
}