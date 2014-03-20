/*
 * Без имени.c
 * 
 * Copyright 2014 Lex <lex22@ubuntu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */




#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>


int main (int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "ERROR \n");
	}
	int fd[2];
	pipe(fd);//links fd[0] with reading, fd[1] to write
	int pid = fork();//pid - process identificator
	if(pid == 0){//kind
		close(fd[0]);
		close(STDOUT_FILENO);// eat apple)
		//int new_fd = dup(old_fd);//dublicate old_fd to new_fd(don't change such as dup2)
		//int new_fd = dup2(old_fd, new_fd)//dublicate 1 to 3(close 1 and copy 3 to 1)
		if(dup2(fd[1], STDOUT_FILENO) == -1){
			perror("");
		}
		close(fd[1]);
		if(execvp(argv[1], argv + 1) == -1){// swap memory one process by another
			perror("");
		}
		exit(0);
	}
	else{//parent
		if(pid == -1){
			printf("error \n");
		}
		close(fd[1]);
		FILE *f = fdopen(fd[0], "r");
		int lines = 0;
		if(f == 0){
			perror("");
		}
		while(1){
			int ch = getc(f);// read by symbols
			if(feof(f)){// f end of file. return 1 if error or end of file
				break;
			}
			putchar(ch);
			if(ch == '\n'){
				 ++lines;
			 }				
		}
		fclose(f);
		close(fd[0]);		
		printf("Count of lines: %d\n", lines);	
	}
	return 0;
}
