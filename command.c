/*
 * command.c
 *
 *  Created on: 2008/10/14
 *      Author: yuichi
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mymain.h"
#include "buf_administer.h"
#include "command.h"


void print_help(int argc,char **ppargv){

	if(argc != 1){
		fprintf(stderr,"argument num must be 1\n");
	}
	else{
		printf("/**********************Welcom To Help********************************/\n");
		printf("\n");
		printf("$help : you can see command help.\n");
		printf("\n");
		printf("$quit : you can end this software\n");
		printf("\n");
		printf("$init : hashlist and freelist are initialized.\n");
		printf("\n");
		printf("$buf [n ...] : if there is no argument,you can see all buffer's statements.\n");
		printf("               if you specify block no,you can see this block statements.\n");
		printf("\n");
		printf("$hash [n ...] : if there is no argument,you can see all hashlist.\n");
		printf("                if you specify hash value,you can see hash list of you specify.\n");
		printf("\n");
		printf("$free : you can see freelist.\n");
		printf("\n");
		printf("$getblk n : you must specify block no.then,this software execute getblk(n).\n");
		printf("\n");
		printf("$brelse n : you must specify block no.then,this software execute brelse(n).\n");
		printf("\n");
		printf("$set n stat[stat ...] :  buffer of block no n is set stat.\n");
	    printf("                   stat(L,V,D,K,W,O) means ->     L:locked\n");
		printf("                                                  V:contain valid data\n");
		printf("                                                  D:delayed wite\n");
		printf("                                                  K:kernel read/wite\n");
		printf("                                                  W:process is waiting\n");
		printf("                                                  O:data is old\n");
		printf("\n");
		printf("$reset n stat[stat ...] :  buffer of block no n is reset stat.\n");
		printf("\n");
		printf("$kwend n : you had better use this command\n");
		printf("           when kernel has finished write back.\n");
		printf("           you must specify block no n.\n");
		printf("\n");
		printf("$krend n : you had better use this command\n");
		printf("           when kernel has finished read data from HDD.\n");
		printf("           you must specify block no n.\n");
		printf("\n");
		printf("\n");
		printf("/**********************Thank You For Seeing Me!**********************/\n");
	}
}

void
quit(int argc,char **ppargv){

	if(argc != 1){
		fprintf(stderr,"argument num must be 1\n");
	}
	else{
		printf("bye!! see you next!!\n");
		exit(0);
	}
}

void
go_getblk(int argc,char **ppargv){

	if(argc != 2){
		fprintf(stderr,"format:$getblk blkno\n");
	}
	else{
		int n = atoi(ppargv[1]);
		getblk(n);
	}
}

void
go_brelse(int argc,char **ppargv){

	if(argc != 2){
		fprintf(stderr,"format:$brelse blkno\n");
	}
	else{
		 struct buf_header *p;
		 int n = atoi(ppargv[1]);
		 p = search_hash(n);
		 if(p == NULL){
			fprintf(stderr,"no such blkno %d\n",n);
		 }
		 else{
			 brelse(p);
		 }
	}
}

void
init(int argc,char **ppargv){

	if(argc != 1){
		fprintf(stderr,"argument num must be 1\n");
	}

	else{
	int init_blkno[] = {28,4,64,17,5,97,98,50,10,3,35,99};
	int lock_flag[]  = {0,0,1,1,0,0,1,1,0,0,1,1};
	int i;
	//empty
	for(i=0; i<MAX_BUF; i++){
		buf[i].blkno = 0;
		buf[i].stat = 0x00000000;
		buf[i].hash_fp = buf[i].hash_bp = NULL;
		buf[i].free_fp = buf[i].free_bp = NULL;
	}
	//point to myself
	for(i=0; i<NHASH; i++){
		hash_header[i].hash_fp = &hash_header[i];
		hash_header[i].hash_bp = &hash_header[i];
	}
	freelist_head.free_fp = freelist_head.free_bp = &freelist_head;
	//set_init
	for(i=0; i<MAX_BUF; i++){
		set_blkno(init_blkno[i],&buf[i]);
		set_VALID(&buf[i]);
		if(lock_flag[i] == 1){
			lock(&buf[i]);
		}
		int h = hash(init_blkno[i]);
		insert_tail(&hash_header[h],&buf[i]);
	}
	//set_freelist
	insert_head_freelist(&buf[8]);
	insert_head_freelist(&buf[5]);
	insert_head_freelist(&buf[0]);
	insert_head_freelist(&buf[1]);
	insert_head_freelist(&buf[4]);
	insert_head_freelist(&buf[9]);

	}
}


