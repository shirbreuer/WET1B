#include "election.h"
#include <stdlib.h>
#include <stdio.h>
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z') 
//static functions








typedef struct area_votes_t {
int area_id;
Map tribe_votes;
struct area_votes_t* next;    
} *AreaVotes;


typedef struct  election_t {
    Map area;
    Map tribe;
    AreaVotes vote_list_head;
    Map results;
};


static AreaVotes nodeCreate(int area_id_to_add) {
    AreaVotes area_votes = malloc(sizeof(*area_votes));
    if(!area_votes){
        return NULL;
    }
    area_votes->area_id=area_id_to_add;
    area_votes->tribe_votes = mapCreate();
    area_votes->next=NULL;
    return area_votes;
}

static bool stringNameValidator(const char* stringName) {
    char* to_check;
    strcpy(to_check, stringName);
    while (*(to_check))
    {
        if (!(IS_LOWER(*(to_check)) || (*(to_check) == ' '))) {
            return false;
        }
        to_check++;
    }
    return true;
    
}

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    if (!election) {
    return NULL;
    }
    election->area = mapCreate();
    election->tribe = mapCreate();
    election->vote_list_head = NULL;
    election->results = mapCreate();
    if (!election->area || !election->tribe || !election->results) {
        mapDestroy(election->area);
        mapDestroy(election->tribe);
        mapDestroy(election->results);
        free(election);
        return NULL;
    }
    return election;
}
void electionDestroy(Election election) {
        mapDestroy(election->area);
        mapDestroy(election->tribe);
        mapDestroy(election->results);
        // list remove function
        free(election);
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) {
    if (!election || tribe_id == NULL || !tribe_name) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (mapGet(election->tribe, (char*)tribe_id)) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (!stringNameValidator(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }

    if (mapPut(election->tribe, (char*)tribe_id, tribe_name) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {
       if(!election || area_id == NULL || !area_name){
           return ELECTION_NULL_ARGUMENT;
       }
       if (area_id < 0){
           return ELECTION_INVALID_ID;
       }
       if(mapGet(election->area,(char*)area_id)){
           return ELECTION_AREA_ALREADY_EXIST;
       }
       if (!stringNameValidator(area_name)){
           return ELECTION_INVALID_NAME;
       }
       if (mapPut(election->area, (char*)area_id, area_name)== MAP_OUT_OF_MEMORY){
           electionDestroy(election);
           return ELECTION_OUT_OF_MEMORY;
       }
       return ELECTION_SUCCESS;
}
const char* electionGetTribeName (Election election, int tribe_id)
{
    if(!election){
        return NULL;
    }
    return((mapGet(election->tribe, (char*)tribe_id)==NULL)? NULL : 
}





ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {
    if (!election || tribe_id == NULL || !tribe_name) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (!mapGet(election->tribe, (char*)tribe_id)) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (!stringNameValidator(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->tribe, (char*)tribe_id, tribe_name) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }

    return ELECTION_SUCCESS;
}