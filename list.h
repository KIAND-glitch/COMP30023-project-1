/*
list.h

Visible structs and functions for linked lists.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/* the process. */
struct process{
    int pid;
    int lockedFile;
    int requestedFile;
};

/* The node which stores the process and the pointer to the next node. */
struct node {
  struct process *process;
  struct node  *next;
};

/* The linked list. */
struct list{
    struct node *head;
    struct node *tail;
};

/* Get a new process. */
struct process *newProcess(int pid, int lockedFile, int requestedFile);

/* Get a new empty list. */
struct list *newlist();

/* Add an item to the tail of the list. Returns the new list. */
struct list *appendList(struct list *list, struct process *process);

#endif


