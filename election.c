#include "election.h"
#include <stdlib.h>
#include <stdio.h>

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
    if (mapPut(Map map, const char* key, const char* data) ==)
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {
       
}