void
print_buf(int argc,char **ppargv){

	int i;
	char c[STAT_NUM];

	if(argc == 1){
		for(i=0; i<MAX_BUF; i++){
			check_stat(i,c);
			if(i <= 9){
				if(buf[i].blkno <= 9){
				printf("[ %d:  %d %c%c%c%c%c%c]\n",i,buf[i].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
				printf("[ %d: %d %c%c%c%c%c%c]\n",i,buf[i].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}
			else{
				if(buf[i].blkno <= 9){
				printf("[%d:  %d %c%c%c%c%c%c]\n",i,buf[i].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
				printf("[%d: %d %c%c%c%c%c%c]\n",i,buf[i].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}
		}
	}

	else{
		int buf_num;
		for(i=1; i<argc; i++){
			buf_num = atoi(ppargv[i]);
			if(buf_num <0 || buf_num >= MAX_BUF){
				fprintf(stderr,"no such buffer NO.%d\n",buf_num);
				break;
			}
			check_stat(buf_num,c);
			if(buf_num <= 9){
				if(buf[buf_num].blkno <= 9){
					printf("[ %d:  %d %c%c%c%c%c%c]\n",buf_num,buf[buf_num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
					printf("[ %d: %d %c%c%c%c%c%c]\n",buf_num,buf[buf_num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}
			else{
				if(buf[buf_num].blkno <= 9){
					printf("[%d:  %d %c%c%c%c%c%c]\n",buf_num,buf[buf_num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
					printf("[%d: %d %c%c%c%c%c%c]\n",buf_num,buf[buf_num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}

		}
	}

}


void
print_hash(int argc,char **ppargv){

	char c[STAT_NUM];
	struct buf_header *p;
	int i,num;

	if(argc == 1){
		for(i=0; i<NHASH; i++){
			printf("%d: ",i);
			for(p=hash_header[i].hash_fp; p != &hash_header[i]; p=p->hash_fp){
				num = convert(p->blkno);
				check_stat(num,c);
				if(p->blkno <= 9){
					if(num <= 9){
						printf("[ %d:   %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
					else{
						printf("[%d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
				}
				else{
					if(num <= 9){
						printf("[ %d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
					else{
						printf("[%d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
				}
			}
			printf("\n");
		}
	}

	else{
		int j;
		for(j=1; j<argc; j++){
			i = atoi(ppargv[j]);
			if(i < 0 || i >= NHASH){
				fprintf(stderr,"No Such Hash No.%d\n",i);
				break;
			}
			printf("%d: ",i);
			for(p=hash_header[i].hash_fp; p != &hash_header[i]; p=p->hash_fp){
				num = convert(p->blkno);
				check_stat(num,c);
				if(p->blkno <= 9){
					if(num <= 9){
						printf("[ %d:   %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
					else{
						printf("[%d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
				}
				else{
					if(num <= 9){
						printf("[ %d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
					else{
						printf("[%d:  %d %c%c%c%c%c%c] "
						,num,p->blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
					}
				}
			}
			printf("\n");
		}
	}

}

void
print_free(int argc,char **ppargv){

	if(argc != 1){
		fprintf(stderr,"argument num must be 1\n");
	}
	else{
		int num;
		char c[STAT_NUM];
		struct buf_header *p;
		for(p=freelist_head.free_fp; p != &freelist_head; p=p->free_fp){
			num = convert(p->blkno);
			check_stat(num,c);
			if(num <= 9){
				if(buf[num].blkno <= 9){
				printf("[ %d:  %d %c%c%c%c%c%c]",num,buf[num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
				printf("[ %d: %d %c%c%c%c%c%c]",num,buf[num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}
			else{
				if(buf[num].blkno <= 9){
				printf("[%d:  %d %c%c%c%c%c%c]",num,buf[num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
				else{
				printf("[%d: %d %c%c%c%c%c%c]",num,buf[num].blkno,c[0],c[1],c[2],c[3],c[4],c[5]);
				}
			}
		}
		printf("\n");
	}
}

void
check_stat(int buf_num,char *stat_str){

	if((buf[buf_num].stat & STAT_OLD) != 0)stat_str[0] = 'O';
	else stat_str[0] = '-';
	if((buf[buf_num].stat & STAT_WAITED) != 0)stat_str[1] = 'W';
	else stat_str[1] = '-';
	if((buf[buf_num].stat & STAT_KRDWR) != 0)stat_str[2] = 'K';
	else stat_str[2] = '-';
	if((buf[buf_num].stat & STAT_DWR) != 0)stat_str[3] = 'D';
	else stat_str[3] = '-';
	if((buf[buf_num].stat & STAT_VALID) != 0)stat_str[4] = 'V';
	else stat_str[4] = '-';
	if((buf[buf_num].stat & STAT_LOCKED) != 0)stat_str[5] = 'L';
	else stat_str[5] = '-';
}

int
convert(int blkno){

	int buf_num;
	for(buf_num=0; buf_num<MAX_BUF; buf_num++){
		if(blkno == buf[buf_num].blkno)break;
	}
	if(buf_num == MAX_BUF){
		fprintf(stderr,"no such blkno %d\n",blkno);
		exit(-1);
	}
	return buf_num;

}

void
set_stat(int argc,char **ppargv){

	if(argc <= 2){
		fprintf(stderr,"format:$et blkno stat [stat...]\n");
	}
	else{
		int blkno = atoi(ppargv[1]);
		 struct buf_header *p;
		 p = search_hash(blkno);
		 if(p == NULL){fprintf(stderr,"no such blkno %d\n",blkno);}
		 else{
			 int i;
			 for(i=2; i<argc; i++){
				 char stat = *ppargv[i];
				 set(p,stat);
			 }
		 }
	}

}

void
set(struct buf_header *p,char stat){


	switch(stat){
		case 'L':
			lock(p);
			break;
		case 'V':
			set_VALID(p);
			break;
		case 'D':
			set_DWR(p);
			break;
		case 'K':
			set_KRDWR(p);
			break;
		case 'W':
			set_WAITED(p);
			break;
		case 'O':
			set_OLD(p);
			break;
		default:
			fprintf(stderr,"no such state %c\n",stat);
			break;
	}
}

void
reset_stat(int argc,char **ppargv){

	if(argc <= 2){
		fprintf(stderr,"format:$reset blkno stat [stat...]\n");
	}
	else{
		int blkno = atoi(ppargv[1]);
		 struct buf_header *p;
		 p = search_hash(blkno);
		 if(p == NULL){fprintf(stderr,"no such blkno %d\n",blkno);}
		 else{
			 int i;
			 for(i=2; i<argc; i++){
				 char stat = *ppargv[i];
				 reset(p,stat);
			 }
		 }
	}

}
void
reset(struct buf_header *p,char stat){

	switch(stat){
		case 'L':
			unlock(p);
			break;
		case 'V':
			reset_VALID(p);
			break;
		case 'D':
			reset_DWR(p);
			break;
		case 'K':
			reset_KRDWR(p);
			break;
		case 'W':
			reset_WAITED(p);
			break;
		case 'O':
			reset_OLD(p);
			break;
		default:
			printf("no such state %c\n",stat);
			break;
	}
}


void
set_Argument(char *command,int *pargc,char **ppargv){

	int i = 0;
	while(command[i] != '\n'){

		while( (command[i] == ' ') || (command[i] == '\t') ){
		i++;}
		*ppargv = &command[i];
		while( (command[i] != ' ') && (command[i] != '\t') && (command[i] != '\n')){
			i++;}
		(*pargc)++;
		if(command[i] == '\n'){command[i] = '\0';break;}
		command[i] = '\0';
		i++;
		ppargv++;

	}

}

void
kwend(int argc,char **ppargv){

	if(argc != 2){
		fprintf(stderr,"format: $kwend blkno\n");
	}
	else{
		struct buf_header *p;
		int blkno = atoi(ppargv[1]);
		p = search_hash(blkno);
		if(p == NULL){
			fprintf(stderr,"No Such blkno %d\n",blkno);
		}
		else{
			reset_KRDWR(p);
		}

	}

}


void
krend(int argc,char **ppargv){

	if(argc != 2){
		fprintf(stderr,"format: $kwend blkno\n");
	}
	else{
		struct buf_header *p;
		int blkno = atoi(ppargv[1]);
		p = search_hash(blkno);
		if(p == NULL){
			fprintf(stderr,"No Such blkno %d\n",blkno);
		}
		else{
			reset_KRDWR(p);
			set_VALID(p);
		}

	}

}


