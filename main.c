//
// Created by Steve Owens on 3/11/20.
//
#include <stdlib.h>
#include <stdio.h>
#include "hashMap.h"

int main() {

    printf("Hello World");

    HashMap *map = hashMapNew(5);
    printf("\nEmpty Buckets: %d", hashMapEmptyBuckets(map));

    hashMapPut(map, "start", 10);
    hashMapPut(map, "second", 20);
    hashMapPut(map, "third", 30);
    hashMapPut(map, "middle", 32);
    hashMapPut(map, "middle", 35);
    hashMapPut(map, "end", 80);
    printf("\nFirst Set of keys & values\n");
    hashMapPrint(map);
    printf("\nEmpty Buckets: %d", hashMapEmptyBuckets(map));
    hashMapPut(map, "fourth", 40);
    hashMapPut(map, "fifth", 70);
    hashMapRemove(map, "middle");
    printf("\nSecond Set of keys & values\n");
    hashMapPrint(map);
    printf("\nEmpty Buckets: %d", hashMapEmptyBuckets(map));
    hashMapDelete(map);



    return 0;
}