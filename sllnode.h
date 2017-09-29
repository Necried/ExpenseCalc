/***
Defines linked-list functionality for key-value pairs
**/

#ifndef SLLNODE_H
#define SLLNODE_H

#include <stdbool.h>

// Define a singly-linked list struct
typedef struct sllnode
{
    char *item;
    double price;
    struct sllnode *next;
}
sllnode;

// Create a new linked list. Returns a pointer to the head of the list.
sllnode* create(char *item, double price);

// Search through a singly-linked list for item (needs rework to get a specific pointer returner)
bool append( sllnode *head, char *item, double price);

// Insert a new node into the linked list. Returns a pointer to the new head.
sllnode *insert(sllnode *head, char *item, double price);

// Delete the entire list
void destroy(sllnode *head);

#endif // SLLNODE_H
