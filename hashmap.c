#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  long keyhash = hash(key,map->capacity);
  while(1){
  if(map->buckets[keyhash]==NULL || map->buckets[keyhash]->key==NULL){
    Pair* newpair = createPair(key,value);
    map->buckets[keyhash]= newpair;
    map->size +=1;
    map->current = keyhash;
    break;
  }
  keyhash++;
  keyhash = keyhash%map->capacity;
    }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** oldBuckets = map->buckets;
  int oldCapacity = map->capacity;
  map->capacity= oldCapacity*2;
  map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  map->size=0;
  for(int i=0,i <= oldCapacity ,i++){
    if (oldBuckets[i]  && oldBuckets[i]->key){
    insertMap(map, oldBuckets[i]->key,oldBuckets[i]->value);
    }
  }
}



HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  map->capacity = capacity;
  map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
  Pair* eliminarpair = searchMap(map,key);
  if (eliminarpair !=NULL){
    eliminarpair->key = NULL;
    map->size--;
  }

}

Pair * searchMap(HashMap * map,  char * key) {   
  long keyhash = hash(key,map->capacity);
  if (map->buckets[keyhash]==NULL){
    return NULL;
  }
  int count=0;
  while(1){
    if(map->buckets[keyhash] != NULL && is_equal(map->buckets[keyhash]->key,key)){
      map->current= keyhash;
      return map->buckets[keyhash];
    }
  keyhash++;
  keyhash = keyhash%map->capacity;
  count++;
  if (count>map->capacity) break;}

  return NULL;
}

Pair * firstMap(HashMap * map) {
    map->current = 0;
    while(1){
      if(map->buckets[map->current] && map->buckets[map->current]->key){
        return map->buckets[map->current];
      }
      map->current++;
      if (map->current >= map->capacity) return NULL;
      }
}

Pair * nextMap(HashMap * map) {
  map->current++;
  while(1){
    if (map->current>=map->capacity) return NULL;
    if(map->buckets[map->current] && map->buckets[map->current]->key){
        return map->buckets[map->current];
      }
    map->current++;
    }
}
