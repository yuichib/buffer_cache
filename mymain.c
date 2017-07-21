/*
 * mymain.c
 *
 *  Created on: 2008/10/12
 *      Author: yuichi
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "buf_administer.h"
#include "command.h"

//struct
struct command_table{
	char *command;
	void (*pf)(int,char **);
} cmd_tbl[] = {
		{"quit",quit},
		{"help",print_help},
		{"init",init},
		{"buf",print_buf},
		{"hash",print_hash},
		{"free",print_free},
		{"getblk",go_getblk},
		{"brelse",go_brelse},
		{"set",set_stat},
		{"reset",reset_stat},
		{"kwend",kwend},
		{"krend",krend},
		{NULL,NULL}//end_flag
};



//global var
struct buf_header buf[MAX_BUF];
struct buf_header hash_header[NHASH];
struct buf_header freelist_head;


int main(int argc,char *argv[]){

	char command[BUFSIZ];
	char *myargv[BUFSIZ];
	int myargc;
	struct command_table *p;


	init(1,NULL);
	while(1){//infinite loop
		//init
		myargc = 0;
		bzero(command,sizeof(command));
		bzero(myargv,sizeof(myargv));

		printf(" $");
		fgets(command,sizeof(command),stdin);

	    set_Argument(command,&myargc,myargv);

		if(command[0] != '\n'){
			 for(p=cmd_tbl;p->command; p++){
				 if(strcmp(myargv[0],p->command) == 0){
					 (*p->pf)(myargc,myargv);
					 break;
				 }
			 }
			if(p->command == NULL){
				fprintf(stderr,"%s:command no found..\n",myargv[0]);
			}
		}

	}//loop end
	return 0;

}


