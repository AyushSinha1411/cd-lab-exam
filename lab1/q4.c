// Program to insert and search for verbs in a hash table
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct Entry {
    char verb[50];
    struct Entry *next;
} Entry;

Entry *hashTable[TABLE_SIZE];

unsigned int hash(char *str) {
    unsigned int hashValue = 0;
    while (*str) {
        hashValue = (hashValue << 5) + *str++;
    }
    return hashValue % TABLE_SIZE;
}

void insert(char *verb) {
    unsigned int index = hash(verb);
    Entry *newEntry = (Entry *)malloc(sizeof(Entry));
    if (newEntry == NULL) {
        printf("Memory allocation error\n");
        return;
    }

    strcpy(newEntry->verb, verb);
    newEntry->next = hashTable[index];
    hashTable[index] = newEntry;
    printf("Verb '%s' inserted at index %d\n", verb, index);
}

int search(char *verb) {
    unsigned int index = hash(verb);
    Entry *entry = hashTable[index];

    while (entry != NULL) {
        if (strcmp(entry->verb, verb) == 0) {
            return index;
        }
        entry = entry->next;
    }
    return -1;
}

int main() {
    char verb[50];
    int choice;

    // Initialize hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    while (1) {
        printf("Enter 1 to insert verb, 2 to search verb, 3 to exit: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Enter a verb to insert: ");
            scanf("%s", verb);
            insert(verb);
        } else if (choice == 2) {
            printf("Enter a verb to search: ");
            scanf("%s", verb);
            int index = search(verb);
            if (index != -1) {
                printf("Verb '%s' found at index %d\n", verb, index);
            } else {
                printf("Verb '%s' not found in hash table\n", verb);
            }
        } else {
            break;
        }
    }

    return 0;
}