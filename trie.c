#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "plist.h"

//initialize a trie node
void init_trie(TrieNode *node, char ch){
  node->letter = ch;
  node->list = NULL;
  node->next = NULL;
  node->down = NULL;
}

//initialize the posting list for a word
void init_plist(TrieNode *node, char* word, int len, int docno){
  node->list = malloc(sizeof(Plist));
  node->list->word = malloc(len+1);
  strncpy(node->list->word, word, len);
  node->list->word[len] = '\0';
  node->list->appearances = 1;
  node->list->docs = 1;
  //initialize the frequencies list
  node->list->frequencies = malloc(sizeof(FreqInfo));
  node->list->frequencies->doc = docno;
  node->list->frequencies->appreance = 1; //add the first appearance
  node->list->frequencies->next = NULL;
}

void insert_in_trie(TrieNode *node, char *word, int pos, int len, int docno){
  if(node->letter == word[pos]){
    if(pos+1 == len){  //word has ended with the current letter
      if(node->list ==NULL)
        init_plist(node, word, len, docno);
      else
        search_n_update(node->list, docno);
    }
    else{   //go one level down to next letter (vertical movement)
      if(node->down !=NULL && node->down->letter > word[pos+1]){ //(LAW &) ORDER
        TrieNode* temp = malloc(sizeof(TrieNode));
        init_trie(temp, word[pos+1]);
        temp->next = node->down;
        node->down = temp;
      }
      else{
        if(node->down == NULL){
          node->down = malloc(sizeof(TrieNode));
          init_trie(node->down, word[pos+1]);
        }
      }
      insert_in_trie(node->down, word, ++pos, len, docno);
    }
  }
  else { //go to the next letter (horizontal movement)
    if(node->next!=NULL && node->next->letter > word[pos]){ //maintain order
      TrieNode* temp = malloc(sizeof(TrieNode));
      init_trie(temp, word[pos]);
      temp->next = node->next;
      node->next = temp;
    }
    else{
      if(node->next ==NULL){
        node->next = malloc(sizeof(TrieNode));
        init_trie(node->next, word[pos]);
      }
    }
    insert_in_trie(node->next, word, pos, len, docno);
  }
}

void swap_root(TrieNode **root, char ch){
  TrieNode* newroot = malloc(sizeof(TrieNode));
  init_trie(newroot, ch);
  newroot->next = *root;
  *root = newroot;
}

//recursively (sort of) print trie
void print_trie(TrieNode *node){
  printf("%c ", node->letter);
  if(node->list !=NULL){
    printf(" word here: %s ", node->list->word);
    print_list(node->list);
    printf(" ");
  }
  if(node->down!=NULL){
    printf("down: ");
    print_trie(node->down);
  }
  else printf("\n");
  if(node->next!=NULL){
    printf("Next: ");
    print_trie(node->next);
  }
}

void delete_trie(TrieNode *node){
  if(node->down!=NULL){
    delete_trie(node->down);
    free(node->down);
  }
  if(node->next!=NULL){
    delete_trie(node->next);
    free(node->next);
  }
  if(node->list!=NULL){
    delete_list(node->list);
    free(node->list);
  }
}

TrieNode* makeTrie(char **documents, int docsize){
  TrieNode * trie = malloc(sizeof(TrieNode));
  char *word = NULL, *subs = NULL, ch= ' ';
  size_t len = 0, j=0;
  init_trie(trie, documents[0][0]);
  for(int i=0; i<docsize; i++){
    ch = ' ';
    subs = documents[i];
    //printf("%s\n", subs);
    while(ch!= '\0'){   //start breaking each document in words
      len=0;
      //count the length of each word
      while((ch = subs[len]) != ' ' && ch !='\t' && ch != '\0')
        len++;
      //copy the word in variable
      word = subs;
      // word = realloc(word, len+1);
      // strncpy(word, subs, len);
      // word[len] = '\0';
      //printf("%s\n", word);
      if(trie->letter > word[0])     //swap root to maintain order (& law)
        swap_root(&trie, word[0]);
      //insert to trie
      insert_in_trie(trie, word, 0, len, i);
      subs += len;
      if(ch != '\0')           //if it's the end of the document don't search
        while(isspace(subs[0]))   //set substring to new word
          subs++;
    }
  }
  //free(word);
  return trie;
}

void document_frequency(TrieNode *node){
  if(node->list !=NULL){
    printf("%s %d\n", node->list->word, calculate_doc_appearances(node->list));
  }
  if(node->down!=NULL)
    document_frequency(node->down);
  if(node->next!=NULL)
    document_frequency(node->next);
}

TrieNode* find_word(TrieNode *node, char* word){
  TrieNode *current = node;
  size_t pos = 0, len = strlen(word);
  while(current!=NULL && pos!=len){   //start traversing the trie
    if(word[pos]==current->letter){
      pos++;
      if(pos!=len)  //word has not ended yet
        current = current->down;
    }
    else
      current = current->next;
  }
  if(current == NULL || current->list == NULL)       //the word is not in the trie
    return NULL;
  return current;
}

int term_frequency(TrieNode *node, char *word, int docno){
  TrieNode *current = find_word(node, word);
  if(current == NULL)
    return 0;
  return caclulate_specific_frequency(current->list, docno); //this is tf
}

int document_frequency_word(TrieNode *node, char *word){
  TrieNode *current = find_word(node, word);
  if(current == NULL)
    return 0;
  return calculate_frequency(current->list);
}

int document_appearances(TrieNode *node, char *word){
  TrieNode *current = find_word(node, word);
  if(current == NULL)
    return 0;
  return calculate_doc_appearances(current->list);    //this is n(qi)
}

void get_word_docid(TrieNode *node, char *word, int **ids, int *idsize){
  TrieNode *current = find_word(node, word);
  if(current == NULL){
    (*ids) = NULL;
    (*idsize) = 0;
  }
  else
    get_all_docid(current->list, ids, idsize);
}
