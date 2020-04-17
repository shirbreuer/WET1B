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

// static const char* electionGetAreaName (Election election, int area_id)
// {
//     if(!election){
//         return NULL;
//     }
//     char* area_name = mapGet(election->area, (char*)area_id);
//     return((area_name==NULL)? NULL : area_name);
// }

static ElectionResult preVoteValidator(Election election, int area_id, int tribe_id, int num_of_votes){
    if(!election|| area_id==NULL || tribe_id == NULL | num_of_votes == NULL){
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id<0 || area_id<0){
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes<=0){
        return ELECTION_INVALID_VOTES;
    }
    if (!mapContains(election->area,(char*)area_id)){
        return ELECTION_AREA_NOT_EXIST;
    }
    if (!mapContains(election->tribe,(char*)tribe_id)){
        return ELECTION_TRIBE_NOT_EXIST;
    }
    return ELECTION_SUCCESS;
}

static AreaVotes nodeGet(AreaVotes head, int area_id) {
    while (head)
    {
        if (head->area_id == area_id) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

static void nodeLink(AreaVotes previous, AreaVotes to_link) {
    to_link->next = previous->next;
    previous->next = to_link;
}

static void nodeDestroy(AreaVotes head, AreaVotes to_destroy) {
    if (!head || !to_destroy) {
        return;
    }
    while (head->next != to_destroy){
        head = head->next;  // Is the value of the head is changing here ? 
    }
    nodeLink(head, to_destroy->next);
    mapDestroy(to_destroy->tribe_votes);
    free(to_destroy);
}

static void listDestroy(AreaVotes head) {
    if(!head) {
        return;
    }
    while(head->next) {
        nodeDestroy(head,head->next);
    }
    mapDestroy(head->tribe_votes);
    free(head);
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
    if (!election) {
        return;
    }
    mapDestroy(election->area);
    mapDestroy(election->tribe);
    mapDestroy(election->results);
    listDestroy(election->vote_list_head);
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
    char* tribe_name = mapGet(election->tribe, (char*)tribe_id);
    return((tribe_name==NULL)? NULL : tribe_name);
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {
    if (!election || tribe_id == NULL || !tribe_name) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (electionGetTribeName(election, tribe_id) == NULL) {
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

ElectionResult electionRemoveTribe (Election election, int tribe_id) {
    if (!election || tribe_id == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (electionGetTribeName(election, tribe_id) == NULL) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (mapRemove(election->tribe, (char*)tribe_id) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;

}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    ElectionResult validor = preVoteValidator(election, area_id, tribe_id, num_of_votes);
    if (validor != ELECTION_SUCCESS) {
        return validor;
    }
    AreaVotes area_node = nodeGet(election->vote_list_head, area_id);
    if (!area_node){
        if (!(area_node = nodeCreate(area_id))) { // re-check
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
        nodeLink(election->vote_list_head , area_node);
    }
    char* current_vote_counter_string = mapGet(area_node,(char*)tribe_id);
    int current_vote_counter = (current_vote_counter_string == NULL ? 0 :  (int)current_vote_counter_string);
    asssert(area_node->tribe_votes);
    if ((mapPut(area_node->tribe_votes , (char*)tribe_id , current_vote_counter + num_of_votes)) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult validor = preVoteValidator(election, area_id, tribe_id, num_of_votes);
    if (validor != ELECTION_SUCCESS) {
        return validor;
    }
    int current_vote_counter = (int)mapGet(election->vote_list_head->tribe_votes, tribe_id);
    if (current_vote_counter <= num_of_votes) {
        num_of_votes = current_vote_counter;
    }
    if(mapPut(election->vote_list_head->tribe_votes, tribe_id, (char*)(current_vote_counter-num_of_votes)) != MAP_SUCCESS) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
}