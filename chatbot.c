#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <strings.h>
#include "chatbot.h"

struct entry_s {
  char *key;
  char *value;
  struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
  int size;
  struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

  hashtable_t *hashtable = NULL;
  int i;

  if( size < 1 ) return NULL;

  /* Allocate the table itself. */
  if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
    return NULL;
  }

  /* Allocate pointers to the head nodes. */
  if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
    return NULL;
  }
  for( i = 0; i < size; i++ ) {
    hashtable->table[i] = NULL;
  }

  hashtable->size = size;

  return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

  unsigned long int hashval = 0;
  unsigned long i = 0;

  /* Convert our string to an integer */
  while( hashval < ULONG_MAX && i < strlen( key ) ) {
    hashval = hashval << 8;
    hashval += key[ i ];
    i++;
  }

  return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, char *value ) {
  entry_t *newpair;

  if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
    return NULL;
  }

  if( ( newpair->key = strdup( key ) ) == NULL ) {
    return NULL;
  }

  if( ( newpair->value = strdup( value ) ) == NULL ) {
    return NULL;
  }

  newpair->next = NULL;

  return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, char *value ) {
  int bin = 0;
  entry_t *newpair = NULL;
  entry_t *next = NULL;
  entry_t *last = NULL;

  bin = ht_hash( hashtable, key );

  next = hashtable->table[ bin ];

  while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
    last = next;
    next = next->next;
  }

  /* There's already a pair.  Let's replace that string. */
  if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

    free( next->value );
    next->value = strdup( value );

  /* Nope, could't find it.  Time to grow a pair. */
  } else {
    newpair = ht_newpair( key, value );

    /* We're at the start of the linked list in this bin. */
    if( next == hashtable->table[ bin ] ) {
      newpair->next = next;
      hashtable->table[ bin ] = newpair;

    /* We're at the end of the linked list in this bin. */
    } else if ( next == NULL ) {
      last->next = newpair;

    /* We're in the middle of the list. */
    } else  {
      newpair->next = next;
      last->next = newpair;
    }
  }
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get( hashtable_t *hashtable, char *key ) {
  int bin = 0;
  entry_t *pair;

  bin = ht_hash( hashtable, key );

  /* Step through the bin, looking for our value. */
  pair = hashtable->table[ bin ];
  while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
    pair = pair->next;
  }

  /* Did we actually find anything? */
  if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
    return NULL;

  } else {
    return pair->value;
  }

}

int main(void) {

  char line[LINELENGTH];
  char *word;
  printf("$ Chatbot v1.0.0!\n");

  hashtable_t *hashtable = ht_create(65536);
  ht_set(hashtable, "hi", "hello");
  ht_set(hashtable, "hey", "hello");
  ht_set(hashtable, "hello", "hello");
  ht_set(hashtable, "sup", "hello");
  ht_set(hashtable, "hear", "What i heard is right");
  ht_set(hashtable, "python", "Nah.. I prefer c over python dude :)");
  ht_set(hashtable, "how'slife??", "Its treating me well what about u??");
  ht_set(hashtable, "light", "I like light");
  ht_set(hashtable, "c", "too cool a language to study");
  ht_set(hashtable, "apple", "ohh! that's a red coloured juicy fruit!!");
  ht_set(hashtable, "watermelon", "a sweet fruit with lots of fibre present!");
  ht_set(hashtable, "banana", "a yellow coloured fruit who's peel can make u slip and fall down!! ");
  ht_set(hashtable, "What", "It is clear, ain't it?");
  ht_set(hashtable, "urname?", "chatbot");
  ht_set(hashtable, "bye", "see ya! have a nice day!!");
  ht_set(hashtable,"bangaluru","It is a IT hub city");
  ht_set(hashtable,"banglore","It is a IT hub city");
  ht_set(hashtable,"delhi","Capital of India");
  ht_set(hashtable,"mumbai","bollywood film city");
  ht_set(hashtable,"jaipur","pink city");
  ht_set(hashtable,"agra","Tajmahal");
  ht_set(hashtable,"goa","city of beaches");
  ht_set(hashtable,"surat","Diamond city");
  ht_set(hashtable,"chennai","capital of Tamil Nadu");
  ht_set(hashtable,"kolkata","capital of west bengal");
  ht_set(hashtable,"i'llkillyou","i'm sorry to hear that :(");



  while(1) {
    printf("\n$ (user) ");
    fgets(line, LINELENGTH, stdin);
    if (strlen(line) <= 1) break; /*exit program*/
    word = strtok(line, SEPCHARS); /*Find first word */

    while (word != NULL) {
      if (strncasecmp(word, "exit", 150) == 0) {
        exit(0);
      }
      if (strncasecmp(word, "die", 150) == 0) {
        exit(0);
      }
      if (strncasecmp(word, "fuck", 150) == 0) {
        exit(0);
      }
      // Some responses based on the keywords
      if (ht_get(hashtable, word) != NULL) {
        printf("\n$ (chatbot) %s\n", ht_get(hashtable, word));
      } else {
        printf("\n$ (chatbot) %s\n", "Sorry, I don't know what to say about that" );
      }
      word = strtok(NULL, SEPCHARS);
    }
  }
}
