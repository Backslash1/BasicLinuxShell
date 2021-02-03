#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
//#include <readline/readline.h>
#include<time.h>
#include <wchar.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


char **splitCommand(char *);//declaration of function for sake of compiler
int getSizeCommand(char **);
void mkdir_command(char ** , int );
char **splitPaths(char *);
void cat_command_n(char ** , int );
int cd_command(char *);
void cat_command_E(char **,int);
void ls_command_s();
void ls_command_default();
void ls_command_1(); 


int main(int argc, char *argv[]){

	/* Here it all begins*/

	//Now we will split the argv[1](which is input from command line--called from parent)
	char **command;
	char *inputCopy = malloc(2000);
	strcpy(inputCopy,argv[1]);
	//printf("Input copy %s\n",inputCopy);
	command = splitCommand(inputCopy); 
	//printf("CWD from argv[] %s\n",argv[2]);
	//command is now an array of strings 
	//printf("Command %s %s\n",command[0],command[1]);
	int sizeCommand = getSizeCommand(command);


	if(strcmp(command[0],"date") == 0){
		//Here we will implement date
		if(sizeCommand == 2){
			//there is some flag
			
			if(strcmp(command[1],"-u")==0){
				// -u flag ---for printing utc time
			
				date_utc();
			}
			else if(strcmp(command[1],"--date=\"tomorrow\"")==0){
				//Tomorrow's date
				date_tomorrow();
			}
		}
		else if(sizeCommand == 1){
			//Other wise print everyth	ing
			date_command();
		}
		else{
			printf("Wrong number of arguments provided or incorrect flag supplied.\n");
		}
	}

    else if(strcmp(command[0],"rm")==0){
        //Delete the file
        //Implement interactive deletion -i
        //2nd one would be -f (forcefull deletion)
        //by defualt it is forcefull

        /* Remeber that we dont delete directories with rm */

        if(strcmp(command[1],"-i")!=0 && strcmp(command[1],"-f")!=0){
            //Implement forced deletion
            int loop = sizeCommand-1;
            int start = 1;
            while(loop>0){
            	int is_directory = directory_exists(command[start]);

            	if(!is_directory){
            		//If it doesn't exist then it is a file
            		//So we need to delete it only
            		//printf("Its'a file not a directory\n");
            	    if(remove(command[start])==0){
                		//success
                	}
                	else{
                    	//failed to delete
                    	printf("%s%s%s%s\n","rm: cannot remove \"",command[start],"\" : ","No such file or directory" );
                	}   
          		
            	}
            	else if(is_directory){
            		//Its a directory not a file
            		printf("Can't delete : \"%s\". It is a directory.\n",command[start]);
            	}
            	loop--;
                start++;	

            }
        }
        else if(strcmp(command[1],"-f")==0){
          // -f flag
            int loop = sizeCommand-2;
            int start = 2;
            while(loop>0){
            	int is_directory = directory_exists(command[start]);
            	if(!is_directory){

	                if(remove(command[start])==0){
	                //success
	                }
	                else{
	                    //failed to delete
	                    printf("%s%s%s%s\n","rm: cannot remove \"",command[start],"\" : ","No such file or directory" );
	                }
	            }    
	            else if(is_directory){
            		//Its a directory not a file
            		printf("Can't delete : \"%s\". It is a directory.\n",command[start]);
            	}
     
                loop--;
                start++;
            }
        }
        else if(strcmp(command[1],"-i")==0){
            int loop = sizeCommand-2;
            int start = 2;

            while(loop>0){
            	int is_directory = directory_exists(command[start]);
            	if(!is_directory){
            		//printf("Not directory so a file\n");
	                char str[200]; 
	                printf("%s%s%s", "rm: remove file \"",command[start],"\" ?(y/n) ");
	                gets(str); 
	                //fgets(str, 200,stdin); //using fgets cause gets throws warning 
	                //printf("str gets %s\n", str)	;
	                if(strcmp(str,"y")==0 || strcmp(str,"Y")==0){
	                    //delete it
	                    int er = remove(command[start]);
	                    if(er == 0){
	                        //success
	                        //printf("Success in deletion\n");
	                    }
	                    else{
	                        printf("%s%s%s%s\n","rm: cannot remove \"",command[start],"\" : ","No such file or directory" );    
	                    }
	                }
	                else{
	                    //don't do it
	                    //skip
	                    //printf("dont delete\n");
	                } 
	            }
            	else if(is_directory){
            		//Its a directory not a file
            		printf("Can't delete : \"%s\". It is a directory.\n",command[start]);
            	}
         
                loop--;
                start++;
            }

        }
    }

	else if(strcmp(command[0],"mkdir")==0){
		//create directory
	    mkdir_command(command , sizeCommand);

	}
	else if(strcmp(command[0],"Echo") == 0){
		//This is echo *
		ls_echo_command(argv[2]);
	}
	else if(strcmp(command[0],"ls")==0){
		//Implement listing the files and directories
		if(sizeCommand == 1){
			//default implementation
			//print just the files name
			ls_command_default(argv[2]);
		}
		else if(sizeCommand==2 && strcmp(command[1],"-s")==0){
			ls_command_s(argv[2]);
		}
		else if(sizeCommand==2 && strcmp(command[1],"-1")==0){
			ls_command_1(argv[2]);
		}
		else {
			printf("Wrong number of arguments provided to %s\n","ls");
		}

	}

	else if(strcmp(command[0],"cat")==0){
		//Implement cat here
		//1st -E flag which adds $ at the place of "\n"
		//2nd -n flag which adds the line number at the starting of the line
		
		if(strcmp(command[1],"-n")==0){
			//Implementing -n via file handling
			cat_command_n(command,sizeCommand);
		}
		else if(strcmp(command[1],"-E")==0){
			//adds "$" in place of \n
			cat_command_E(command,sizeCommand);
		}
		else if(strcmp(command[1],"-n")!=0 && strcmp(command[1],"-E")!=0){
			//this is the default cat
			cat_command_default(command,sizeCommand);
		}
	}
	else{
		//No such command is there
		//printf("%d\n", sizeCommand);
		printf("%s%s\n",command[0]," command doesn't exist" );
		
	}
	//At the end
	free(command);
	free(inputCopy);
	return 0;
}
void ls_command_s(char *pointer){
	
	//implements ls -s
	//pointer contains the path that might be modified from parent

	//pointer = getenv("PWD");
	DIR *mydir = opendir((char *)pointer);
	struct dirent *myfile;
	struct stat mystat;
	char buffer[2000];
	long size = 0;
	int counter = 0;
	while(1){
		myfile = readdir(mydir);
		if(myfile == NULL){
			break;
		}
		else{
			//Not null
			if((counter+1)%9 == 0){
				//New line
				printf("\n");
			}
			sprintf(buffer,"%s/%s",pointer,myfile->d_name);
			stat(buffer,&mystat);
			if(strcmp(myfile->d_name,".")==0){
				continue;
			}
			if(strcmp(myfile->d_name,"..")==0){
				continue;
			}
			printf("%ld %s\t",mystat.st_blocks/2,myfile->d_name);
			size += mystat.st_blocks/2;
			counter++;
		}
	}
	printf("\nTotal size %ld\n", size);
	closedir(mydir);
	return;

}
void ls_echo_command(char *pointer){
	//echo * command
	DIR *mydir = opendir((char *)pointer);
	struct dirent *myfile;
	struct stat mystat;
	char buffer[2000];
	long size = 0;
	int counter = 0;
	while(1){
		myfile = readdir(mydir);
		if(myfile == NULL){
			break;
		}
		else{
			//Not null
			
			sprintf(buffer,"%s/%s",pointer,myfile->d_name);
			stat(buffer,&mystat);
			if(strcmp(myfile->d_name,".")==0){
				continue;
			}
			if(strcmp(myfile->d_name,"..")==0){
				continue;
			}
			printf("%s ",myfile->d_name);
			size += mystat.st_blocks/2;
			counter++;
		}
	}
	printf("\n");
	closedir(mydir);
	return;	
}
void ls_command_default(char *pointer){
	
	//implements default ls 

	DIR *mydir = opendir((char *)pointer);
	struct dirent *myfile;
	struct stat mystat;
	char buffer[2000];
	long size = 0;
	int counter = 0;
	while(1){
		myfile = readdir(mydir);
		if(myfile == NULL){
			break;
		}
		else{
			//Not null
			if((counter+1)%9 == 0){
				//New line after 10 prints
				printf("\n");
			}
			sprintf(buffer,"%s/%s",pointer,myfile->d_name);
			stat(buffer,&mystat);
			if(strcmp(myfile->d_name,".")==0){
				continue;
			}
			if(strcmp(myfile->d_name,"..")==0){
				continue;
			}
			printf("%s\t",myfile->d_name);
			size += mystat.st_blocks/2;
			counter++;
		}
	}
	printf("\n");
	closedir(mydir);
	return;

}
void ls_command_1(char *pointer){
	
	//implements default ls -1 

	//char *pointer;
	//pointer = getenv("PWD");
	DIR *mydir = opendir((char *)pointer);
	struct dirent *myfile;
	struct stat mystat;
	char buffer[2000];
	long size = 0;
	int counter = 0;
	while(1){
		myfile = readdir(mydir);
		if(myfile == NULL){
			break;
		}
		else{
			//Not null
			sprintf(buffer,"%s/%s",pointer,myfile->d_name);
			stat(buffer,&mystat);
			if(strcmp(myfile->d_name,".")==0){
				continue;
			}
			if(strcmp(myfile->d_name,"..")==0){
				continue;
			}
			printf("%s\n",myfile->d_name);
			size += mystat.st_blocks/2;
			counter++;
		}
	}
	closedir(mydir);
	return;

}

