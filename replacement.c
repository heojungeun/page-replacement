#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "replacement.h"


int main (int argc, char *argv[])
{

  float **misses = (float **)malloc(sizeof(float*)*NUM_OF_ALGOS);
  
  int i,j;
  for (i = 0; i < NUM_OF_ALGOS; ++i) {
    misses[i] = (float *)malloc(sizeof(float)*MAX_FRAMES);
  }

  for (i = 0; i < NUM_OF_ALGOS; ++i){
    for (j = 1; j < MAX_FRAMES+1; ++j) {
      misses[i][j] = wrapper(i,j);
    }
  }

  print_frame_row();
  print_misses(misses);
  for (i = 0; i < NUM_OF_ALGOS; ++i) {
    float* current = misses[i];
    free(current);
  }
  return 0;
}

float wrapper(int algo, int num_frames){
  int misses;
  switch (algo){
    case 0:
      misses = FIFO(num_frames);
      break;
    case 1:
      misses = LRU(num_frames);
      break;
    case 2:
      misses = OPTIMAL(num_frames);
      break;
    default:
      misses = 0;
      break;
  }
  return ((float) misses/20);
}

int FIFO(int num_frames){
  int misses = 0;
  int f_in = 0;
  int *frames = (int *)malloc(sizeof(int)*num_frames);
  
  init_array(frames, num_frames, -1);
  
  int i;
  for (i = 0; i< PAGES_LENGTH; ++i) {
    if (in_frames(PAGES[i], frames, num_frames)!=-1)
    {
      continue;
    }
    else {
      frames[f_in] = PAGES[i];
      f_in = (f_in + 1) % num_frames;
      ++misses;
    }
  }
  free(frames);
  return misses;
}

int LRU(int num_frames) {
  int misses = 0;
  int max_index = -1;
  int page_index = 0;
  int *frames = (int *)malloc(sizeof(int)*num_frames);
  int *used = (int *)malloc(sizeof(int)*num_frames);

  init_array(frames, num_frames, -1);
  init_array(used, num_frames, INT_MAX);

  int i;
  for (i = 0; i< PAGES_LENGTH; ++i) {
    page_index = in_frames(PAGES[i], frames, num_frames);
    if (page_index == -1) {
      ++misses;
      max_index = find_max(used, num_frames);
      frames[max_index] = PAGES[i];
      map_plus(used, num_frames, 1);
      used[max_index] = 0;
    }
    else {
      map_plus(used, num_frames, 1);
      used[page_index] = 0;
    }
  }
  free(frames);
  free(used);
  return misses;
}

int OPTIMAL(int num_frames) {
  int misses = 0;
  int max_index = -1;
  int page_index = 0;
  int *frames = (int *)malloc(sizeof(int)*num_frames);
  int *until = (int *)malloc(sizeof(int)*num_frames);

  init_array(frames, num_frames, -1);
  init_array(until, num_frames, INT_MAX);

  int i;
  for (i = 0; i < PAGES_LENGTH; ++i) {
    page_index = in_frames(PAGES[i], frames, num_frames);
    if (page_index == -1) {
      ++misses;
      max_index = find_max(until, num_frames);
      frames[max_index] = PAGES[i];
      map_plus(until, num_frames, -1);
      until[max_index] = find_next(PAGES[i], i);
    }
    else {
      map_plus(until, num_frames, -1);
      until[page_index] = find_next(PAGES[i],i);
    }
  }
  free(frames);
  free(until);
  return misses;
}

void init_array(int *frames, int num_frames, int init_value) {
  int i;
  for (i = 0; i< num_frames; ++i) {
    frames[i] = init_value;
  }
}

int in_frames(int to_find, int *frames, int num_frames){
  int i;
  for (i = 0; i< num_frames; ++i) {
    if (frames[i] == to_find) {
      return i;
    }
  }
  return -1;
}

int find_max(int *used, int num_frames) {
  int max_value = INT_MIN;
  int max_index = 0;

  int i;
  for (i = 0; i < num_frames; ++i){ 
    if (used[i] > max_value) {
      max_value = used[i];
      max_index = i;
    }
  }
  return max_index;
}

void map_plus(int *frames, int num_frames, int change_by){
  int i;
  for (i = 0; i< num_frames; ++i) {
    if (frames[i] != INT_MAX) {
      frames[i] = frames[i] + change_by;
    }
  }
}

int find_next(int page, int current) {
  int count = 1;
  int i;
  for (i = current+1; i < PAGES_LENGTH; ++i) {
    if (PAGES[i] == page) {
      return count;
    }
    ++count;
  }
  return INT_MAX;
}

void print_array(int *frames, int num_frames){
  int i;
  for (i = 0; i < num_frames; ++i) {
    printf("%i\t", frames[i]);
  }
  printf("\n");
}

void print_frame_row() {
  printf("     ");
  int i;
  for (i = 1; i <= 7; ++i) {
    printf("\t  %i", i);
  }
  printf("\n");
}

void print_misses(float **misses) {
  int i,j;
  for (i = 0; i < NUM_OF_ALGOS; ++i) {
    if (i == 0) {
      printf("FIFO:\t"); 
    }
    else if (i == 1) {
      printf("LRU :\t");
    }
    else {
      printf("OPT :\t");
    }
    for (j = 1; j <= MAX_FRAMES; ++j) {
      printf("%.2f\t", misses[i][j]);
      if ( j == MAX_FRAMES) {
        printf("\n");
      }
    }
  }
} 
