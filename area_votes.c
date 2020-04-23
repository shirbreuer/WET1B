#include "area_votes.h"
#include "map.h" // change to mtm_map/map.h before submitting
#include <stdlib.h>


struct area_votes_t {
int area_id;
Map tribe_votes;
struct area_votes_t* next;    
};


AreaVotes areaVotesCreate(int area_id_to_add) {
    AreaVotes area_votes = malloc(sizeof(*area_votes));
    if(!area_votes) {
        return NULL;
    }
    area_votes->area_id=area_id_to_add;
    area_votes->tribe_votes = mapCreate();
    if(!area_votes->tribe_votes) {
        free(area_votes);
        return NULL;
    }
    area_votes->next=NULL;
    return area_votes;
}


AreaVotes areaVotesGet(AreaVotes head, int area_id) {
    while (head) {
        if (head->area_id == area_id) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

AreaVotes areaVotesCopy(AreaVotes head) {
    AreaVotes result = malloc(sizeof(*AreaVotes));
    if (!result){
        return NULL;
    }
    result->area_id = head->area_id;
    result->next = head->next;
    result->tribe_votes = mapCopy(head->tribe_votes);

    return result;
}


void areaVotesLink(AreaVotes previous, AreaVotes to_link) {
    to_link->next = previous->next;
    previous->next = to_link;
}

AreaVotesResult areaVotesRemove(AreaVotes head, AreaVotes to_remove) {
    if (!head || !to_remove) {
        return AREAVOTES_NULL_ARGUMENT;
    }
    while (head->next != to_remove){
        head = head->next;  // Is the value of the head is changing here ? 
    }
    areaVotesLink(head, to_remove->next);
    mapDestroy(to_remove->tribe_votes);
    free(to_remove);
    return AREAVOTES_SUCCESS;
}

void areaVotesDestroy(AreaVotes head) {
    if(!head) {
        return;
    }
    while(head->next) {
        areaVotesRemove(head,head->next);
    }
    mapDestroy(head->tribe_votes);
    free(head);
}