#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>


//we need to define this earlier 
//so that compuler knows about the function declaration
char **getInput2(char *); 
char **getFirst(char *);
void trimSpaces(char *);
int getSizeCommand(char **command);
char *split_at_backslash(char *);
char **getFirst2(char *,char *);
char *firstPart5(char *);
//char *firstPart(char *);
char *getFlag(char *input);

char **history; //global variable for storing history
int history_counter_index[1000];

int main() {
	history = malloc(5000*sizeof(char *));
    char **command;
    char *input;
    int child_pid;
    int status;
    int history_counter = 0;
    int commandSize;
    printf("----------------------------------------------------------");
    printf("\n");
    while (1) {
        input = readline("<<<varun@UnixShell>>> ");
        trimSpaces(input); //it's necessary to remove the spaces from the start
        if(!input[0]){
            continue;
        }
       
        char *inputCopy = malloc(3000);
        char *inputCopy2 = malloc(3000);
        char *inputCopy3 = malloc(3000);
        char *inputCopy4 = malloc(3000);
        char *inputCopy5 = malloc(3000);
        char *inputCopy6 = malloc(3000);
        char *inputCopy7 = malloc(3000);
        strcpy(inputCopy,input);
        strcpy(inputCopy2,input);
        strcpy(inputCopy3,input);
        strcpy(inputCopy4 , input);
        strcpy(inputCopy5 , input);
        strcpy(inputCopy6 , input);
        strcpy(inputCopy7 , input);
        
        char *firstPart = firstPart5(inputCopy7);
        //printf("inputCopy %s\n",inputCopy);
        if(strcmp(firstPart,"echo")==0){
            int flagExist = doesFlagExist(inputCopy2,"-n");
            
            if(!flagExist){
                flagExist = doesFlagExist(inputCopy3,"*");
            }
        
        	if(flagExist){
                char *flag = getFlag(inputCopy4);
                command = getFirst2(inputCopy5,flag);
                
                
                trimSpaces(command[0]);
                trimSpaces(command[1]);
                trimSpaces(command[2]);
                
                if(strcmp(flag,"*")==0){
                    command[0] = "ls"; //Echo means echo*
                    command[1] = "*";
                    command[2] = "";
                    inputCopy = "Echo";
                }
        	}
        	else{
        		//flag doesn't exist
                //This is a simple echo command
        		command = getFirst(input); //splits input in 2 parts-echo and output string
        		
        	}
        }
      
    	else{
    		//This is not echo
        	command = getInput2(input);
        }

        commandSize = getSizeCommand(command); //Size of the command

        if (!command[0]) {      /* Handle empty commands */
            free(input);
            free(command);
            continue;
        }

		else if(strcmp(command[0],"exit") == 0){
			//exit out of the shell
			//int sc = getSizeCommand(command);
			int exit_status = 0;
			if(commandSize==2){
                exit_status = atoi(command[1]);
                if(exit_status == 0 && command[1] !='0'){
                    //not a number
                    printf("Not a valid argument for exit\n");
                    continue;
                }
                else{
                    //is a number

                }

			}
			else if(commandSize == 1){
				exit_status = 0;
			}
			printf("Exiting with status : ");
			printf("%d\n",exit_status );
			free(history);
			exit(exit_status);	
		}
		
		else if (strcmp(command[0], "cd") == 0) {
			//implement cd command here cause if we use it in child process 
			//then only the directory of child process is changed not parents
            history_counter_index[history_counter] = history_counter+1;
            history[history_counter++] = inputCopy; 
            
            if(commandSize > 3 || commandSize == 3 && (strcmp(command[1],"-L")!=0 && strcmp(command[1],"--help")!=0) ){
                printf("%s\n", "Command not handled or invalid command.");
                continue;
            }
            // history_counter_index[history_counter] = history_counter+1;
            // history[history_counter++] = inputCopy; 

		    if(commandSize ==2 && strcmp(command[1],"--help")==0){
                //This is a helo command
                cd_command_help();
                continue;
            }
            else if(commandSize == 3 && strcmp(command[1],"-L")==0){
	            cd_command(command[2]);
                continue;

            }
	        else if(commandSize == 2){
	           cd_command(command[1]);
               continue;
            }
            printf("Command not handled or invalid command.\n");
	        continue;
	    }
		else if(strcmp(command[0],"echo")==0){
			//printf("Echoing\n");

            history_counter_index[history_counter] = history_counter+1;
            history[history_counter++] = inputCopy; 

			trimSpaces(command[1]);
			if(commandSize == 2 && strcmp(command[1],"*")!=0 && strcmp(command[1],"-n")!=0){
                //simple echo
                //just outputs the string
                echo_command_default(command[1]);
            }
            else if(commandSize == 3 && strcmp(command[1],"-n")==0){
                //-n flag echo
                //Prints without new line
                echo_command_n(command[2]);
            }
            else if(commandSize == 3 && strcmp(command[1],"*")==0){
                //Does the job of ls
                     
            }

			// history_counter_index[history_counter] = history_counter+1;
			// history[history_counter++] = inputCopy6; 
			continue;
		}
		else if(strcmp(command[0],"pwd")==0){
            history_counter_index[history_counter] = history_counter+1;
            history[history_counter++] = inputCopy; 

            if(commandSize != 1 && strcmp(command[1],"-L")!=0 && strcmp(command[1],"--help")!=0){
                printf("%s\n", "Command not handled or invalid command.");
                continue;
            }
            if(commandSize > 3 || commandSize == 2 && (strcmp(command[1],"-L")!=0 && strcmp(command[1],"--help")!=0) ){
                printf("%s\n", "Command not handled or invalid command.");
                continue;
            }
            // history_counter_index[history_counter] = history_counter+1;
            // history[history_counter++] = inputCopy;
            if(commandSize == 2 && strcmp(command[1],"--help")==0){
                //help command for pwd
                pwd_command_help();
                continue;
            }
            else if(commandSize == 1){
			    pwd_command();
                continue;    
            }
            printf("%s\n", "Command not handled or invalid command.");
			continue;
		}
		else if(strcmp(command[0],"history")==0){
            history_counter_index[history_counter] = history_counter+1;
            history[history_counter++] = inputCopy; 

            if(commandSize == 2 && strcmp(command[1],"-c")==0){
                //clear the history
                history = "";
                history = malloc(500*sizeof(char *));
                history[0] = inputCopy;
                history_counter = 1;
                continue;
            }
            else if(commandSize == 2 && strcmp(command[1],"--help")==0){
                history_command_help();
                continue;
            }
            //else if(commandSize == 3 && strcmp(command[1],""))
            else if(commandSize>=2 && strcmp(command[1],"-c")!=0 || commandSize >2){
                printf("bash: history: wrong argument/arguments specified\n");
                continue;
            }

			history_command();

			// history_counter_index[history_counter] = history_counter+1;
			// history[history_counter++] = inputCopy; 
			continue;
		}
		//If none of the above happen then store the history now otherwise
		//in child process it won't edit
		history_counter_index[history_counter] = history_counter+1;
		history[history_counter++] = inputCopy6;

        //Fork it cause we don't want our main program to crash due to external commands
        child_pid = fork();

        if (child_pid == 0) {
           /* Never returns if the call is successful */
        	//here we need to implement the external commands
        	//Probably we will use new .c file to code that part.
        	//Commands : ls,rm,cat,mkdir,etc
        	
            /*
                Note : Specify the path of ./Exec executable file
                otherwise execvp would fail if the PWD is changed.
            */

        	char cwd[1000];
        	getcwd(cwd,sizeof(cwd));
        	char *args[]={"/home/varun/OperatingSystems/AssignmentOS/ShellImplementation/./Exec",inputCopy,cwd,NULL}; 

        	execvp(args[0],args); 
            //execvp(command[0], command);
            printf("Execvp failed somehow:(\n");
            //continue;

        } else {
        	//Waiting for the child here on
            waitpid(child_pid, &status, WUNTRACED);
        }

        free(input);
        free(command);
    }

    return 0;
}
int getSizeCommand(char **command){
	int count = 0;
	while(command[count]!='\0'){
		count++;
	}
	return count;

}

