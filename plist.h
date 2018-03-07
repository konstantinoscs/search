#ifndef PLIST_H
#define PLIST_H

typedef struct FreqInfo{
  int doc;          //document id
  int appreance;    //no of appearances in that doc
  struct FreqInfo *next;
}FreqInfo;

typedef struct Plist{
  char * word;        //the word of this list
  int appearances;    //total appearances of word in all documents
  int docs;       //no of documents that have this word in them
  FreqInfo *frequencies;    //frequencies list
  FreqInfo *last;         //last to speed up updates
}Plist;

void delete_freq(FreqInfo *frequency);
void search_n_update(Plist *list, int docno);
void delete_list(Plist *list);
void print_list(Plist *list);
int calculate_frequency(Plist *list);
int caclulate_specific_frequency(Plist *list, int docno);
//where is overloading when you need it? :'(
int calculate_doc_appearances(Plist *list);
//store all document IDs that a word appears in in array ids
void get_all_docid(Plist *list, int **ids, int*idsize);

#endif
