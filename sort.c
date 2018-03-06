#include "sort.h"

void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int *array, int left, int right){
  int pivot = array[right];
  int i = left-1;
  for(int j=left; j<=right-1; j++){
    if(array[j]<=pivot){
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i + 1], &array[right]);
  return (i + 1);
}

void quicksort(int *array, int left, int right){
  if (left<right){
    int pivot = partition(array, left, right);
    quicksort(array, left, pivot-1);
    quicksort(array, pivot+1, right);
  }
}

void scoreSwap(ScoreElem *a, ScoreElem *b){
  ScoreElem t;
  t.doc = a->doc;
  t.score = a->score;
  a->doc = b->doc;
  a->score = b->score;
  b->doc = t.doc;
  b->score = t.score;
}

int scorePartition(ScoreElem *array, int left, int right){
  double pivot = array[right].score;
  int i = left-1;
  for(int j=left; j<=right-1; j++){
    if(array[j].score>=pivot){
      i++;
      scoreSwap(&array[i], &array[j]);
    }
  }
  scoreSwap(&array[i + 1], &array[right]);
  return (i + 1);
}

void scoreQuicksort(ScoreElem *array, int left, int right){
  if (left<right){
    int pivot = scorePartition(array, left, right);
    scoreQuicksort(array, left, pivot-1);
    scoreQuicksort(array, pivot+1, right);
  }
}
