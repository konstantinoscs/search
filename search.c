#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

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

void fill_carrets(char* carrets, char *document, char **queries, int queriesNo){
  int ci=0, len=0;   //carrets index
  char * subs = document, ch = ' ';
  for(int i=0; i<strlen(document); i++)
    carrets[i] = ' ';
  while(ch!='\0'){
    len = 0;
    while((ch = subs[len]) != ' ' && ch !='\t' && ch != '\0')
      len++;
    subs[len] = '\0';
    for(int i=0; i<queriesNo; i++){
      if(!strcmp(queries[i], subs)){
        for(int j=0; j<len; j++)
          carrets[ci+j] = '^';
        break;
      }
    }
    ci+=len;
    subs[len] = ch;
    subs+=len;
    if(ch != '\0')           //if it's the end of the document don't search
      while(isspace(subs[0])){   //set substring to new word
        subs++;
        ci++;
      }
  }
}

void print_result(char *document, char *carrets, int ws_col, int offset){
  int di=0, ci=0, len = strlen(document);
  int lines = len/(ws_col-offset), chars = ws_col -offset;
  if(!lines || len%chars) lines++;
  for(int j=0; j<lines; j++){
    if(j)
      for(int i=0; i<offset; i++)
        printf(" ");
    for(int i=0; i<chars; i++){
      if(di==len)
        break;
      printf("%c", document[di++]);
    }
    printf("\n");
    for(int i=0; i<offset; i++)
      printf(" ");
    for(int i=0; i<chars; i++){
      if(ci==len)
        break;
      printf("%c", carrets[ci++]);
    }
    printf("\n");
  }
}

int find_width(int number){
  int digits = 1;
  while (number/=10) digits++;
  return digits;
}

int search(TrieNode *trie, char**documents, int *doc_length, int docsize,
  char**queries, int queriesNo, float k1, float b, double avgdl, int k){

  int *distinct_doc = NULL, distinctNo = 0; //number of distinct documents
  char * carrets = NULL;  //the carrets under the word
  ScoreElem *doc_scores = NULL;
  make_docid_list(trie, &distinct_doc, &distinctNo, queries, queriesNo);
  int docwidth = 0, scorewidth = 0;
  int maxid = 0;
  int idwidth = 2, precision = 5;
  int offset = 0;
  doc_scores = malloc(distinctNo*sizeof(ScoreElem));
  struct winsize w;
  ioctl(fileno(stdout), TIOCGWINSZ, &w);
  //w.ws_col;

  for(int i=0; i<distinctNo; i++){
    doc_scores[i].doc = distinct_doc[i];
    doc_scores[i].score = get_score(trie, documents[distinct_doc[i]],
      distinct_doc[i], doc_length, docsize, queries, queriesNo,
      k1, b, avgdl);
    if(doc_scores[i].doc>maxid)   //find max id to sepcify width
      maxid = doc_scores[i].doc;
    docwidth = find_width(maxid);
  }
  scoreQuicksort(doc_scores, 0, distinctNo-1);
  if(distinctNo){      //determine the width of the score
    scorewidth = find_width(doc_scores[0].score);
    if(doc_scores[distinctNo].score < 0)
      scorewidth++;                 //+1 for the - sign
  }
  offset = idwidth + docwidth + scorewidth + precision + 7; //set correct offset
  if(distinctNo < k)
    k = distinctNo;
  for(int i=0; i<k; i++){
    // printf("%s\n", documents[doc_scores[i].doc]);
    // printf("len:%d\n", strlen(documents[doc_scores[i].doc]));
    printf("%*d.(%*d)[%*.*lf] ", idwidth, i+1, docwidth, doc_scores[i].doc,
      scorewidth+precision+1, precision, doc_scores[i].score);
    carrets = realloc(carrets, strlen(documents[doc_scores[i].doc]));
    fill_carrets(carrets, documents[doc_scores[i].doc], queries, queriesNo);
    //printf("%s\n%s\n", documents[doc_scores[i].doc], carrets);
    print_result(documents[doc_scores[i].doc], carrets, w.ws_col, offset);
    //%s\n", , documents[doc_scores[i].doc]);
    //printf("%d after score: %lf\n", doc_scores[i].doc, doc_scores[i].score);
  }
  free(carrets);
  free(doc_scores);
  free(distinct_doc);
}