void cat_command_default(char **command , int sizeCommand ){
	//From command[2] onwards starts the name of files
	int start = 1;
	int loop = sizeCommand - 1;

	while(loop>0){
		//Loop over all the files
	
		FILE *fp = fopen(command[start],"r");
		if(fp == NULL){
			//File doesn't exists
			printf("%s%s%s\n","File : \"",command[start],"\" doesn't exist." );
			start++;
			loop--;
			continue;
		}
		char *line ;
		size_t line_capacity = 0; 

		while( getline(&line,&line_capacity,fp ) > 0 ) {
		    
		    line[strlen(line)-1] = '\0';
		    printf("%s\n", line);
		}
		fclose(fp);
		start++;
		loop--;
	}

}
void cat_command_n(char **command , int sizeCommand){
	//From command[2] onwards starts the name of files
	int start = 2;
	int loop = sizeCommand - 2;
	int line_counter = 1;

	while(loop>0){
		//Loop over all the files
	
		FILE *fp = fopen(command[start],"r");
		if(fp == NULL){
			//File doesn't exists
			printf("%s%s%s\n","File : \"",command[start],"\" doesn't exist." );
			start++;
			loop--;
			continue;
		}
		char *line ;
		size_t line_capacity = 0; 

		while( getline(&line,&line_capacity,fp ) > 0 ) {
		    
		    line[strlen(line)-1] = '\0';
		    printf("\t%d %s\n",line_counter, line);
			line_counter++;
		}
		fclose(fp);
		start++;
		loop--;
	}

}
void cat_command_E(char **command , int sizeCommand){
	//From command[2] onwards starts the name of files
	int start = 2;
	int loop = sizeCommand - 2;
	int line_counter = 1;
	
	while(loop>0){
		//Loop over all the files
		
		FILE *fp = fopen(command[start],"r");
		if(fp == NULL){
			//File doesn't exists
			printf("%s%s%s\n","File : \"",command[start],"\" doesn't exist." );
			start++;
			loop--;
			continue;
		}
		char *line ;
		size_t line_capacity = 0; 

		while( getline(&line,&line_capacity,fp ) > 0 ) {
		    
		    line[strlen(line)-1] = '\0';
		    printf("%s%s\n", line,"$");
			line_counter++;
		}
		fclose(fp);
		start++;
		loop--;
	}

}