int cd_command(char *path) {

	char *ret = strstr(path,"/");

	char *new_path;
	new_path = path;
	if(ret ==NULL){
		//That means / is not there
	}
	
	ret = strstr(path,"\\");
	if(ret !=NULL){
		//That means \ is there
		new_path = split_at_backslash(path);
	}

	int return_status = chdir(new_path);
	
	if(return_status < 0){
		//Failed to change
		printf("%s : %s\n",path,"Directory not found");
	}
	else{
		//Success
		//printf("Changed successfully\n");
	}
    return return_status;
}
void cd_command_help(){
    printf("cd [dir]\n");
    printf("\t%s\n", "Change the shell working directory.");
    printf("\t%s\n\n", "Change the current directory to DIR.  The default DIR is the value of the HOME shell variable.");
    printf("\t%s\n", "Use \\ or / to specify directory within directory");
    printf("\t%s\n\n", "If the directory is not found then it shows \"Directory not found\"");
    printf("\t%s\n", "Option:");
    printf("\t %s\t%s\n", "-L" ,"force symbolic links to be followed(It is also the default)");
    printf("\t%s\n","Exit status:" );
    printf("\t%s\n","Returns 0 if the directory is changed." );


}
void pwd_command(void){
	char directory[1000];
	getcwd(directory, sizeof(directory));
    printf("%s\n", directory);
}
void pwd_command_help(){
    printf("%s\n", "pwd: pwd [-L]");
    printf("\t%s\n\n", "Print the name of the current working directory.");
    printf("\t%s\n", "Options:");
    printf("\t%s\t%s\n\n","-L" , "print the value of $PWD if it names the current working directory");
    printf("\t%s\n","By default , 'pwd' behaves as if '-L' were specified." );
    printf("\t%s\n", "Exit status:");
    printf("\t%s\n","Returns 0 unless an invalid option is given or the current directory cannot be read." );
}
void history_command_help(){
    printf("%s\n", "history: history [-c]");
    printf("\t%s\n\n", "Display the history of commands along with their line numbers.");
    printf("\t%s\n", "Options");
    printf("\t %s\t%s\n\n", "-c","clears the history list by deleting all of the entries");
    printf("\t%s\n", "Only the history of current running program is displayed.");
    printf("\t%s\n", "Exit status:");
    printf("\t%s","If it is a success then we simply jump out of the if else block but" );
    printf("%s\n", " if it fails somehow then we print appropriate error message.");
}
void echo_command_default(char *string){
	//Default echo
	printf("%s\n", string);

}
void echo_command_new(char string[100]){

	//Split at \n but it is not implemented now
    //I chose the other 2 options
	char **command = malloc(8 * sizeof(char *));
    //char *separator = "\n";
    char *parsed;
    int index = 0;
    //printf("Input %s\n",string );
    parsed = strtok(string, "\n");
    //printf("Parse 1 %s\n", parsed);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, "\n");
    }

    command[index] = NULL;

    int size = getSizeCommand(command);
    for (int i = 0; i < size; i++)
    {
    	printf("%s\n",command[i] );
    }
    
}
// void echo_command_making(char string[100]){
// 	char **command = malloc(200);

