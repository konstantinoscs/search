#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

int parseArguments(int argc, char ** argv, char** doc, int *k){
  int i=1;
  if(argc == 3 ){
    *k = 10;  //default value
  }
  else if(argc !=5 ){
    fprintf(stderr, "Wrong number of arguments given\n");
    exit(-1);
  }
  while(i<argc){
    if(!strcmp(argv[i], "-i")){ //parse docfile
      *doc = malloc(strlen(argv[++i])+1);
      strcpy(*doc, argv[i]);
    }
    else if(!strcmp(argv[i], "-k")){  //parse k
      *k = atoi(argv[++i]);
    }
    i++;  //move to the next arg one incremation has already been done
  }
  printf("k is %d\n", *k);
  return 1;
}

//parseDocument reads all the documents in the file specified by doc
int parseDocument(char* doc, char*** documents, int *docsize){
  int docm = 8; //arbitary start from 8 words
  int wordm = 2, wordc=0; //start from a word with 2 characters
  int pid = -1, id =0; //previous id and id
  int ch;
  FILE * docf = fopen(doc, "r");

  *documents = malloc(docm*sizeof(char*));

  while((ch=getc(docf)) != EOF){
    ungetc(ch, docf);
    fscanf(docf, "%d", &id);
    if(id != ++pid){
      fprintf(stderr, "ids in docfile are not in the correct order\n");
      return -1;
    }
    if(id == docm){     //allocate space for more documents
      docm *= 2;
      *documents = realloc(*documents, docm*sizeof(char*));
    }
    (*documents)[id] = malloc(wordm); //allocate memory for the word

     //get the first space after the document number
    while(isspace(ch = getc(docf)));
    ungetc(ch, docf);
    while((ch=getc(docf)) != '\n'){
      //while(isspace(ch)) ch=getc(docf);
      if(isspace(ch)){
        while(isspace(ch) && ch!='\n') ch=getc(docf);
        if(ch == '\n') break;
        ungetc(ch, docf);
        ch = ' ';
      }
      if(wordc+1 == wordm){  //realloc condition -- save space for '\0'
        wordm *= 2;
        (*documents)[id] = realloc((*documents)[id], wordm);
      }
      (*documents)[id][wordc++] = ch; //save character in documents
    } //document is saved exactly as read --including whitespace

    (*documents)[id][wordc] = '\0';
    (*documents)[id] = realloc((*documents)[id], wordc+1); //shrink to fit
    // printf("%s\n", (*documents)[id]);
    // fflush(stdout);
    wordm = 2;  //re-initialize for next document
    wordc = 0;
  }
  *documents = realloc(*documents, (id+1)*sizeof(char*)); //shrink to fit
  *docsize = id+1;
  fclose(docf);
}

void printDocuments(char **documents, int docsize, char* desc){
  printf("%s:\n", desc);
  for(int i=0; i<docsize; i++)
    printf("%d %sE\n", i, documents[i]);
}

int readQueries(char ***queries, int *queriesNo){
  char ch = ' ';
  int wordm=0, wordc=0;   //current query memory and query counter
  int queriesc = 0;

  if(ch =getchar() == '\n')
    return 0;
  *queriesNo = 2;
  *queries = malloc((*queriesNo)*sizeof(char*));
  while(ch!='\n'){
    wordm = 2;    //start from 1 character (plus '\0')
    wordc = 0;
    if(queriesc == 10){
      while((ch=getchar()) !='\n') continue;
      break;
    }
    if(queriesc == *queriesNo){
      (*queriesNo) *=2;
      *queries = realloc(*queries, (*queriesNo)*sizeof(char*));
    }
    (*queries)[queriesc] = malloc(wordm);
    while((ch=getchar())!= ' ' && ch!='\n'){
      if(wordc+1 == wordm){     //chech if reallocation needed
        wordm *= 2;
        (*queries)[queriesc] = realloc((*queries)[queriesc], wordm);
      }
      (*queries)[queriesc][wordc++] = ch;
    }
    (*queries)[queriesc][wordc] ='\0';
    (*queries)[queriesc] = realloc((*queries)[queriesc], wordc+1); //shrink tf
    queriesc++;
  }
  *queries = realloc(*queries, queriesc*sizeof(char*));
  *queriesNo = queriesc;
  return 1;
}

void deleteQueries(char ***queries, int queriesNo){
  if(*queries == NULL){
    fprintf(stderr, "NULL pointer given to free\n");
    return;
  }
  for(int i=0; i<queriesNo; i++)
    free((*queries)[i]);
  free(*queries);
  *queries = NULL;
}
