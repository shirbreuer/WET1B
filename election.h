//
// Created by Ortal on 3/12/2020.
//

#ifndef MTM_ELECTION_H
#define MTM_ELECTION_H
#include "map.h" // change to mtm_map/map.h before submitting

/* Type for defining the Election struct. */
typedef struct election_t* Election;

typedef enum ElectionResult_t {
    ELECTION_OUT_OF_MEMORY,
    ELECTION_NULL_ARGUMENT,
    ELECTION_SUCCESS,
    ELECTION_INVALID_ID,
    ELECTION_TRIBE_ALREADY_EXIST,
    ELECTION_INVALID_NAME,
    ELECTION_AREA_ALREADY_EXIST,
    ELECTION_TRIBE_NOT_EXIST,
    ELECTION_AREA_NOT_EXIST,
    ELECTION_INVALID_VOTES
} ElectionResult;

typedef bool (*AreaConditionFunction) (int);



/**
* electionCreate: Allocates a new empty election struct.
*
* @return
* 	NULL - if allocations failed.
* 	A new Map in case of success.
*/
Election electionCreate();




/**
* electionDestroy: Deallocates an existing election struct. Clears all elements.
*
* @param election - Target election to be deallocated. If election is NULL nothing will be done
*/
void electionDestroy(Election election);



/**
* electionAddTribe: put given tribe id + name into map that contains tribes info.
*
* @param election - Target election with tribes' map inside.
* @param tribe_id - tribe id to insert.
* @param tribe_name - tribe name to insert.
* @return
* 	ELECTION_NULL_ARGUMENT -  if one of the params is NULL.
*   ELECTION_INVALID_ID - if tribe id is negative.
*   ELECTION_TRIBE_ALREADY_EXIST - if map has tribe already.
*   ELECTION_INVALID_NAME - if tribe name contains invalid characters.
*   ELECTION_OUT_OF_MEMORY - if allocations failed.
* 	ELECTION_SUCCESS - otherwise.
*/
ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name);




/**
* electionAddArea: put given area id + name into map that contains areas info.
*
* @param election - Target election with areas' map inside.
* @param area_id - area id to insert.
* @param area_name - area name to insert.
* @return
* 	ELECTION_NULL_ARGUMENT -  if one of the params is NULL.
*   ELECTION_INVALID_ID - if area id is negative.
*   ELECTION_AREA_ALREADY_EXIST - if map has area already.
*   ELECTION_INVALID_NAME - if area name contains invalid characters.
*   ELECTION_OUT_OF_MEMORY - if allocations failed.
* 	ELECTION_SUCCESS - otherwise.
*/
ElectionResult electionAddArea(Election election, int area_id, const char* area_name);


/**
* electionGetTribeName: returns tribe name for given tribe id
*
* @param election - Target election with tribes' map inside.
* @param tribe_id - tribe id to insert.
* @return
* 	NULL -  if one of the params is NULL.
*   tribe name in case of success.
*/
const char* electionGetTribeName (Election election, int tribe_id);

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes);

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes);


/**
* electionSetTribeName: set name for given tribe id
* @param election - Target election with tribes' map inside.
* @param tribe_id - Target tribe id to set
* @param tribe_name - area name to insert.
* @return
* 	ELECTION_NULL_ARGUMENT -  if one of the params is NULL.
*   ELECTION_INVALID_ID - if area id is negative.
*   ELECTION_AREA_ALREADY_EXIST - if map has area already.
*   ELECTION_INVALID_NAME - if area name contains invalid characters.
*   ELECTION_OUT_OF_MEMORY - if allocations failed.
* 	ELECTION_SUCCESS - otherwise.
*/
ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name);

ElectionResult electionRemoveTribe (Election election, int tribe_id);

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area);

Map electionComputeAreasToTribesMapping (Election election);

#endif //MTM_ELECTION_H