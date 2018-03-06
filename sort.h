#ifndef SORT_H
#define SORT_H

typedef struct {
  double score;
  int doc;
}ScoreElem;

void swap(int *a, int *b);
void quicksort(int *array, int left, int right);
void scoreQuicksort(ScoreElem *array, int left, int right);

#endif
