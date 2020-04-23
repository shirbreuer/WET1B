#ifndef AREA_VOTES
#define AREA_VOTES

#include "map.h" // change to mtm_map/map.h before submitting


/**
 * Area Votes
 * 
 * Implements a List type.
 * in each area_votes (node) the arguments are - area id (int), tribe_votes (map) and next (pointer to area_votes).
 * 
 * 
 * The following functions are available:
 * 
 * areaVotesCreate - creates a new area_votes struct containing the first area.
 * areaVotesDestroy - delete an existing List of area_votes and frees all resources.
 * areaVotesRemove - delete one area_votes Node and frees all the resources for a given id from the user.
 * areaVotesLink - link a area_votes node between a given node and the next one after.
 * areaVotesGet - return the area_votes node that has the id given by the user.
 * 
 */

/* Type for defining the area_votes struct*/
typedef struct area_votes_t* AreaVotes;

/* Type used for returning error codes from the area_votes functions */
typedef enum AreaVotesResult_t{
    AREAVOTES_NULL_ARGUMENT,
    AREAVOTES_OUT_OF_MEMORY,
    AREAVOTES_INVALID_ID,
    AREAVOTES_AREA_DOESNT_EXIST,
    AREAVOTES_SUCCESS,
} AreaVotesResult;

/**
 * areaVotesCreate: Allocates a new empty area_votes.
 * 
 * @param area_id_to_add - The id of the area we want to add a area_votes of it
 * @return
 * NULL - if allocations failed.
 * A new area_votes in case of success.
*/

AreaVotes areaVotesCreate(int area_id_to_add);

/**
 * areaVotesDestroy: Deallocates an existing area_votes list. remove all the content from each area_votes.
 *  
 * @param head - Pointer to the first area_votes in the list (the last one inserted). 
 * If the pointer is NULL nothing will happend.
*/

void areaVotesDestroy(AreaVotes head);

/**
 * areaVotesGet: return the area_votes node that has the id given by the user.
 * 
 * @param area_id - The id of the area we want to get from the list.
 * @param head - Pointer to the first area_votes in the list (the last one inserted).
 * @return
 * NULL - if there is no area_votes with this id.
 * The pointer for the area_votes with this id.
*/

AreaVotes areaVotesGet(AreaVotes head, int area_id);

/**
 * areaVotesList: link a area_votes between a given area_votes and the next one after.
 *  
 * @param previous - The area_votes we want to be before the area_votes we want to connect.
 * @param to_link - The area_votes we want to link after the previous.
*/

void areaVotesLink(AreaVotes previous, AreaVotes to_link);



/**
 * areaVotesRemove: remove the current area_votes.
 *  
 * 
 * @return
 * AREA_VOTES_NULL_ARGUMENT - if input was NULL.
 * AREAVOTES_SUCCESS - if remove was successful
*/
AreaVotesResult areaVotesRemove(AreaVotes head, AreaVotes to_remove);


#endif