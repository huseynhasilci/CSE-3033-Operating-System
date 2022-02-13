#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

// #define READ_FLAGS O_RDONLY
#define COUNT 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


struct thread_infos{  // my linked list to add the readed words
	char text[100];
	int index;
	struct thread_infos *next;
};


void printList(struct thread_infos *node,long id){
	int index = 0;
	while(node != NULL){
		printf("THREAD %ld: Added '%s' at index %d\n",id,node->text,index);
		//printf("%d",index);
		index++;
		node = node->next;
	}
}

int searchKey(struct thread_infos* head, char* argv){
	struct thread_infos* current = head;
	while(current != NULL){
		if (strcmp(current->text,argv) == 0 )
		{
			return 1;
		}
		current = current->next;
	}
	return 0;

}
/*void get_count(struct thread_infos* head){
	int count = 0;
	struct thread_infos* current = head;
	while(current!=NULL){
		COUNT++;
		current = current->next;
	}
	
}*/

void append_into_thread_infos(struct thread_infos** head_ref,char* argv){  // apending into the linked list

	struct thread_infos* new_node = (struct thread_infos*)malloc(sizeof(struct thread_infos));
	struct thread_infos *last = *head_ref;


	strcpy(new_node->text,argv);
	new_node->index++;
	//new_node->text = &argv;
	new_node->next = NULL;


	if(*head_ref == NULL){
		*head_ref = new_node;
		return;
	}
	while(last->next!=NULL){
		last = last->next;
	}
	last->next = new_node;
	//return;

}

void *ReadTheFile(void *filename){

	struct thread_infos* head = NULL;
	char mystring[100];
	FILE *pfile;
	char *pch;
	char *here_filename = filename;

	FILE *fptr;
	char c;
	fptr = fopen(here_filename,"r");
	if(fptr == NULL){
		fprintf(stderr, "File did not found.\n");
	}
	c = fgetc(fptr);
	printf("The content of %s.txt is:\n",here_filename);
	while(c != EOF){

		printf("%c",c);
		c = fgetc(fptr);
	}
	fclose(fptr);

	int search_value;
	printf("MAIN THREAD: Assigned %s to worker thread %ld\n",here_filename,pthread_self());
	//printf("%s\n",here_filename);
	pfile = fopen(here_filename,"r");
	if (pfile == NULL){
		fprintf(stderr, "Couldn't find the file\n" );
	}

	else{
		pthread_mutex_lock(&mutex);  // mutex
		while(fgets(mystring,100,pfile)!=NULL){
			pch = strtok(mystring," \n");
			append_into_thread_infos(&head,pch);
			while(pch != NULL){
				//printf("%s\n",pch);
				/**/
				pch = strtok(NULL," \n");
				if (pch == NULL)
				{
					continue;
				}
				//printf("Burada %d\n",searchKey(head,pch));
				
				
					
				append_into_thread_infos(&head,pch);
				
				
			}

		}
		fclose(pfile);
		pthread_mutex_unlock(&mutex);

	}

	//printf("Buraya girdi\n");
	//get_count(head);
	printList(head,pthread_self());
	pthread_exit((void *) filename);
}




int main(int argc,char *argv[]){

	
	//printf("%s\n",dir->d_name);

	if (argc != 5){
		fprintf(stderr, "USAGE: project3.o -d <directoryName> -n <#ofTheads> You give attributes more or less then five please give five arguments to the system.\n" );
	}
	else if(strcmp(argv[1],"-d")!=0){
		fprintf(stderr, "Second argument is not -d please write -d\n");
	}
	else if(strcmp(argv[3],"-n")!=0){
		fprintf(stderr, "Fourth argument is not -n please write -d\n");
	}
	// ********************************* Dosyanin icindeki degerleri teker teker okuma *********************************
	/*
	FILE *pfile;
	char mystring[100];
	char *pch; 

	pfile = fopen("filename1.txt","r");
	if (pfile == NULL){
		fprintf(stderr, "Couldn't find the file\n" );
	}
	else{
		while(fgets(mystring,100,pfile)!=NULL){
			//printf("%s\n", mystring);
			pch = strtok(mystring," ");
			while(pch != NULL){
				printf("%s\n",pch);
				pch = strtok(NULL," ");
			}

		}
		fclose(pfile);
	}*/
	// ********************************* Dosyanin icindeki degerleri teker teker okuma *********************************




	int NUMBER_OF_THREAD = atoi(argv[4]);
	int num_of_files = 0;
	DIR *d1;
	struct dirent *dir1;
	d1 = opendir(argv[2]);
	if (d1 == NULL){
		printf("Could not open current directory");
	}
	if (d1){
		while((dir1 = readdir(d1))!=NULL){
			if (strstr(dir1->d_name,".txt")==NULL){
				continue;
			}
			num_of_files++; // get all the number of files in the directory



		}
		closedir(d1);
	}
	/*******************************************/


	int i;
	struct thread_infos* head;
	pthread_t thread[NUMBER_OF_THREAD];
	void *status;
	int rc;
	DIR *d;
	struct dirent *dir;


	if ((num_of_files == NUMBER_OF_THREAD) || (NUMBER_OF_THREAD > num_of_files))  // checking number of files are equl or lower
	{
		d = opendir(argv[2]);
		if (d == NULL){
			printf("Could not open current directory");
		}	
		if (d){
			while((dir = readdir(d))!=NULL){
				if (strstr(dir->d_name,".txt")==NULL){
					continue;
				}
				for (i = 0; i<NUMBER_OF_THREAD;i++){
					rc = pthread_create(&(thread[i]),NULL,&ReadTheFile,(void*) dir->d_name);  // creating the threads
					if(rc){
						printf("Error: return code from pthread_create() is %d\n",rc);
						exit(-1);
					}
					
				}
				
				for (i=0; i<NUMBER_OF_THREAD; i++){
					rc = pthread_join(thread[i],NULL);  // waiting the threads
					if(rc){
						printf("Error: return code from pthread_create() is %d\n",rc);
						exit(-1);
					}
				}


			}
			closedir(d);
		}
	}
	else{
		fprintf(stderr, "Number of the threads are not equal to number of the files or number of the files are not greater then number of the threads.\n");
	}
	
	printf("MAIN THREAD: All done (successfully read %d words with %d threads from %d files).\n",COUNT,NUMBER_OF_THREAD,num_of_files);
	pthread_exit(NULL);
	return 0;
}

