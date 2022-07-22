/*
graph.h

Visible structs and functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  int *vertices;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
};

/* checks if an element is present in the array and returns its index, else returns -1 */
// int isPresent(int arr[], int size, int el);

struct graph *newGraph(int numVertices);

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end);

/* returns the number of neighbours of a vertex v */
int numNeighbours(struct graph *g,int v);

/* gets the neighbours of a vertex v and stores in *neighbours */
void getNeighbours(struct graph *g, int v, int *neighbours);

/* Frees all memory used by graph. */
void freeGraph(struct graph *g);

/* 
 Recursive dfsExplore function which explores from current vertex in the graph.
 recStack,visited and deadlocks are arrays of length numVertices, n_visited,order
 and n_deadlocks are pointers to the number of nodes which have already been visited,
 number of nodes in the recStack and number of deadlocks detected
*/
void dfsExplore(struct graph *g,int curr_ver, int *visited, int *n_visited,int *recStack, int *order, int deadlocks[],int *n_deadlocks);

/* 
 Runs a depth first search on the given graph, to check for cycles by recusively calling
 the dfsExplore function for each vertex of the graph.
 If deadlocks are detected they are stored in the deadlocks array, with n_deadlocks being
 the number of deadlocks, and then they are printed for the lowest process which triggered
 it, by using qsort with the cmpfunction
*/
void detectDeadlocks(struct graph *graph);

#endif
