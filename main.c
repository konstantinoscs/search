#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "search.h"
#include "trie.h"
#include "utilities.h"

int main(int argc, char** argv){
  char *doc = NULL, **documents = NULL;
  char *cmd = malloc(8);  //since there are only 4 commands make greedy malloc
  char **queries = NULL;  //all the search queries
  int k=0;
  int docsize=0;  //no of documents
  int queriesNo = 0, target_doc =-1;  //no of queries and docno for /df
  float k1=1.2, b=0.75;
  TrieNode *trie = NULL;

  parseArguments(argc, argv, &doc, &k);
  parseDocument(doc, &documents, &docsize);
  printf("Parsed documents\n");
  //printDocuments(documents, docsize, "");
  int *doc_length = malloc(docsize*sizeof(int));  //lengths of documents
  double avgdl=(double) fill_lengths(documents, docsize, doc_length)/docsize;
  clock_t t = clock();
  trie = makeTrie(documents, docsize);
  t = clock() -t;
  printf("Made trie in:%lf\n", (double)t/CLOCKS_PER_SEC);
  //print_trie(trie);
  while(1){
    scanf("%s", cmd);
    if(!strcmp(cmd, "/search")){
      if(!readQueries(&queries, &queriesNo)){
        fprintf(stderr, "No queries given!\n");
        continue;
      }
      search(trie, documents, doc_length, docsize, queries, queriesNo, k1, b,
        avgdl, k);
      deleteQueries(&queries, queriesNo);
    }
    else if(!strcmp(cmd, "/df")){
      if(!readQueries(&queries, &queriesNo)){
        document_frequency(trie);
        continue;
      }
      else if(queriesNo!=1){    //maybe support multiple words
        fprintf(stderr, "Wrong queries given!\n");
      }
      else{
        int df = document_appearances(trie, queries[0]);
        printf("%s %d\n", queries[0], df);
      }
      deleteQueries(&queries, queriesNo);
    }
    else if(!strcmp(cmd, "/tf")){
      if(!readQueries(&queries, &queriesNo)){
        fprintf(stderr, "No queries given!\n");
        continue;
      }
      else if(queriesNo!=2){
        fprintf(stderr, "Wrong queries given!\n");
      }
      else{
        int termf = term_frequency(trie, queries[1], atoi(queries[0]));
        printf("%d %s %d\n", atoi(queries[0]), queries[1], termf);
      }
      deleteQueries(&queries, queriesNo);
    }
    else if(!strcmp(cmd, "/exit")){
      break;
    }
    else{
      fprintf(stderr, "Unknown command, try again\n");
    }
    printf("\n");
  }

  //cleanup
  delete_trie(trie);
  free(trie);
  free(doc_length);
  free(doc);
  free(cmd);
  for(int i=0; i<docsize; i++)
    free(documents[i]);
  free(documents);
}
