#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>


#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */

// alias linked list

struct AliasNode{
    char name[100];
    char command[100];
    struct AliasNode* next;
}; 


int check_file_exits_or_not(char *matched_path){

    FILE *file = fopen(matched_path,"r");

    if(file ==NULL){
        return 0;
    }
    
    fclose(file);
    return 1;

}

char** get_path_from_terminal(char* args[]){

    //  Matched path variables
    char **matched_path;
    char **full_path;
    int i,j =0;
    // Tokenizing varibales...    
    char *get_paths = getenv("PATH"); // taken from en.cppreference.com/w/c/program/getenv 
    char **paths_array;
    int paths_array_index = 0;
    paths_array = (char **)malloc(sizeof(char)*100); 
    //printf("%s",get_paths);

    char *token = strtok(get_paths, ":");

    while ( token != NULL){
        //printf(" %s\n",token);
        paths_array[paths_array_index] = (char *)malloc(sizeof(char)*100);
        strcpy(paths_array[paths_array_index],token);
        paths_array_index++;
        token = strtok(NULL,":");
    }



    matched_path = (char **)malloc(sizeof(char)*100);
    full_path = (char **)malloc(sizeof(char)*100);
    for (int i = 0; i < paths_array_index; i++)
    {
        
        matched_path[i] = (char *)malloc(sizeof(char)*100);
        full_path[i] = (char *)malloc(sizeof(char)*100);
        strcpy(matched_path[i],paths_array[i]);
        //printf("%s\n",matched_path[i]);
        strcat(matched_path[i],"/");
        //printf("%s\n",matched_path[i]);
        strcat(matched_path[i],args[0]);
        //printf("%s\n",matched_path[i]);
        if(check_file_exits_or_not(matched_path[i])){
            strcpy(full_path[j],matched_path[i]);
            //printf("%s\n",full_path[j]);
            j++;
        }


    }
    //printf("%s\n",full_path[j-1]);
    free(matched_path);
    free(paths_array);
    //free(args);
    return full_path;

}


void execute_built_ins(char** paths, char* args[]){

    //printf("execute_built_ins function");
    pid_t child_pid;
    int status;
    

    if((child_pid = fork()) < 0){ // creating the child and checking if there is any problem
        fprintf(stderr, "Failed to fork\n");
        
        
    }
    else if(child_pid == 0){ 
        //printf("%s",*args[1]);

        execv(paths[0],args);
        //printf("%d",execv(paths[0],args));
        //printf("%u",getppid());

        fprintf(stderr, "The child is failed\n");
        exit(1);
        
        
        
    }
    else{
        int return_status;
        waitpid(child_pid,&return_status,0);
        if (return_status == 0)
        {
            printf("Everthing fine");
        }
        if (return_status == 1)
        {
            fprintf(stderr, "Child process terminated with error\n");
        }
    }
    /*if(wait(NULL) != child_pid){
        int return_status;
        waitpid
        fprintf(stderr, "Child does not finished\n");
    }*/


}

