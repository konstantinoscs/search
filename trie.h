#ifndef TRIE_H
#define TRIE_H

#include "plist.h"

typedef struct TrieNode{
  char letter;          //the character of this node
  Plist* list;          //posting list if a word ends here
  struct TrieNode *next;       //nodes in the same level
  struct TrieNode *down;  //next letters
}TrieNode;

//initializes a trienode with character ch
void init_trie(TrieNode *node, char ch);
//inserts a word in the trie with root node
void insert_in_trie(TrieNode *node, char *word, int pos, int len, int docno);
void print_trie(TrieNode *node);
//deletes trie
void delete_trie(TrieNode *node);
//constructs trie with all words in the documents
TrieNode* makeTrie(char **documents, int docsize);
//prints the document frequency for all words in the trie
void document_frequency(TrieNode *node);
//returns the term frequency of a word in documents with id docno
int term_frequency(TrieNode *node, char *word, int docno);
//returns all appearances of a word in all documents
int document_frequency_word(TrieNode *node, char *word);
//returns the number of documents a word appears in
int document_appearances(TrieNode *node, char *word);
//stores all the document ids of a word in ids
void get_word_docid(TrieNode *node, char *word, int **ids, int *idsize);


#endif
