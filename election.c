#include "election.h"
#include "area_votes.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z') 

#define GET_VOTE(map, tribe_id) mapGet(map,(char*)tribe_id) == NULL ? 0 : (int)mapGet(map,(char*)tribe_id); // re-check





typedef struct  election_t {
    Map area;
    Map tribe;
    AreaVotes vote_list_head;
    Map results;
};




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
       if(!election || (char*)area_id == NULL || !area_name) {
           return ELECTION_NULL_ARGUMENT;
       }
       if (area_id < 0) {
           return ELECTION_INVALID_ID;
       }
       if(mapGet(election->area,(char*)area_id)) {
           return ELECTION_AREA_ALREADY_EXIST;
       }
       if (!stringNameValidator(area_name)) {
           return ELECTION_INVALID_NAME;
       }
       if (mapPut(election->area, (char*)area_id, area_name)== MAP_OUT_OF_MEMORY) {
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
    if (!election || (char*)tribe_id == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if(mapRemove(election->tribe, (char*)tribe_id) == MAP_ITEM_DOES_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    AreaVotes current_area_votes = election->vote_list_head;
    while(current_area_votes){
        mapRemove(current_area_votes->tribe_votes,(char*)tribe_id);
        current_area_votes=current_area_votes->next;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election,  AreaConditionFunction should_delete_area) {
    if(!election || !should_delete_area){
        return ELECTION_NULL_ARGUMENT;
    }
    for(char* key = mapGetFirst(election->area);key;key=mapGetNext(election->area)){
        if(should_delete_area((int)key)) {
            mapRemove(election->area,key);
            AreaVotes area_votes_to_remove = areaVotesGet(election->vote_list_head,(int)key);
            areaVotesDestroy(election->vote_list_head,area_votes_to_remove);
        }
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    ElectionResult validator = preVoteValidator(election, area_id, tribe_id, num_of_votes);
    if (validator != ELECTION_SUCCESS) {
        return validator;
    }
    AreaVotes area_votes = areaVotesGet(election->vote_list_head, area_id);
    if (!area_votes){
        if (!(area_votes = areaVotesCreate(area_id))) { // re-check
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
        areaVotesLink(election->vote_list_head , area_votes);
    }
    int current_vote_counter = GET_VOTE(election->vote_list_head->tribe_votes, tribe_id);
    assert(area_votes->tribe_votes);
    if ((mapPut(area_votes->tribe_votes , (char*)tribe_id , current_vote_counter + num_of_votes)) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return validator;
}

ElectionResult electionRemoveVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult validator = preVoteValidator(election, area_id, tribe_id, num_of_votes);
    if (validator != ELECTION_SUCCESS) {
        return validator;
    }
    int current_vote_counter = GET_VOTE(election->vote_list_head->tribe_votes, tribe_id);
    if (current_vote_counter <= num_of_votes) {
        num_of_votes = current_vote_counter;
    }
    if(mapPut(election->vote_list_head->tribe_votes, tribe_id, (char*)(current_vote_counter-num_of_votes)) != MAP_SUCCESS) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return validator;
}

Map electionComputeAreasToTribesMapping (Election election) {
    if(!election->results) {
        return NULL;
    }
    while (election->vote_list_head) {
        char* tribe_winner_id = mapGetFirst(election->vote_list_head->tribe_votes);
        int tribe_winner_votes = (int)mapGet(election->vote_list_head->tribe_votes, tribe_winner_id);
        for (char* temp_tribe_id = (mapGetNext(election->vote_list_head->tribe_votes));temp_tribe_id;temp_tribe_id = (mapGetNext(election->vote_list_head->tribe_votes)))
        {
            int temp_tribe_votes = (int)mapGet(election->vote_list_head->tribe_votes, temp_tribe_id);
            if ((temp_tribe_votes > tribe_winner_votes) || ((temp_tribe_votes == tribe_winner_votes)&&(int)temp_tribe_id < (int)tribe_winner_id)) {
                tribe_winner_id = temp_tribe_id;
                tribe_winner_votes = temp_tribe_votes;
            }            
        }
        assert(election->results && election->vote_list_head->area_id && tribe_winner_id);
        mapPut(election->results, (char*) (election->vote_list_head->area_id), tribe_winner_id);
        election->vote_list_head = election->vote_list_head->next;
    }
    return election->results;
}