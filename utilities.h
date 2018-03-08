#ifndef UTILITIES_H
#define UTILITIES_H

#include "trie.h"

int parseArguments(int argc, char **argv, char **doc, int *k);
//parseDocument reads all the documents in the file specified by doc
int parseDocument(char *doc, char ***documents, int *docsize);
void printDocuments(char **documents, int docsize, char* desc);
int readQueries(char ***queries, int *queriesNo);
void deleteQueries(char ***queries, int queriesNo);
//cam be also used for non emergency cleanup :)
void emergency_exit(char ***documents, int docsize, char *doc, char* cmd);
void cleanup(char ***documents, int docsize, char *doc, char* cmd,
  int *doc_length, TrieNode *root);
#endif