void mkdir_command(char **command , int totCommands){
	//mkdir
	//Handles -v
	//And other is -p
	
	if(strcmp(command[1],"-v")!=0 && strcmp(command[1],"-p")!=0){
		//Default
		int start = 1;
		int loop = totCommands-1;
		while(loop>0){
			//printf("start %s\n", command[start]);
			int status = mkdir(command[start],0777);
			if(!status){
				//Created
			}
			else{
				printf("Unable to create directory : \"%s%s\n", command[start],"\"");
			}
			loop--;
			start++;
		}	
	}
	else if(strcmp(command[1],"-v")==0){
		// -v flag implementation
		int start = 2;
		int loop = totCommands-2;
		while(loop>0){
			int status = mkdir(command[start],0777);
			if(!status){
				//created
				printf("%s%s%s\n","mkdir : created directory '" ,command[start],"'");
			}
			else{
				printf("Unable to create directory : \"%s%s\n", command[start],"\"");
			}
			loop--;
			start++;
		}
	}
	else if(strcmp(command[1],"-p")==0){
		//mkdir -p h1/h2/h3 -- creates directory within directory 
		//And if it doesnt exist then it creates it first
		int start = 0;
		char **sage_paths = splitPaths(command[2]);
		int loop = getSizeCommand(sage_paths);
		
		while(loop>0){
			//directory order is in comand[2] 
		
			int ret_status = directory_exists(sage_paths[start]);
		
			if(ret_status){
				//that directory exists 
				cd_command(sage_paths[start]);
			}
			else if(!ret_status){
				//make directory//and then change the directory to made directory so that 
				//we can make a directory in it
				mkdir(sage_paths[start],0777);
				cd_command(sage_paths[start]);
			}
			start++;
			loop--;
		}
	}
}
int directory_exists(char *name){
	DIR* dir = opendir(name);
	//printf("DIr %d\n", dir);
	if (dir) {
	    /* Directory exists. */
	    //printf("The directory_exists\n");
	    closedir(dir);
	} else if (ENOENT == errno) {
		//printf("Doesnt exist\n");
	    /* Directory does not exist. */
	} else {

	    /*failed due to other reason. */
	}
	return dir;

}
int cd_command(char *path) {
	int return_status = chdir(path);
	
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
char **splitPaths(char *path){
	char **command = malloc(8 * sizeof(char *));
    char *separator = "/";
    char *token;
    int index = 0;

    token = strtok(path, separator);
    while (token != NULL) {
        command[index] = token;
        index++;

        token = strtok(NULL, separator);
    }
    command[index] = NULL;
    return command;

}

void date_tomorrow(){
	char cur_time[130];
  
	time_t t;
	struct tm* pTime;
	  
	t = time(NULL);
	pTime = localtime(&t);	  
	//printf("%d",pTime->tm_sec);
	pTime->tm_mday = pTime->tm_mday+1; //change date to 
	//printf("Next day %d\n", pTime->tm_mday);   
  	strftime(cur_time, 130, "%d-%b-%Y %H:%S:%M", pTime);
  
  	printf("%s\n", cur_time);
  	return ;
}
void date_command(void){
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("%s", asctime(tm));
    return;
}
void date_utc(){
	time_t now = time(&now);
    
    if (now == -1) {
        //failed
    }
        
    struct tm *pTime = gmtime(&now);
    if (pTime == NULL) { 
        puts("The gmtime() function failed");
    }    
    printf("UTC time: %s", asctime(pTime));

}

int getSizeCommand(char **command){
	int count = 0;
	while(command[count]!='\0'){
		count++;
	}
	return count;

}
char **splitCommand(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *token;
    int index = 0;

    token = strtok(input, separator);
    while (token != NULL) {
        command[index] = token;
        index++;

        token = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;

}