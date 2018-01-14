/*
* CS 261 Data Structures
* Assignment 6
* Name:
* Date:
*/

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int hashFunction1(const char* key)
{
	int r = 0;
	for (int i = 0; key[i] != '\0'; i++)
	{
		r += key[i];
	}
	return r;
}

int hashFunction2(const char* key)
{
	int r = 0;
	for (int i = 0; key[i] != '\0'; i++)
	{
		r += (i + 1) * key[i];
	}
	return r;
}

/**
* Creates a new hash table link with a copy of the key string.
* @param key Key string to copy in the link.
* @param value Value to set in the link.
* @param next Pointer to set as the link's next.
* @return Hash table link allocated on the heap.
*/
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
	HashLink* link = malloc(sizeof(HashLink));
	link->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(link->key, key);
	link->value = value;
	link->next = next;
	return link;
}

/**
* Free the allocated memory for a hash table link created with hashLinkNew.
* @param link
*/
static void hashLinkDelete(HashLink* link)
{
	free(link->key);
	free(link);
}

/**
* Initializes a hash table map, allocating memory for a link pointer table with
* the given number of buckets.
* @param map
* @param capacity The number of table buckets.
*/
void hashMapInit(HashMap* map, int capacity)
{
	map->capacity = capacity;
	map->size = 0;
	map->table = malloc(sizeof(HashLink*) * capacity);
	for (int i = 0; i < capacity; i++)
	{
		map->table[i] = NULL;
	}
}

/**
* Removes all links in the map and frees all allocated memory. You can use
* hashLinkDelete to free the links.
* @param map
*/
void hashMapCleanUp(HashMap* map)
{
	// FIXME: implement
	for (int i = 0; i < map->capacity; i++) {
		hashLinkDelete(map->table[i]);
	}
}

/**
* Creates a hash table map, allocating memory for a link pointer table with
* the given number of buckets.
* @param capacity The number of buckets.
* @return The allocated map.
*/
HashMap* hashMapNew(int capacity)
{
	HashMap* map = malloc(sizeof(HashMap));
	hashMapInit(map, capacity);
	return map;
}

/**
* Removes all links in the map and frees all allocated memory, including the
* map itself.
* @param map
*/
void hashMapDelete(HashMap* map)
{
	hashMapCleanUp(map);
	free(map);
}

/**
* Returns a pointer to the value of the link with the given key. Returns NULL
* if no link with that key is in the table.
*
* Use HASH_FUNCTION(key) and the map's capacity to find the index of the
* correct linked list bucket. Also make sure to search the entire list.
*
* @param map
* @param key
* @return Link value or NULL if no matching link.
*/
int* hashMapGet(HashMap* map, const char* key)
{
	// FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % map->capacity;

	if (hashIndex < 0)
		hashIndex += map->capacity;

	if (hashMapContainsKey(map, key)) {

		HashLink * temp;
		temp = map->table[hashIndex];
		//assert(temp);
		//int * newValue;

		while (temp != 0) {
			if (temp->key == key) {
				return &temp->value;
			}
			else {
				temp = temp->next;
			}
		}
	}

	//free(temp);
	// if none of the values in the list are equal to the key, then throw an error
	// (the test element is not contained in the hash table)
	assert(0);
}

/**
* Resizes the hash table to have a number of buckets equal to the given
* capacity. After allocating the new table, all of the links need to be
* rehashed into it because the capacity has changed.
*
* Remember to free the old table and any old links if you use hashMapPut to
* rehash them.
*
* @param map
* @param capacity The new number of buckets.
*/
void resizeTable(HashMap* map, int capacity)
{
	// FIXME: implement

	int oldCapacity = map->capacity;
	struct HashLink ** oldTable = map->table; //ptr to data

	HashLink *cur, *last;

	hashMapInit(map, 2 * oldCapacity);
	for (int i = 0; i < oldCapacity; i++)
	{
		cur = oldTable[i]; //data from the old table
		while (cur != 0)
		{
			hashMapPut(map, cur->key, cur->value);
			last = cur;
			cur = cur->next;
			free(last); //missing ()'s
		}
	}
	free(oldTable);

}

/*// create a new hash table
	struct HashMap * newHt = (struct HashMap *) malloc(sizeof(HashMap));
	assert(newHt);

	// initialize the new hash table to be twice the size of the old hash table
	hashMapInit(newHt, 2 * (map->capacity));

	// set a temporary link to iterate through the old hash table.
	struct HashLink * temp = NULL;

	// iterate through the old hash table and assign their elements into the new hash table
	for (int i = 0; i < map->capacity; i++)
	{
		temp = map->table[i];

		// If bucket is non-empty, copy its contents into corresponding
		// bucket in new hash table
		while (temp != NULL)
		{
			hashMapPut(newHt, temp->key, temp->value);
				temp = temp->next;
		}
	}

	//Point original hashTable’s table data member at new hashTable’s expanded 
	// table; update ht->size; make necessary deallocations

	free(map->table);
	map->table = newHt->table;
	map->size = newHt->size;
	map->capacity = newHt->capacity;

	free(newHt);*/

