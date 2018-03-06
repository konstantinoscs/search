#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "sort.h"
#include "trie.h"

//simple function to count the length of a document
int compute_length(char*document){
  int words =0, pos = 0;
  while(document[pos]!='\0'){
    if(document[pos] == ' ')
      words++;
    pos++;
  }
  return words++;
}

//computes all lengths and returns their sum
int fill_lengths(char**documents, int docsize, int* doc_length){
  int sum =0;
  for(int i=0; i<docsize; i++){
    doc_length[i] = compute_length(documents[i]);
    sum+= doc_length[i];
  }
  return sum;
}

double get_idf(TrieNode *trie, int docsize, char *word){
  int nqi = document_appearances(trie, word);
  return log((docsize-nqi+0.5)/(nqi+0.5));
}

//compute score
double get_score(TrieNode *trie, char *document, int doc_id, int *doc_length,
  int docsize, char**queries, int queriesNo, float k1, float b, double avgdl){

  int f=0;
  double idf=0, score=0;

  for(int i=0; i<queriesNo; i++){
    idf = get_idf(trie, docsize, queries[i]);  //get idf for this query
    f = term_frequency(trie, queries[i], doc_id); //get term frequency
    //score is computed with the given formula
    score += idf*f*(k1+1)/(f+k1*(1-b+b*doc_length[doc_id]/avgdl));
  }
  return score;
}

void eliminate_duplicates(int **distinct_doc, int *distinctNo){
  int truei =0;  //true index
  for(int i=0; i<*distinctNo; i++){
    if((*distinct_doc)[i] != (*distinct_doc)[truei])
      swap(&(*distinct_doc)[++truei], &(*distinct_doc)[i]);
  }
  *distinct_doc = realloc(*distinct_doc, (++truei)*sizeof(int));
  *distinctNo = truei;
}

void make_docid_list(TrieNode *trie, int **distinct_doc, int *distinctNo,
  char**queries, int queriesNo){

  int **documentId = malloc(queriesNo*sizeof(int*));
  int *idSize = malloc(queriesNo*sizeof(int));
  int totalSize = 0;

  //get all docids for every query
  for(int i=0; i<queriesNo; i++){
    get_word_docid(trie, queries[i], &documentId[i], &idSize[i]);
    totalSize += idSize[i];
    //  printf("for query %d got ids, size: %d ", i, idSize[i]);
    //  for(int j=0; j<idSize[i]; j++)
    //    printf("%d ", documentId[i][j]);
    //  printf("\n");
  }
  if(totalSize != 0){
    *distinct_doc = malloc(totalSize*sizeof(int));
    int doci = 0;   //docindex
    //now merge into a single array
    for(int i=0; i<queriesNo; i++)
      for(int j=0; j<idSize[i]; j++)
        (*distinct_doc)[doci++] = documentId[i][j];

    quicksort(*distinct_doc, 0, totalSize-1);
    eliminate_duplicates(distinct_doc, &totalSize);
  }
  (*distinctNo) = totalSize;

  for(int i=0; i<queriesNo; i++)
    if(documentId[i] != NULL)
      free(documentId[i]);
  free(idSize);
  free(documentId);
}

int search(TrieNode *trie, char**documents, int *doc_length, int docsize,
  char**queries, int queriesNo, float k1, float b, double avgdl, int k){

  int *distinct_doc = NULL, distinctNo = 0; //number of distinct documents
  ScoreElem *doc_scores = NULL;
  make_docid_list(trie, &distinct_doc, &distinctNo, queries, queriesNo);
  //if(distinctNo == 0) //all queries were not present in the text
    //return 0;
  doc_scores = malloc(distinctNo*sizeof(ScoreElem));
  int width = 3, precision = 5;

  for(int i=0; i<distinctNo; i++){
    doc_scores[i].doc = distinct_doc[i];
    doc_scores[i].score = get_score(trie, documents[distinct_doc[i]],
      distinct_doc[i], doc_length, docsize, queries, queriesNo,
      k1, b, avgdl);
    //printf("%*d.(%*d)[%*lf] %s\n", width, i, width, doc_scores[i].doc, precision, doc_scores[i].score, documents[doc_scores[i].doc]);
  }
  scoreQuicksort(doc_scores, 0, distinctNo-1);
  if(distinctNo < k)
    k = distinctNo;
  for(int i=0; i<k; i++){
    printf("%*d.(%*d)[%*lf] %s\n", width, i+1, width, doc_scores[i].doc, precision, doc_scores[i].score, documents[doc_scores[i].doc]);
    //printf("%d after score: %lf\n", doc_scores[i].doc, doc_scores[i].score);
  }
  free(doc_scores);
  free(distinct_doc);
}
