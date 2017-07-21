/*
 * command.h
 *
 *  Created on: 2008/10/14
 *      Author: yuichi
 */

#ifndef COMMNAD_H_
#define COMMNAD_H_
#include "buf_administer.h"

void print_help(int argc,char **ppargv);
void init(int argc,char **ppargv);
void print_buf(int argc,char **ppargv);
void print_hash(int argc,char **ppargv);
void print_free(int argc,char **ppargv);
void set_stat(int argc,char **ppargv);
void reset_stat(int argc,char **ppargv);
void quit(int argc,char **ppargv);
void go_getblk(int argc,char **ppargv);
void go_brelse(int argc,char **ppargv);
void kwend(int argc,char **ppargv);
void krend(int argc,char **ppargv);


void set(struct buf_header *p,char stat);
void reset(struct buf_header *p,char stat);
void check_stat(int buf_num,char *stat_str);
int convert(int blkno);  //buf_number <-> logic_blkno
void set_Argument(char *command,int *pargc,char **ppargv);


#endif /* COMMNAD_H_ */
