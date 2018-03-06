#include <stdio.h>
#include <stdlib.h>

#include "plist.h"

void delete_freq(FreqInfo *frequency){
  if(frequency->next!=NULL){
    delete_freq(frequency->next);
    free(frequency->next);
  }
}

void print_freq(FreqInfo *frequency){
  printf("[%d, %d]", frequency->doc, frequency->appreance);
  if(frequency->next!=NULL){
    printf(",");
    print_freq(frequency->next);
  }
}

//returns the appearances of a word in a specific doc
int search_n_get(FreqInfo *frequency, int docno){
  FreqInfo *current = frequency;
  while(current->doc != docno){
    if(current->next == NULL)
      return 0;
    current = current->next;
  }
  return current->appreance;
}

void delete_list(Plist *list){
  if(list->frequencies!=NULL){
    delete_freq(list->frequencies);
    free(list->frequencies);
  }
  if(list->word!=NULL)
    free(list->word);
}

//update the posting list for a word
void search_n_update(Plist *list, int docno){
  FreqInfo *current = list->frequencies;
  while(current->doc!=docno){
    if(current->next == NULL){
      current->next = malloc(sizeof(FreqInfo));
      current->next->doc = docno;
      current->next->appreance = 0;
      current->next->next = NULL;
      list->docs++;
    }
    current = current->next;
  }
  current->appreance++;
  list->appearances++;
}

void print_list(Plist *list){
  printf("[");
  print_freq(list->frequencies);
  printf("]");
}

int calculate_frequency(Plist *list){
  return list->appearances;
}

int caclulate_specific_frequency(Plist *list, int docno){
  return search_n_get(list->frequencies, docno);
}

int calculate_doc_appearances(Plist *list){
  return list->docs;
}

//store all document IDs that a word appears in in array ids
void get_all_docid(Plist *list, int **ids, int*idsize){
  FreqInfo *current = list->frequencies;
  int currentId = 0;
  *idsize = list->docs;
  *ids = malloc(list->docs*sizeof(int));
  while(current!=NULL){
    // if(currentId+1 == (*idsize)){   //realloc condition
    //   (*idsize) *= 2;
    //   *ids = realloc(*ids, (*idsize)*sizeof(int));
    // }
    (*ids)[currentId++] = current->doc;
    current = current->next;
  }
  //*ids = realloc(*ids, currentId*sizeof(int));
  //*idsize = currentId;
}
