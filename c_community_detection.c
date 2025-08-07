/* A simplified community detection algorithm:
 *
 * Skeleton code written by Jianzhong Qi, May 2023
 * Edited by: [Muhan Chu 1346862]
 *
 */
 /* algorithms are fun */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */
#define MAX_NUM_TAG 10
#define MAX_TAG_SIZE 21
#define MAX_NUM_USER 50
#define STORAGE_YEAR 5 /* The size of array use to store year */
#define NUM_MATRIX_UNREAD 2 /* After stage_two only 2 line of matrix has been read */
#define STAGE_FOUR_PRINT_LIST 5 /* Print 5 hashtags for a line */

typedef struct {
	int user_id;
	char user_year[STORAGE_YEAR];
	char user_tag[MAX_NUM_TAG][MAX_TAG_SIZE];
    int num_tag;
} user_t;

typedef char data_t[MAX_TAG_SIZE];							  /* to be modified for Stage 4 */

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

int stage_one(user_t users[]);
void stage_two(int fred_matrix[][MAX_NUM_USER], int num_user);
void stage_three(int fred_matrix[][MAX_NUM_USER], double soc_matrix[][MAX_NUM_USER], int num_user);
void stage_four(double soc_matrix[][MAX_NUM_USER], int num_user, user_t users[]);

/* add your own function prototypes here */
void read_fred_matrix(int user_id,int num_user, int fred_matrix[][MAX_NUM_USER]);
double calculate_soc(int num_matrix1, int num_matrix2, int fred_matrix[][MAX_NUM_USER],
	int num_user);
int check_core_user(int user_id, double soc_matrix[][MAX_NUM_USER], int num_user, double ths,
	int thc);
void print_close_friend(double soc_matrix[][MAX_NUM_USER], int user_id, double ths, int num_user);
int stringcmp(char* s1, char* s2);
void strcopy(char* des, char* src);
int ckeck_hashtag_in_list(list_t* list, data_t hashtag);
list_t* loop_tags_and_insert(list_t* list, user_t users[], int user_id);

