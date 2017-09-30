/***
Defines linked-list functionality for key-value pairs
**/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sllnode.h"

// Create a new linked list. Returns a pointer to the head of the list.
sllnode* create(char *item, double price)
{
    sllnode *new_head = malloc(sizeof(sllnode));
    if (new_head != NULL)
    {
        new_head -> item = item;
        new_head -> price = price;
        new_head -> next = NULL;
    }

    else
        return NULL;

    return new_head;
    /* note that it's a good idea to store the pointer to the head of the list in
    a global variable! */
}

// Search through a singly-linked list in O(n) time for item
// Append to item and return true if found, else false
bool append(sllnode *head, char *item, double price)
{
    // Initialize a pointer
    sllnode *trav = head;
    
    // Iterate through the linked list
    while (trav != NULL)
    {
        if (strcmp(trav -> item, item) == 0)
		{
        	trav -> price += price;
			return true;
		}        
		else
            trav = trav -> next;
    }
    return false;
}

// Insert a new node into the linked list. Returns a pointer to the new head.
sllnode* insert(sllnode *head, char *item, double price)
{
    sllnode *new_node = malloc(sizeof(sllnode));

    if (new_node == NULL)
        return NULL;
    
    new_node -> item = item;
    new_node -> price = price;
    new_node -> next = head;
    
    head = new_node;

    return head;
}

// Deletes the entire node from memory, recursively from back to front
void destroy(sllnode* head)
{
    if (head == NULL)
        return;
    else
        destroy(head -> next);

    free(head);
}




