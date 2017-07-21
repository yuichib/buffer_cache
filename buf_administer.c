/*
 * buf_administer.c
 *
 *  Created on: 2008/10/10
 *      Author: yuichi
 */

#include <stdio.h>
#include "mymain.h"
#include "buf_administer.h"


int
hash(int blkno){
	return blkno % NHASH;
}


struct buf_header *
search_hash(int blkno){

	int h;
	struct buf_header *p;

	h = hash(blkno);
	for(p=hash_header[h].hash_fp; p!=&hash_header[h]; p=p->hash_fp){
		if(p->blkno == blkno){
			return p;}
	}
	return NULL;
}


void
insert_head(struct buf_header *h,struct buf_header *p){

	p->hash_bp = h;
	p->hash_fp = h->hash_fp;
	h->hash_fp->hash_bp = p;
	h->hash_fp = p;

}

void
insert_tail(struct buf_header *h,struct buf_header *p){

	p->hash_bp = h->hash_bp;
	p->hash_fp = h;
	h->hash_bp->hash_fp = p;
	h->hash_bp = p;
}

void
remove_from_hash(struct buf_header *p){

	p->hash_bp->hash_fp = p->hash_fp;
	p->hash_fp->hash_bp = p->hash_bp;
	p->hash_fp = NULL;
	p->hash_bp = NULL;
}


void
insert_head_freelist(struct buf_header *p){

		p->free_bp = &freelist_head;
		p->free_fp = freelist_head.free_fp;
		freelist_head.free_fp->free_bp = p;
		freelist_head.free_fp = p;
}

void
insert_tail_freelist(struct buf_header *p){

		p->free_fp = &freelist_head;
		p->free_bp = freelist_head.free_bp;
		freelist_head.free_bp->free_fp = p;
		freelist_head.free_bp = p;
}

void
remove_from_freelist(struct buf_header *p){

	p->free_bp->free_fp = p->free_fp;
	p->free_fp->free_bp = p->free_bp;
	p->free_fp = NULL;
	p->free_bp = NULL;

}

//getblk START
struct buf_header *
getblk(int blkno){

	int h = hash(blkno);
	struct buf_header *p;

	while(1){
		if((p = search_hash(blkno)) != NULL){ //block found
			if((p->stat & STAT_LOCKED) != 0){
				//if locked
				/*scenario5*/
				printf("execute scenario5\n");
				set_WAITED(p);
				//sleep(event buffer becomes free);
				printf("Process goes to sleep\n");
				return NULL;
			}
			// if free
			/*scenario1*/
			printf("execute scenario1\n");
			lock(p);
			remove_from_freelist(p);
			return p;
		}

		else{                                 //block not found
			if(freelist_head.free_fp == &freelist_head){//freelist is empty
				/*scenario4*/
				printf("execute scenario4\n");
				//sleep(event any buffer becomes free);
				printf("Process goes to sleep\n");
				return NULL;
			}
			struct buf_header *free_top = freelist_head.free_fp;
			remove_from_freelist(free_top); //LRU
			if((free_top->stat & STAT_DWR) != 0 ){
				//delayed write back
				/*scenario3*/
				printf("execute scenario3\n");
				lock(free_top);
				//kernel HDD write back
				reset_DWR(free_top);
				set_KRDWR(free_top);
				set_OLD(free_top);
				continue;
			}
			/*scenario2*/
			printf("execute scenario2\n");
			lock(free_top);
			remove_from_hash(free_top);
			set_blkno(blkno,free_top);
			insert_tail(&hash_header[h],free_top);
			reset_VALID(free_top);
			//kernel HDD read
			set_KRDWR(free_top);
			return free_top;
		}
	}

}
//getblk END


//brelse START
void
brelse(struct buf_header *buffer){

	//1.wakeup();
	printf("Wakeup processes waiting for any buffer\n");
	if((buffer->stat & STAT_WAITED) != 0){
		//2.wakeup();
		printf("Wakeup processes waiting for buffer of blkno %d\n",buffer->blkno);
		reset_WAITED(buffer);
	}

	//raise_cpu_level();
	if( ((buffer->stat & STAT_VALID) != 0) && ((buffer->stat & STAT_OLD) == 0) ){
		printf("blkno:%d insert tail of freelist\n",buffer->blkno);
		insert_tail_freelist(buffer);
	}
	else{
		printf("blkno:%d insert head of freelist\n",buffer->blkno);
		insert_head_freelist(buffer);
		reset_OLD(buffer);

	}
	//lower_cpu_level();

	unlock(buffer);
}
//brelse END



/*STAT SET AND RESET*/
void
lock(struct buf_header *buffer){

	buffer->stat |= STAT_LOCKED;
}


void
unlock(struct buf_header *buffer){

	buffer->stat &= (~STAT_LOCKED);
}


void
set_blkno(int blkno,struct buf_header *buffer){
	buffer->blkno = blkno;
}

void
set_VALID(struct buf_header *buffer){
	buffer->stat |= STAT_VALID;
}

void
reset_VALID(struct buf_header *buffer){
	buffer->stat &= ~STAT_VALID;
}

void
set_DWR(struct buf_header *buffer){
	buffer->stat |= STAT_DWR;
}

void
reset_DWR(struct buf_header *buffer){
	buffer->stat &= ~STAT_DWR;
}

void
set_KRDWR(struct buf_header *buffer){
	buffer->stat |= STAT_KRDWR;
}

void
reset_KRDWR(struct buf_header *buffer){
	buffer->stat &= ~STAT_KRDWR;
}

void
set_WAITED(struct buf_header *buffer){
	buffer->stat |= STAT_WAITED;
}

void
reset_WAITED(struct buf_header *buffer){
	buffer->stat &= ~STAT_WAITED;
}

void
set_OLD(struct buf_header *buffer){
	buffer->stat |= STAT_OLD;
}

void
reset_OLD(struct buf_header *buffer){
	buffer->stat &= ~STAT_OLD;
}






