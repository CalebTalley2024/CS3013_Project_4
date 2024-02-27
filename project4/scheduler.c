#include <stdio.h>

struct job {
		int id ;
		int length ;
		// other meta data
		struct job * next ;
};

// const int is_rr = strcmp ( algorithm , "RR") == 0;
// const int is_prio = strcmp ( algorithm , " PRIO ") == 0;

// // ....

//  if ( is_rr ) {
// // Something special for round - robin .
//  }

// // ....

// if ( is_prio ) {
// // Calculate extra stuff for each priority level .
// }



int main(int argc, char **argv){

    printf("Hello, please help me schedule!\n");

}