/**
* Updates the given key-value pair in the hash table. If a link with the given
* key already exists, this will just update the value. Otherwise, it will
* create a new link with the given key and value and add it to the table
* bucket's linked list. You can use hashLinkNew to create the link.
*
* Use HASH_FUNCTION(key) and the map's capacity to find the index of the
* correct linked list bucket. Also make sure to search the entire list.
*
* @param map
* @param key
* @param value
*/
void hashMapPut(HashMap* map, const char* key, int value)
{
	// FIXME: implement
	
	int hashIndex = HASH_FUNCTION(key) % map->capacity;

	if (hashIndex < 0)
		hashIndex += map->capacity;

	struct HashLink *newLink = hashLinkNew(key, value, map->table[hashIndex]);
	//struct HashLink *newLink = (struct HashLink *) malloc(sizeof(struct HashLink));
	assert(newLink);

	//newLink->value = value;


	//newLink->next = map->table[hashIndex];
	map->table[hashIndex] = newLink; // add to bucket
	map->size++;
	if((map->size / (double)map->capacity) > 8.0) resizeTable(map, map->capacity);

	
	//check to see if the key is located on the map? then update value

	/*struct HashLink * temp = map->table[hashIndex];

	// while not at the end of the linked list
	if (hashMapContainsKey(map, key)) {
		while (temp != NULL) {
			// check to see if the value referenced in temp is the test element
			if (temp->key == key) {
				// if it is, set the value to the new value
				temp->value = value;
			}
			else {
				// if it is not, set temp to the next value in the list
				temp = temp->next;
			}
		}
	}
	
		if (temp == NULL) {
			temp = hashLinkNew(key, value, NULL);
		}
		else {
			while (temp != NULL) {
				temp = temp->next;
			}

			temp = hashLinkNew(key, value, NULL);
		}*/
				

}


/**
* Removes and frees the link with the given key from the table. If no such link
* exists, this does nothing. Remember to search the entire linked list at the
* bucket. You can use hashLinkDelete to free the link.
* @param map
* @param key
*/
void hashMapRemove(HashMap* map, const char* key)
{

	// FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % map->capacity;

	if (hashIndex < 0) {
		hashIndex += map->capacity;
	}

	HashLink *cur, *last;
	assert(hashMapSize(map) != 0);

	if (hashMapContainsKey(map, key)) {
		cur = map->table[hashIndex];
		last = map->table[hashIndex];

		while (cur != 0) {
			if (cur->key == key) {
				if (cur == map->table[hashIndex]) {//special case 1st
					map->table[hashIndex] = cur->next;
				}
				else {
					last->next = cur->next;
					free(cur);
					cur = 0; //make sure jump out of loop.
				}
			}
			else {
				last = cur;
				cur = cur->next;
			}
		}
	}
}

/*struct HashLink * temp = map->table[hashIndex];

while (temp != NULL) {
// check to see if the value referenced in temp is the test element
if (temp->key == key) {
hashLinkDelete(temp);
}
else {
// if it is not, set temp to the next value in the list
temp = temp->next;
}
}*/

/**
* Returns 1 if a link with the given key is in the table and 0 otherwise.
*
* Use HASH_FUNCTION(key) and the map's capacity to find the index of the
* correct linked list bucket. Also make sure to search the entire list.
*
* @param map
* @param key
* @return 1 if the key is found, 0 otherwise.
*/
int hashMapContainsKey(HashMap* map, const char* key)
{
	// FIXME: implement
	// compute hash value to find the correct bucket
	int hashIndex = HASH_FUNCTION(key) % map->capacity;

	if (hashIndex < 0)
		hashIndex += map->capacity;

	
	struct HashLink *temp;
	
	temp = map->table[hashIndex];
	while (temp != 0)
	{
		if (temp->key == key)
			return 1;
		temp = temp->next;
	}
	return 0;
}

	// check to see if testElement is within the hashed bucket

	// set a temporary link to the indexed portion of the table.
	/*struct HashLink * temp = map->table[hashIndex];

	// while not at the end of the linked list
	while (temp != NULL)
	{
		// check to see if the value referenced in temp is the test element
		if (temp->key == key)
		{
			// if it is, return true (the test element is contained in the hash table)
			return 1;
		}
		else
		{
			// if it is not, set temp to the next value in the list
			temp = temp->next;
		}
	}
	// if none of the values in the list are equal to the test element, then return false
	// (the test element is not contained in the hash table)
	return 0;*/




/**
* Returns the number of links in the table.
* @param map
* @return Number of links in the table.
*/
int hashMapSize(HashMap* map)
{
	// FIXME: implement

	return map->size;
}

/**
* Returns the number of buckets in the table.
* @param map
* @return Number of buckets in the table.
*/
int hashMapCapacity(HashMap* map)
{
	// FIXME: implement
	return map->capacity;
}

/**
* Returns the number of table buckets without any links.
* @param map
* @return Number of empty buckets.
*/
int hashMapEmptyBuckets(HashMap* map)
{
	// FIXME: implement
	int empties = 0;
	

	for (int i = 0; i < map->capacity; i++) {
		struct HashLink * temp = map->table[i];
		if (temp == NULL) {
			empties++;
		}
		free(temp);

	}
	return empties;
}

/**
* Returns the ratio of (number of links) / (number of buckets) in the table.
* Remember that the buckets are linked lists, so this ratio tells you nothing
* about the number of empty buckets. Remember also that the load is a floating
* point number, so don't do integer division.
* @param map
* @return Table load.
*/
float hashMapTableLoad(HashMap* map)
{
	// FIXME: implement
	float size = hashMapSize(map);
	float capacity = hashMapCapacity(map);
	float ratio = size/capacity;
	return ratio;
}

/**
* Prints all the links in each of the buckets in the table.
* @param map
*/
void hashMapPrint(HashMap* map)
{
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink* link = map->table[i];
		if (link != NULL)
		{
			printf("\nBucket %i ->", i);
			while (link != NULL)
			{
				printf(" (%s, %d) ->", link->key, link->value);
				link = link->next;
			}
		}
	}
	printf("\n");
}