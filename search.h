#ifndef SEARCH_H
#define SEARCH_H

#include "trie.h"

int fill_lengths(char**documents, int docsize, int* doc_length);
int search(TrieNode *trie, char**documents, int *doc_length, int docsize,
  char**queries, int queriesNo, float k1, float b, double avgdl, int k);

#endif
