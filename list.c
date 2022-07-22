/*
list.c

Implementations for helper functions for linked list construction and
manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Get a new process. */
struct process *newProcess(int pid, int lockedFile, int requestedFile){
  struct process *process = (struct process*) malloc(sizeof(struct process));
  assert(process);
  process->pid = pid;
  process->lockedFile = lockedFile;
  process->requestedFile = requestedFile;
  return process;
}

/* Get a new empty list. */
struct list *newlist(){
  struct list *list = (struct list *) malloc(sizeof(struct list));
  assert(list);
  list->head = NULL;
  list->tail = NULL;
  return list;
}

/* Add an item to the tail of the list. Returns the new list. */
struct list *appendList(struct list *list, struct process *process){
  struct node *node = (struct node *) malloc(sizeof(struct node));
  node->process = process;
  node->next = NULL;
  assert(node);
  if(list->head == NULL){
      list->head = node;
      list->tail = node;
  }
  else{
    list->tail->next = node;
    list->tail = node;
  }
  return list;
}



