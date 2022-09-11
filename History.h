#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>

//Kelsey Porter

//for creating history as implemented we need a circular linked list

struct HistoyPoint {
    char line[512];
    int idx;
};

struct HistoyPoint history[21];
unsigned int size; //pretend this is 0
int last_global_index = 1;
int last_indx;

/*void swap(struct HistoyPoint *xp, struct HistoyPoint *yp) {
    struct HistoyPoint temp = *xp;
    *xp = *yp;
    *yp = temp;
}*/

/*void sort_history() { //absolutely useless
    int i, j, min_idx;
    //int dix = size > 10 ? 10 : size;
	int dix;
	if (size > 20) {
		dix = 20;
	}
	else {
		dix = size;
	}
    // One by one move boundary of unsorted array
    for (i = 0; i < dix - 1; i++) {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < dix; j++) { 
            if (history[j].idx < history[min_idx].idx) 
                min_idx = j;
		}
		//printf("%s %d %s %d\n", "i is ", i, " and min_idx is ", min_idx);
		//printf("%s %d %s %d\n", "history[i] is ", history[i].idx, " and history[min_idx] is ", history[min_idx].idx);
        // Swap the found minimum element with the first element
        swap(&history[min_idx], &history[i]);
		//printf("%s %d %s %d\n", "swapped ", history[min_idx].idx, " with ", history[i].idx);
		//printf("%s\n", "-------------------");
    }
}*/

void print_history() {
	int idx = size;
	if (size > 21) {
		idx = 21;
	}
    //sort_history();
	//printf("%s %d\n", "size = ", size);
    for (int i = 0; i < idx - 1; i++) {
        printf("[%d] : %s\n", history[i].idx, history[i].line);
    }
}

void add_command_to_history(char *line) {
    struct HistoyPoint newPoint; //make new point
    newPoint.idx = last_global_index; //new point = last index
    last_global_index++; //increase last index
    strcpy(newPoint.line, line); //copy line into new point
    if (size < 20) {
		history[size] = newPoint;
	}
	else {
		//printf("%d\n", (size+1));
		history[size % 20] = newPoint;
		//printf("%s %d %s %d\n", "the number in ", (size+1) % 20, " is ", history[size % 20].idx);
		//printf("%s\n", "----------------------");
	}
    //history[size % 20] = newPoint;
    size++; //increase size
	//printf("%d\n", size);
}

int get_history_point_idx(int idx) {
    for (int i = 0; i < size; i++) {
        if (history[i].idx == idx) {
            return i;
		}
    }
    return -1;
}

//take the command line input
int run_history_command(char *commands[], char line[512]) {
    int msize = 0;
    for (msize = 0; NULL != commands[msize]; msize++)
        ;
    if (msize == 1) { //execute myhistory
        print_history();
    }
    else if (msize == 2) {
        //clear command
        if (strcmp(commands[1], "-c") == 0) { //erase myhistory
            size = 0;
            printf("myHistory is cleared\n");
        }
    }
    else if (msize == 3) {
        //re-execute command
        if (strcmp(commands[1], "-e") == 0) {
            int num;
            sscanf(commands[2], "%d", &num);
            num = get_history_point_idx(num); //execute number
            if (num != -1) {
                strcpy(line, history[num].line);
                return 1;
            }
        }
        fprintf(stderr, "Error using the myhistory command\nUsage:myhistory  -e <myhistory_number>\n");
    }
    else {
        fprintf(stderr, "Error using the myhistory command\n");
    }
    return 0;
}