void add_alias(char *args[],struct AliasNode** head_ref){ // , struct AliasNode** head_ref

    // adding into the linked list aliasnode

    int length = 0;
    int cont_val;
    char** name_from_args;
    char** command_from_args;

    struct AliasNode* new_node = (struct AliasNode*)malloc(sizeof(struct AliasNode));


    while(args[length]!=NULL){
            length++;
    }

    //printf("%d",length);

    name_from_args = (char **)malloc(sizeof(char)*100);
    command_from_args = (char **)malloc(sizeof(char)*100);
    
    name_from_args[0] = (char *)malloc(sizeof(char)*100);
    command_from_args[0] = (char *)malloc(sizeof(char)*100);


    strcpy(name_from_args[0],args[length-1]);
    strcpy(command_from_args[0],args[1]);
    strcat(command_from_args[0]," ");
    for (int cont_val = 2; cont_val < length-1; cont_val++)
    {
        strcat(command_from_args[0],args[cont_val]);
        if (cont_val < length-1){
            strcat(command_from_args[0]," ");
        }
        else if (cont_val == length){
            continue;
        }
        
    }
    
    strcpy(new_node->name,*name_from_args);
    //printf("%s\n",new_node->name);
    strcpy(new_node->command,*command_from_args);
    //new_node->name = name_from_args[0];
    //new_node->command = command_from_args[0];
    new_node->next = (*head_ref);
    (*head_ref) = new_node;

    //char *token = strtok(get_paths, '"');
    
    //strcpy(name_from_args,args[2]);
    //strcpy(command_from_args,args[1]);
    //printf("%s\n",*name_from_args);
    //printf("%s\n",args[1]);
    //printf("%s\n",*command_from_args);
    //system("ls");
    //printf("Alias Part");

}
void remove_from_alias(struct AliasNode** head_ref,char* args[]){

    // removing item form aliasnode

    struct AliasNode *temp = *head_ref, *prev;

    if((temp != NULL) && (strcmp(temp->name,args[1]) == 0)){
        *head_ref = temp->next;
        free(temp);
        return;
    }
    while((temp != NULL) && (strcmp(temp->name,args[1]) == 0)){
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL){
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Remove from alias");
}


void print_linked_list(struct AliasNode* node){
    
    // printing items form aliasnode

    if (node == NULL){
        printf("There is no item in alias\n");
    }
    while(node != NULL){
        printf("%s %s\n",node->name,node->command);
        node = node->next;
    }
}
int search_in_linked_list(struct AliasNode* head,char* args[]){
    
    // printing items form aliasnode

    char** command_from_main;
    
    command_from_main = (char **)malloc(sizeof(char)*100);
    command_from_main[0] = (char *)malloc(sizeof(char)*100);
    strcpy(command_from_main[0],"");
    //strcat(command_from_main[0],"deneme")
    //printf("%s",command_from_main[0]);
    int i = 0;


    struct AliasNode* current = head;
    while(current != NULL){
        if (strcmp(current->name,args[0])==0)
        {
            while(current->command[i]!='\0'){
                i++;
            }


            for(int j = 1; j<i-2; j++){

                strncat(command_from_main[0],&current->command[j],1);

            }
            system(command_from_main[0]);
            free(command_from_main[0]);
            //system(current->name);
            return 1;
        }
        current = current->next;
    }

    //printf("Deneme");
    return 0;
}
// not working 
void i_o_redirection(char *args[],char** paths){

    int fd;
    pid_t child_pid;
    //child_pid = fork();
    
    

    
    
        
    if (strcmp(args[1],">")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[2], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }
        

        
        
        printf("Buraya girdi >");
    }
    else if (strcmp(args[2],">")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[3], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }
    }
    else if (strcmp(args[1],">>")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[2], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }

    }
    else if (strcmp(args[2],">>")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[3], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }
    }
    else if (strcmp(args[1],"<")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[2], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }
    }
    else if (strcmp(args[2],"<")==0){
        if ((child_pid = fork()) < 0){
            fprintf(stderr, "Failed to fork\n");
        } 
        else if (child_pid == 0){
            fd  = open(args[3], CREATE_FLAGS,CREATE_MODE);
            if (fd == -1){
                fprintf(stderr, "Failed to open file\n");
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                fprintf(stderr,"Failed to redirect standard output");
            }
            if (close(fd)==-1){
                fprintf(stderr, "Failed to close the file\n");
            }
            execv(paths[0],args);
            //return 1;
        }
        if (wait(NULL)!=child_pid)
        {
            fprintf(stderr, "Parent fails\n");
        }
    }
    /*else if ((strcmp(args[1],"<")==0) && (strcmp(args[3],">")==0) ){
        printf("Buraya girdi args1 and 3 < > ");
    }
    else if ((strcmp(args[2],"<")==0) && (strcmp(args[4],">")==0) ){
        printf("Buraya girdi args2 and 4 < >");
    }*/
        
}


void setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
        i,      /* loop index for accessing inputBuffer array */
        start,  /* index where beginning of next command parameter is */
        ct;     /* index of where to place the next parameter into args[] */
    
    ct = 0;
        
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO,inputBuffer,MAX_LINE);  

    /* 0 is the system predefined file descriptor for stdin (standard input),
       which is the user's screen in this case. inputBuffer by itself is the
       same as &inputBuffer[0], i.e. the starting address of where to store
       the command that is read, and length holds the number of characters
       read in. inputBuffer is not a null terminated C-string. */

    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */

/* the signal interrupted the read system call */
/* if the process is in the read() system call, read returns -1
  However, if this occurs, errno is set to EINTR. We can check this  value
  and disregard the -1 value */
    if ( (length < 0) && (errno != EINTR) ) {
        perror("error reading the command");
	    exit(-1);           /* terminate with error code of -1 */
    }

	//printf(">>%s<<",inputBuffer);
    for (i=0;i<length;i++){ /* examine every character in the inputBuffer */

        switch (inputBuffer[i]){
	    case ' ':
	    case '\t' :               /* argument separators */
    		if(start != -1){
                args[ct] = &inputBuffer[start];    /* set up pointer */
    		    ct++;
    		}
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
		    start = -1;
		    break;

        case '\n':                 /* should be the final char examined */
    		if (start != -1){
                args[ct] = &inputBuffer[start];     
    		    ct++;
    		}
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
		    break;

	    default :             /* some other character */
		  if (start == -1)
		    start = i;
            if (inputBuffer[i] == '&'){
		        *background  = 1;
                inputBuffer[i-1] = '\0';
		    }
	   } /* end of switch */
    }    /* end of for */
    args[ct] = NULL; /* just in case the input line was > 80 */
    
	//for (i = 0; i <= ct; i++)
	//	printf("args %d = %s\n",i,args[i]);
} /* end of setup routine */
 

int main(void)
{
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2 + 1]; /*command line arguments */
    char **paths_from_terminal;
    int args_lenght=0;
    struct AliasNode* head = NULL;
    int search_val;
    int fd;

    while (1)
    {
        
        background = 0;
        printf("myshell: ");
        fflush(NULL);
        /*setup() calls exit() when Control-D is entered */
        setup(inputBuffer, args, &background);

        while(args[args_lenght]!=NULL){
            args_lenght++;
        }
        //printf("%d",args_lenght);
        /*while(head != NULL){
            if(strcmp(head->name,args[0]) == 0){
                printf("Buraya girdi");
                head->next = NULL;
            }
            else{
                head = head->next;
            }
        }*/
        //printf("%s\n",args[2]);
        /*if(args[2] != NULL){
            paths_from_terminal = get_path_from_terminal(args);
            i_o_redirection(args,paths_from_terminal);
        }*/


        if (strcmp(args[0],"exit")==0) //checking user want to exit
        {   
            printf("See you soon.\n");
            exit(0);
        }
        else if (args[0] == NULL){  // if pressed enter
            continue;
        }
        else if((strcmp(args[0],"alias")==0) && strcmp(args[1],"-l")==0){  // checking alias -l is writed

        
            print_linked_list(head);
            
        }
        else if (strcmp(args[0],"alias")==0) // checking alias is writed
        {   

            
            add_alias(args,&head);
            
        }
        
        else if (strcmp(args[0],"unalias") == 0){ // checking unalias is writed
            remove_from_alias(&head,args);
            
        }
        
        
        else{
            
            if(search_in_linked_list(head,args)== 1){  // checking the given item is in the node
                //system(args[]);
                printf("Buraya girdi\n");
            }
            else{
                paths_from_terminal = get_path_from_terminal(args);  //executing from the execv
                //execv(paths_from_terminal[0],args);
                execute_built_ins(paths_from_terminal,args);
            }
            
            
            
        }
        
        /* the steps are:*/
        /*(1) fork a child process using fork()*/
        /*(2) the child process will invoke execv()*/
		/*(3) if background == 0, the parent will wait,*/
        //execv(paths_from_terminal[0],args);
            /*if (args_lenght-1 == 0){
                args[args_lenght] = NULL;
            }
            else{
                args[args_lenght-1] = NULL;
            }*/
                
    }
}
