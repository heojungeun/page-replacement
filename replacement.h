#define false 0
#define true (!false)

int PAGES [20] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
int PAGES_LENGTH = sizeof(PAGES)/sizeof(PAGES[0]);
int NUM_OF_ALGOS = 3;
int MAX_FRAMES = 7;

int in_frames(int to_find, int *f, int n);
void init_array(int *f, int n, int i);
int find_max(int *f, int n);
void map_plus(int *u, int n, int c);
int find_next(int p, int c);
void print_array(int *i, int n);
void print_frame_row();
void print_misses(float **m);


float wrapper(int algo, int num_frames);
int FIFO(int num_frames);
int LRU(int num_frames);
int OPTIMAL(int num_frames);