// }
void echo_command_n(char string[1000]){
	//Implement -n which doesn't adds \n at the end of line
	printf("%s", string);
}
int getIndex(char *string , char *a){
	char *position_ptr = strchr(string , a);
	int position = (position_ptr == NULL ? -1 : position_ptr - string);
	return position;
}

void history_command(){
	//prints the history(all the previous commands)
	int c = 0;
	while(history[c]!='\0'){
		printf("%d", history_counter_index[c]);
		printf(" %s\n", history[c] );
		c++;
	}
}
void printWithB(char **string){
	printWithB("Gettinf called");

	string = (char *)malloc(100);
   	char text[100] , blank[100];
   	int c = 0, d = 0;
 
   	while (text[c] != '\0')
   	{
    	if (!(string[c] == ' ' && string[c+1] == ' ')) {
  	     	blank[d] = string[c];
    	   	d++;
      	}
      	c++;
   	}
   blank[d] = '\0';
   //printf("Text after removing blanks\n%s\n", blank);
   return 0;
}
char **getFirst(char *input){
	//printf("In get first\n");
	char **command = malloc(50*sizeof(char *));
	//char *separator = " ";
    // char *parsed;
    int index = 0;

    int c = 0;
    //char first[100] ,second[100];
    char *first = (char *)malloc(2000);
    char *second = (char *)malloc(2000);

    //char *retFlag = strstr(input,"-n"); 

    while(input[c]!='\0'){
    	if(input[c] == ' '){
    		break;
    	}
    	//printf("IN loop\n");
    	first[c] = input[c];
    	c++;
    }
    first[c] = '\0';
    int t = 0;
    while(input[c]!='\0'){
    	//printf("Second loop\n");
    	second[t] = input[c];
    	c++;
    	t++;
    }
    //printf("%s\n", first);
    //printf("%s\n", second);
    if(strcmp(first,"echo")==0){
    	//printf("Equal\n");
    	command[0] = first;
    	command[1] = second;
    	return command;
    }
    return NULL;
   
}
char **getFirst2(char *input , char *flag){
    //printf("fladddd %s\n",flag );
	int index = getIndex(input,flag[0]);
    //printf("Index of flag %d\n", index);
	char **command = malloc(2000);

    char *first = (char *)malloc(2000);
    char *second = (char *)malloc(2000);
    char *third = (char *)malloc(2000);

    int c = 0;
    while(input[c]!='\0'){
    	if(input[c]==' '){
    		break;
    	}
    	first[c] = input[c];
    	c++;
    }
    first[c] ='\0'; //adding it explicitly
    
    int t = index ;
   	if(strcmp(flag,"*")==0){ //flag == '*'
   		second = "*";
   		t = index+1;
   		command[2] =""; 
   	}
   	else if(strcmp(flag,"-n")==0){  //flag == '-'
   		second[0] = '-';
   		second[1] = 'n';

   		t = index+2;
   	
	   	int tr = 0;
	   	while(input[t] != '\0'){
	   		third[tr] = input[t];
	   		tr++;
	   		t++;
	   	}

	   	command[2] = third;   	
	}
	command[0] = first;
	command[1] = second;
    //printf("First is %s\n", first);
    //printf("Seconf is %s\n", second);
    //printf("Third is %s\n", third);
	return command;
	//From where it is called just trim the whitespaces at beginning
	//return commmand storing first ,second and third but if flag is * then third won't matter


}
int doesFlagExist(char *input,char *flag){
	//Check if flag exsists ...and if not then do what getFirst does
    //printf("In doesFlagExist flag %s\n",flag );
    //printf("doesFlagExist input %s\n",input);
	char **command = getInput2(input);
	int sizeCommand = getSizeCommand(command);
    //printf("command[1] %s\n", command[1]);
	if(sizeCommand>1 && strcmp(command[1],flag)==0){
		//Flag -a exists
        //printf("hereeaswell\n");
		return 1;
	}
	return 0;
}
char *getFlag(char *input){
    //printf("In getFlag\n");
	char **command = getInput2(input);
	int sizeCommand = getSizeCommand(command);
    //printf("Size of command %d\n", sizeCommand);
	if(sizeCommand>=2){
		return command[1];
	}

}
char *firstPart5(char *input){
	//Returns what is the first part of input
	//Maybe echo,cd,pwd,etc,.
    char *separator = " ";
	char *token = strtok(input,separator);
    strtok(NULL,separator);
	//printf("First part %s\n", token);
	return token;
}
char *split_at_backslash(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = {"\\","/"};

    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }
    command[index] = NULL;
    char buffer[300];
    int size = getSizeCommand(command);
    //sprintf(buffer,"%s%s",command[0],"/");

    for (int i = 0; i < size; i++)
    {
    	strcat(buffer,command[i]);
    	strcat(buffer,"/");
    }
   
    printf("Buffer %s\n",buffer );
    return buffer;
}

char **getInput2(char *input) {
    char **command = malloc(50* sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

void trimSpaces(char *str)
{
    int index;
    index = 0;
    while(1){
    	if(str[index] == ' '){
    		index++;
    		continue;
    	}
    	else if(str[index] == '\t'){
    		index++;
    		continue;
    	}
    	else if(str[index] == '\n'){
    		index++;
    		continue;
    	}
    	break;
    }

    if(index != 0)
    {
        int i = 0;
        do{
        	str[i] = str[i+index];
            i++;
        }
        while(str[i+index] != '\0');

        str[i] = '\0'; // String should be null terminated
    }
} 