/* main function controls all the action; modify if needed */
int
main(int argc, char *argv[]) {
	int fred_matrix[MAX_NUM_USER][MAX_NUM_USER];
	double soc_matrix[MAX_NUM_USER][MAX_NUM_USER];
	user_t users[MAX_NUM_USER];
	int num_user = 0;

	/* stage 1: read user profiles */
	num_user = stage_one(users); 

	/* stage 2: compute the strength of connection between u0 and u1 */
	stage_two(fred_matrix, num_user);

	/* stage 3: compute the strength of connection for all user pairs */
	stage_three(fred_matrix, soc_matrix, num_user);

	/* stage 4: detect communities and topics of interest */
	stage_four(soc_matrix, num_user, users);

	/* all done; take some rest */
	return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read user profiles */
int 
stage_one(user_t users[]) {
	int num_users = 0; /* The number of users whose information has been read */
	int max_tags = 0; /* The number of hashtags that the user has the most */
	int max_user = 0; /* The user has the most hashtags */
	/* Keep reading each line of user, starting with the id and year */
	while(scanf("u%d %s ", &users[num_users].user_id, users[num_users].user_year) == 2){
		/* The num of hashtags for the current user */
		int num_tag = 0;	   
		/* Read and record the current user's hashtag */
		while(scanf("#%s ", users[num_users].user_tag[num_tag]) == 1) {
			num_tag += 1;
		}

		users[num_users].num_tag = num_tag;
		/* Record the id of the user with the most hashtags and the num of hashtags */
		if (num_tag > max_tags) {
			max_tags = num_tag;
			max_user = num_users;
		}

		num_users += 1;
	}
	/* print out the information that question need */
	print_stage_header(STAGE_NUM_ONE);
	printf("Number of users: %d\n", num_users);
	printf("u%d has the largest number of hashtags:\n", max_user);
	int i;
	for (i = 0; i < max_tags; i++) {
		if(i == max_tags-1) {
			printf("#%s\n", users[max_user].user_tag[i]);
		}else {
			printf("#%s ", users[max_user].user_tag[i]);
		}

	}

	printf("\n");
	return num_users;
}



/* stage 2: compute the strength of connection between u0 and u1 */
void 
stage_two(int fred_matrix[][MAX_NUM_USER], int num_user) {
	/* Read the friendship matrix of u0 and u1 */
	read_fred_matrix(0, num_user, fred_matrix);
	read_fred_matrix(1, num_user, fred_matrix);
	/* Calculate the soc */
	double soc = 0; /* Record the strength of connection between u0 and u1 */
	/* Check whether u1 and u0 are friends */
	if (fred_matrix[0][1] == 0){ 
		soc = 0;
	} else { 
		soc = calculate_soc(0,1,fred_matrix,num_user);
	}

	/* print out the information that question need */
	print_stage_header(STAGE_NUM_TWO);
	printf("Strength of connection between u0 and u1: %4.2f\n", soc);
	printf("\n");
}



/* stage 3: compute the strength of connection for all user pairs */
void 
stage_three(int fred_matrix[][MAX_NUM_USER], double soc_matrix[][MAX_NUM_USER], int num_user) {
	/* Read the remaining friendsh matrix */
	int i, j;
	for(i = NUM_MATRIX_UNREAD; i < num_user; i++){
		read_fred_matrix(i, num_user, fred_matrix);
	}
	/* Loop over any two Users and compute the soc between them */
	for(i = 0; i < num_user; i++){
		for(j = 0; j < num_user; j++){
			/*useri and userj are not friends */
			if(fred_matrix[i][j] == 0){
				soc_matrix[i][j] = 0;
			}else{
				soc_matrix[i][j] = calculate_soc(i, j,fred_matrix, num_user);
			}

		}

	}
	print_stage_header(STAGE_NUM_THREE);
	/* print soc_matrix */
	for(i = 0; i < num_user; i++){
		for(j = 0; j < num_user; j++){
			if(j == num_user-1){
				printf("%4.2f\n", soc_matrix[i][j]);
			} else {
				printf("%4.2f ", soc_matrix[i][j]);
			}

		}

	}
				
	printf("\n");
}




/* stage 4: detect communities and topics of interest */
void stage_four(double soc_matrix[][MAX_NUM_USER], int num_user, user_t users[]) {
	double ths = 0;
	int thc =0;
	print_stage_header(STAGE_NUM_FOUR);
	/* Read the last two values */
	scanf("%lf %d \n", &ths, &thc);
	int i, j;
	/* Loop through the soc_matrix for each user */
	for(i = 0; i < num_user; i++){
		/* The user is core user */
		if(check_core_user(i, soc_matrix, num_user, ths,thc) == 1){
			printf("Stage 4.1. Core user: u%d;", i);
			print_close_friend(soc_matrix, i, ths, num_user);
			printf("\n"); 
			printf("Stage 4.2. Hashtags:\n");
			/* Make a empty list */
			list_t* list = make_empty_list(); 
			/*Loop over the core user's hasgtags and insert the distinct hashtags */
			list = loop_tags_and_insert(list, users, i);
			/* Compare the hashtags of core user with the hashtags of close friend */
			for(j = 0; j < num_user; j++) {
				/* find close friend */
				if(soc_matrix[i][j] > ths) {
					/*Loop over the close friend hashtag */
					list = loop_tags_and_insert(list, users, j);
				}

			}

			print_list(list);
		}

	}

}



/* Calculate the value of the soc */
double calculate_soc(int num_matrix1, int num_matrix2, int fred_matrix[][MAX_NUM_USER],
	int num_user){
	int union_fred = 0;
	int intersect_fred = 0;
	double soc = 0;
	int i;
	for(i = 0; i < num_user; i++){
		/* Count the number of mutual friends */
		if (fred_matrix[num_matrix1][i] == 1 && fred_matrix[num_matrix2][i] == 1){
			intersect_fred +=1;
		}
		/* Calculate the total number of friends of two users */
		if (fred_matrix[num_matrix1][i] == 1 || fred_matrix[num_matrix2][i] == 1){
			union_fred +=1;
		}

	}

	soc = (double)intersect_fred/union_fred;
	return soc;
}




/* Reading the friendship matrix */
void read_fred_matrix(int user_id, int num_user, int fred_matrix[][MAX_NUM_USER]){
	int i;
	for(i = 0; i < num_user; i++){
		int num;
		scanf("%d", &num);
		fred_matrix[user_id][i] = num;
	}
}



/* Check if user is a core user */
int check_core_user(int user_id, double soc_matrix[][MAX_NUM_USER], int num_user, double ths,
	int thc){
	/* Loop through the user's soc matrix, keeping track of how many CloseFriends he has */
	int i;
	int count_close = 0;
	for (i = 0; i < num_user; i++) {
		if (soc_matrix[user_id][i] > ths) {
			count_close += 1;
		}

	}
		
	if (count_close > thc) {
		return 1;
	}
	/* user is not core user */
	return 0;
}




/* Find the user's close friends and print their id */
void print_close_friend(double soc_matrix[][MAX_NUM_USER], int user_id, double ths, int num_user){
	int i;
	printf(" close friends:");
	for(i = 0; i < num_user; i++){
		if(soc_matrix[user_id][i] > ths){
			printf(" u%d", i);
		}

	}
}



/* copy the content */
void strcopy(char* des, char* src){
	while(*src){
		*des = *src;
		des++;
		src++;
	}

	*des = '\0';
}




/* Compare two string */
int stringcmp(char* s1, char* s2){
	while(*s1 && *s2){ /* compare the char in same position */
		if(*s1 > *s2){
			return 1;
		}

		if(*s1 < *s2){
			return -1;
		}

		s1++;
		s2++;
	}
	/* s2 is longer than s1 */
	if (*s1 == '\0' && *s2 != '\0'){
		return -1;
	}
	/* s1 is longer than s2 */
	if(*s2 == '\0' && *s1 != '\0'){
		return 1;
	}
	/* s1 and s2 is same */
	return 0;
}



/* Check if the hashtags appear in the list */
int ckeck_hashtag_in_list(list_t* list, data_t hashtag){
	/* Let temp record the address of the first node */
	node_t* temp = list->head; 
	while(temp != NULL){  
		if(stringcmp(temp->data,hashtag) == 0){
			
			return 1;
		}
		/* Update temp to the address of the next node */

		temp = temp->next;

	}

	return 0;
}


/*Loop through the hashtags and add the eligible hashtags to the list */
list_t* loop_tags_and_insert(list_t* list, user_t users[], int user_id){
	int k;
	/*Loop over the hasgtags */
	for(k = 0; k < users[user_id].num_tag; k++) {
		/* Check whether the core user's hashtag is in the list */
		if(ckeck_hashtag_in_list(list, users[user_id].user_tag[k]) == 0) {
			list = insert_unique_in_order(list, users[user_id].user_tag[k]);
		}

	}

	return list;
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
list_t *insert_unique_in_order(list_t *list, data_t value){
	/* creat a node */
	node_t *new = (node_t*)malloc(sizeof(*new)); 
	assert(list!=NULL && new!=NULL); 
	strcopy(new->data, value); 
	new->next = NULL; 
	/* Let value be added to the empty list */
	if (list->head==NULL) { /* The list is empty, with no nodes */
		list->head = list->foot = new;
	} else if (stringcmp(new->data, list->head->data) < 0){ /* New is smaller than the first node */
		new->next = list->head;
		list->head = new;
	} else if (stringcmp(new->data, list->foot->data) > 0){ /* New id biiger than last node */
		list->foot->next = new;
		list->foot = new;
	}else{
		node_t* current = list ->head;
		node_t* preview = NULL;
		/* Keep the address of the current node and the address of the previous node */
		while(current != NULL){
			preview = current;
			current = current ->next;
			if(stringcmp(new->data, current->data) < 0){
				break; /* This means that current is the first node that is greater than value */
			}

		}

		new->next = current;
		preview->next = new;
	}
	
	return list;
}

/* print the data contents of a list */
void
print_list(list_t *list) {
	int count = 1; /* Record how much hashtag has been print */
	node_t* print = list->head;
	while(print != NULL){
		if(count % STAGE_FOUR_PRINT_LIST == 0 || print->next == NULL){
			printf("#%s\n",  print->data);
			count += 1;
		} else {
			printf("#%s ",print->data);
			count += 1;
		}

		print = print->next;
	}

	free_list(list);
}


/****************************************************************/
/*
	Write your time complexity analysis below for Stage 4.2, 
	assuming U users, C core users, H hashtags per user, 
	and a maximum length of T characters per hashtag:

   O(U*U*((H*H*T)+(U*H*H*T)+(H*T)))
*/