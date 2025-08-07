/* A simplified community detection algorithm:
 *
 * Skeleton code written by Jianzhong Qi, May 2023
 * Edited by: [Add your name and student ID here for automatic recognition]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

typedef struct {
	/* add your user_t struct definition */
} user_t;

typedef int data_t;							  /* to be modified for Stage 4 */

/* linked list type definitions below, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/
typedef struct node node_t;

struct node {
	data_t data;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

/****************************************************************/

/* function prototypes */
list_t *make_empty_list(void);
void free_list(list_t *list);
void print_list(list_t *list);
list_t *insert_unique_in_order(list_t *list, data_t value);

void print_stage_header(int stage_num);

void stage_one();
void stage_two();
void stage_three();
void stage_four();

/* add your own function prototypes here */

/****************************************************************/

/* main function controls all the action; modify if needed */
int
main(int argc, char *argv[]) {
	/* add variables to hold the input data */

	/* stage 1: read user profiles */
	stage_one(); 
	
	/* stage 2: compute the strength of connection between u0 and u1 */
	stage_two();
	
	/* stage 3: compute the strength of connection for all user pairs */
	stage_three();
	
	/* stage 4: detect communities and topics of interest */
	stage_four();
	
	/* all done; take some rest */
	return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read user profiles */
void 
stage_one() {
	/* add code for stage 1 */
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);

	printf("\n");
}

/* stage 2: compute the strength of connection between u0 and u1 */
void 
stage_two() {
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);

	printf("\n");
}

/* stage 3: compute the strength of connection for all user pairs */
void 
stage_three() {
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);

	printf("\n");
}

/* stage 4: detect communities and topics of interest */
void stage_four() {
	/* add code for stage 4 */
	/* print stage header */
	print_stage_header(STAGE_NUM_FOUR);

	printf("\n");
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* linked list implementation below, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/

/* create an empty list */
list_t
*make_empty_list(void) {
	list_t *list;

	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;

	return list;
}

/* free the memory allocated for a list (and its nodes) */
void
free_list(list_t *list) {
	node_t *curr, *prev;

	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}

	free(list);
}

/* insert a new data element into a linked list, keeping the
	data elements in the list unique and in alphabetical order
*/
list_t
*insert_unique_in_order(list_t *list, data_t value) {
	/* the following code inserts a new node to the end of list.
		Modify it to suit Stage 4.2. Start by modifying it to 
		insert a new node while keeping an alphabetical order
		(think about how to insert in the middle of a list), 
		then, only insert when value is not in list already.
		[remove this comment to save line space if needed]
	*/

	node_t *new;

	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}

	return list;
}

/* print the data contents of a list */
void
print_list(list_t *list) {
	/* add code to print list */
}

/****************************************************************/
/*
	Write your time complexity analysis below for Stage 4.2, 
	assuming U users, C core users, H hashtags per user, 
	and a maximum length of T characters per hashtag:

*/
