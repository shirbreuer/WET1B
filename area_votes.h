#ifndef AREA_VOTES
#define AREA_VOTES

#include "map.h" // change to mtm_map/map.h before submitting


/**
 * Area Votes
 * 
 * Implements a List type.
 * in each node the arguments are - area id (int), tribe_votes (map) and next (pointer to area_votes).
 * 
 * 
 * The following functions are available:
 * 
 * areaVotesCreate - creates a new empty area_votes struct.
 * areaVotesDestroy - delete an existing List of area_votes and frees all resources.
 * areaVotesRemove - delete one area_votes Node and frees all the resources for a given id from the user.
 * areaVotesLink - link a area_votes node between a given node and the next one after.
 * areaVotesGet - return the area_votes node that has the id given by the user.
 * 
 */

typedef struct area_votes_t* AreaVotes;

typedef enum AreaVotesResult_t{
    AREAVOTES_NULL_ARGUMENT,
    AREAVOTES_OUT_OF_MEMORY,
    AREAVOTES_INVALID_ID,
    AREAVOTES_AREA_DOESNT_EXIST,
    AREAVOTES_SUCCESS,
} AreaVotesResult;


AreaVotes areaVotesCreate(int area_id_to_add);

void areaVotesDestroy(AreaVotes head);


AreaVotes areaVotesGet(AreaVotes head, int area_id);


void areaVotesLink(AreaVotes previous, AreaVotes to_link);

AreaVotesResult areaVotesRemove(AreaVotes head, AreaVotes to_remove);


#endif