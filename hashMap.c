/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Steven Owens
 * Date: 11 Mar 2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

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
    for(int i = 0; i < map->capacity; i++)
    {
        HashLink * curr = map->table[i];
        HashLink * prev = curr;

        while(curr != NULL)
        {
            prev = curr;
            curr = curr->next;
            hashLinkDelete(prev);
        }
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
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
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
    int index = HASH_FUNCTION(key);

    HashLink * curr = map->table[index];

    while (curr != NULL)
    {
        if(curr->key == key)
        {
            return &(curr->value);
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{

    HashLink ** newTable = malloc(sizeof(HashLink) * capacity);

    HashLink * curr, ** oldTable;

    oldTable = map->table;
    map->table = newTable;

    for (int i = 0; i < map->capacity; i++)
    {
        curr = oldTable[i];

        while (curr != NULL)
        {
            hashMapPut(map, curr->key, curr->value);
            curr = curr->next;
        }
    }

    //free memory for oldTable after rehashing all old links into new larger capacity table.
    for(int j = 0; j < map->capacity; j++)
    {
        HashLink * temp = map->table[j];
        HashLink * prev = temp;

        while(temp != NULL)
        {
            prev = temp;
            curr = temp->next;
            hashLinkDelete(prev);
        }
    }

    free(oldTable);
    //update capacity of map following rehash of all old links.
    map->capacity = capacity;
    
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    int index = HASH_FUNCTION(key);

    HashLink * curr = map->table[index];

    int * val = hashMapGet(map, key);

    if(val != NULL)
    {
        *val = value;
        return;
    }

    HashLink *newLink = malloc(sizeof(HashLink));
    newLink->key = key;
    newLink->value = value;
    newLink->next = NULL;
    map->size++;

    while (curr != NULL)
    {
        if(curr->next == NULL)
        {
            curr->next = newLink;
        }
        curr = curr->next;
    }
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
    int index = HASH_FUNCTION(key);

    HashLink *curr = map->table[index];

    while (curr != NULL)
    {
        if (curr->key == key)
        {
            hashLinkDelete(curr);
        }
        curr = curr->next;
    }

}

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
    int containsKey = 0;
    int index = HASH_FUNCTION(key);

    if(map->table[index]->key == key)
    {
        containsKey = 1;
    }
    return containsKey;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
   return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
   return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    int counter = 0;

    for(int i = 0; i < map->capacity; i++)
    {
        if(map->table[i] != NULL)
        {
            counter += 1;
        }
    }
    return counter;
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

    return (float) map->size / (float) map->capacity;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    printf("Hash Table Contains: \n");

    for (int i = 0; i < map->capacity; i++)
    {
        HashLink * curr = map->table[i];

        while(curr != NULL)
        {
            printf("Bucket %d - Key: %s Value: %d \n", i, curr->key, curr->value);

            curr = curr->next;
        }
    }
}
