#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h> 
#include <ctype.h>
#include <stdbool.h>
#include "list.h"
#include "graph.h"
#include "util.h"

int main(int argc, char* argv[]) {
    
  int eflag = 0;
  int cflag = 0;
  char *fvalue = NULL;
  int index;
  int c;

  opterr = 0;

  // accepts input using the getopt function as suggested in the specs
  // checking for -f -e -c flags for file name, execution time
  // and challenge task respectively
  while ((c = getopt (argc, argv, "ecf:")) != -1)
  {
    switch (c)
      {
      case 'e':
          eflag = 1;
          break;
      case 'c':
          cflag = 1;
          break;
      case 'f':
          fvalue = optarg;
          break;
      case '?':
          if (optopt == 'f'){
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          }
          else if (isprint (optopt)){
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          }         
          else{
            fprintf (stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
          }
        return 1;
      default:
          abort ();
      }
  }
  for (index = optind; index < argc; index++){
    printf ("Non-option argument %s\n", argv[index]);
  }
  
  int numProcesses = 0;
  // Opens and reads file stored in the fvalue if the -f flag is mentioned
  FILE* fp = fopen(fvalue, "r");
  assert(fp);

  struct list *problemList = newlist();
  // stores the id associated with the process, locked file and requested file
  int pid, lockedFile,requestedFile; 

  while (fscanf(fp, "%d %d %d", &pid, &lockedFile, &requestedFile) == 3) {

      numProcesses++;
      struct process *process = newProcess(pid, lockedFile, requestedFile);
      appendList(problemList,process);

  }

  fclose(fp); 

  // stores the files and the number of files read
  int files[2*numProcesses];
  int currentFileIndex = 0;

  // stores the process
  int processes[numProcesses];
  // stores the required files for all the processes
  // number of reqfiles will be same as currentProcessIndex
  int reqFiles[numProcesses];
  int currentProcessIndex = 0;

  struct node *current_node = problemList->head;
  while ( current_node != NULL) {

      files[currentFileIndex] = current_node->process->lockedFile;
      processes[currentProcessIndex] = current_node->process->pid;
      currentFileIndex++;
      files[currentFileIndex] = current_node->process->requestedFile;
      reqFiles[currentProcessIndex] = current_node->process->requestedFile;
      currentProcessIndex++;
      currentFileIndex++;
      current_node = current_node->next;

  }

  // counts the distinct files present in the files array
  int numFiles= countDistinctElement(files,currentFileIndex);

  // provides output to the challenge task if the -c switch is mentioned
    // if(cflag == 1){

   if(cflag == 1){

      // stores the visited processes and the number of visited processes
      int n_visitedProcesses = 0;
      int *visitedProcesses = malloc(sizeof(int) * numProcesses);

      // the time unit, where the processes are run
      int timestamp = 0;

      // keeps checking if all the processes have been run
      while(n_visitedProcesses < numProcesses){

        // files and processes that can be run in the current time unit
        // the number of files and processes in the current time unit
        int nfiles = 0, nprocesses = 0;
        int *fileStack = malloc(sizeof(int) * numProcesses * 2);
        int *processStack = malloc(sizeof(int) * numProcesses);

        // iterates through each element of the problem list
        struct node *node = problemList->head;
        while ( node != NULL) {

            // checks if the process is not already visited
            if(isElementPresent(visitedProcesses,n_visitedProcesses, node->process->pid) == -1){

              // checks if the files for the process are available in the current time unit
              if(isElementPresent(fileStack,nfiles,node->process->lockedFile) == -1 &&
              isElementPresent(fileStack,nfiles,node->process->requestedFile) == -1){

                  fileStack[nfiles] = node->process->lockedFile;
                  fileStack[nfiles+1] = node->process->requestedFile;
                  nfiles += 2;
                  processStack[nprocesses] = node->process->pid;
                  nprocesses++;
                }
          } 
          node = node->next;
        }

        // prints the processes that are executed in each time stamp
        for (int i = 0; i < nprocesses; i++)
        {
          printf("%d %d %d,%d\n", timestamp, processStack[i],fileStack[2*i],fileStack[2*i + 1]);
          visitedProcesses[n_visitedProcesses] = processStack[i];
          n_visitedProcesses++;
        }
        timestamp++;

        free(processStack);
        free(fileStack); 
      }

      printf("Simulation time %d\n",timestamp);
      return 0;

    }

    
    //prints the number of Process and Files
    printf("Processes %d\n",numProcesses);
    printf("Files %d\n",numFiles);

    // specifies the execution time if the -e switch is mentioned
    // since each process takes 1 unit time to do work and release the locked file
    // and another one unit of time to request and work on the requested file
    // if k processes request the same file, each of those k processes 
    // will have to wait to get access to that file and then once they get access 
    // perform work.
    // the first process would unlock its file and then work on the requested file
    // and so on
    // therefore the time taken for execution would be 
    if(eflag == 1){
        int maxcount = MostRepeatingElementCount(reqFiles,currentProcessIndex);
        printf("Execution time %d\n", maxcount+1);
        return 0;
    }

    // calculates the max process id used in the input file
    // which is later added to the file id used to make the 
    // vertex of the graph
    int maxProcessId = -1;
    for(int i = 0; i < numProcesses; i++){
      if(processes[i] > maxProcessId){
        maxProcessId = processes[i];
      }
    }

    int vertices[numFiles+numProcesses];
    int vertexIndex = 0;

    // creates the graph using the processes and files as vertices
    // files vertices are stored as (maxProcessId + file number)
    // creates a vertex array to store in the graph
    // vertices are added as process id and file id if they are not
    // already present in the vertices array using the
    // isPresent function
    struct graph *rag = newGraph(numFiles+numProcesses);
    current_node = problemList->head;
    while(current_node != NULL){
      addEdge(rag, (current_node->process->lockedFile)+ maxProcessId,current_node->process->pid);
      addEdge(rag, current_node->process->pid, (current_node->process->requestedFile)+ maxProcessId);
      
      vertices[vertexIndex] = current_node->process->pid;
      vertexIndex++;
      if(isElementPresent(vertices,vertexIndex,(current_node->process->lockedFile)+maxProcessId)==-1){
        vertices[vertexIndex] = (current_node->process->lockedFile)+maxProcessId;
        vertexIndex++;
      }
      if(isElementPresent(vertices,vertexIndex,(current_node->process->requestedFile)+maxProcessId)==-1){
        vertices[vertexIndex] = (current_node->process->requestedFile)+maxProcessId;
        vertexIndex++;
      }

      current_node = current_node->next;

    }

    // stores the vertices array in the graph for use in the
    // deatectDeadlocks function
    rag->vertices = vertices;

    // detects and prints deadlocks by checking if there exists cycles 
    // in the graph by using dfs traversal, printing the smallest 
    // process id which must be terminated
    // if multiple deadlocks are detected it prints them in an
    // ascending orders

    detectDeadlocks(rag);


    // challenge task

    // step 1: create process array and create resource array
    // step 2: iterate through process array and check for combinations that exhaust resource array


    return 0;
}



