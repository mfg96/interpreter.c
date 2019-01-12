// Muhammad Faraz Sohail
// commands interpreter

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 255

int readLine (int fd, char *buff,int n)
{
	int i=0, m;                          //without exceeding buff size n 
  	
	  while(i<n && (m=read(fd, &buff[i], 1))==1){
	    if(buff[i]=='\n')
	      return(i+1);
	    i++;
	  }
	  if(m == -1)    // Error
	    return(-1);
	  else
	    return(i);   // number of characters read or 0 for EOF
}

int main(int argc, char *argv[]){
	
	int x,status=0;
	char *cmds[SIZE];
	char buf[SIZE];
	int i,j;
	
    int fd=open(argv[1],O_RDONLY);
	
	//checks if there is file given or not
	if(argc!=2){
		perror("Missing Filename\n");
		exit(1);
	}
	
	//checks if the file is there or not
    if(argv[1]==NULL)
    {
        perror("Unable to open the file\n");
        exit(1);
    }
	
	while(1){
		
		x=readLine(fd,buf,SIZE);
		if(x<=0)
			break;
		
		buf[x]='\0'; //initializes the last character of buf to '\0'
		
		if(buf[0]=='#'|| buf[0]=='\0') 
						continue;
		
	    i=j=0;
	      cmds[i]=(char *) malloc(25); //memory allocation to cmds
	      while (sscanf(buf+j, "%s", cmds[i]) == 1){ 
			  
	        j += strlen(cmds[i]);
	        while (buf[j] == ' ')    // skip blanks to keep good count
	          j++;      // increase the j for pointer to argument in buf
	        i++;
	        cmds[i]=(char *) malloc(25);
	      }
	      cmds[i]=NULL;
		  
	      if(!fork())// checks if fork is not equal to 0 (not parent)
	        if(execvp(cmds[0], cmds)==-1){ //execvp executes the cmds
	          perror("child Process");
	          exit(3);
	        }
	      wait(&status); //parent waits for the child to exit
	    }
	    if(x==-1)
	      perror("Reading error ");
		
	close(fd);
	exit(0);
}
