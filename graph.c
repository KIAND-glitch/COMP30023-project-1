/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "graph.h"
#include "util.h"

#define INITIALEDGES 32

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* returns the number of neighbours of a vertex v */
int numNeighbours(struct graph *g,int v){
    int n = 0;
    for(int i=0;i<g->numEdges;i++){
        if(g->edgeList[i]->start == v){
            n++;
        }
    }
    return n;
}

/* gets the neighbours of a vertex v and stores in *neighbours */
void getNeighbours(struct graph *g, int v, int *neighbours){
    int j = 0;
    for(int i=0;i<g->numEdges;i++){
        if(g->edgeList[i]->start == v){
            neighbours[j] = g->edgeList[i]->end;
            j++;
        }
    }
    return;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

/* cmpfunc used for qsort, which compares two integers to sort in ascending order */
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

/* Recursive dfsExplore function which explores from current vertex in the graph.
 recStack,visited and deadlocks are arrays of length numVertices, n_visited,order
 and n_deadlocks are pointers to the number of nodes which have already been visited,
 number of nodes in the recStack and number of deadlocks detected */
void dfsExplore(struct graph *g,int curr_ver, int *visited, int *n_visited,int *recStack, 
                  int *order, int deadlocks[],int *n_deadlocks)
{         

    // curr_ver is already part of the current recStack, i.e there is a cycle
    // min stores the index of the vertex in the rec stack, where the cycle
    // starts
    if (isElementPresent(recStack,*order,curr_ver) != -1)
    {
      int index = isElementPresent(recStack,*order,curr_ver);
      int min = curr_ver;
      for(int i = index; i < *order; i++){
        if(min > recStack[i]){min = recStack[i];}
      }
      if(isElementPresent(deadlocks,*n_deadlocks,min)==-1){
            deadlocks[*n_deadlocks] = min;
            (*n_deadlocks)++;
      }
      return;
    }    

    // add curr_ver to the visited array
    visited[*n_visited] = curr_ver;
    (*n_visited)++;

    // add curr_ver to the recStack
    recStack[*order] = curr_ver;
    (*order)++;

    int nNeighbours = numNeighbours(g,curr_ver);
    int *neighbours = malloc(sizeof(int) * nNeighbours);

    getNeighbours(g,curr_ver,neighbours);

    // traverses all the neighbours of the curr_ver to detect for cycles
    int v;
    for (int i = 0; i < nNeighbours; i++) {
        v = neighbours[i];
        dfsExplore( g, v, visited, n_visited, recStack,order,deadlocks,n_deadlocks);
    }        

    return ;
}


/* Runs a depth first search on the given graph, to check for cycles by recusively calling
 the dfsExplore function for each vertex of the graph.
 If deadlocks are detected they are stored in the deadlocks array, with n_deadlocks being
 the number of deadlocks, and then they are printed for the lowest process which triggered
 it, by using qsort with the cmpfunction */
void detectDeadlocks(struct graph *graph) {
  
  int n = graph->numVertices;
  int n_visited = 0;

  int *visited = malloc(sizeof(int) * n);
  assert(visited);

  int n_deadlocks = 0;
  int deadlocks[n];

  int curr_ver;

  for(int i=0;i < n; i++){
    deadlocks[i] = -1;
  }

  for (int u = 0; u < n; u++) {

    curr_ver = graph->vertices[u];
    if(isElementPresent(visited,n_visited,curr_ver) == -1){

        int *recstack = malloc(sizeof(int) * n);
        int order = 0;
        assert(recstack);

        dfsExplore(graph,curr_ver, visited, &n_visited ,recstack, &order, deadlocks, &n_deadlocks);

    }
  }

  // prints output if deadlocks have been detected
  if(n_deadlocks > 0){

    qsort(deadlocks, n_deadlocks, sizeof(int), cmpfunc);
    printf("Deadlock detected\n");
    printf("Terminate");

    // prints the processes to terminate
    for(int i = 0; i < n_deadlocks; i++){
      printf(" %d",deadlocks[i]);
    }

    printf("\n");
    return;
  }

  printf("No deadlocks\n");
  return;
}

