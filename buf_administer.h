/*
 * buf_administer.h
 *
 *  Created on: 2008/10/13
 *      Author: yuichi
 */

#ifndef MYHEAD_H_
#define MYHEAD_H_

//struct
struct buf_header{
	int blkno;
	struct buf_header *hash_fp;
	struct buf_header *hash_bp;
	unsigned int stat;
	struct buf_header *free_fp;
	struct buf_header *free_bp;
	char *cashe_data;
};


//define
#define NHASH 4
#define MAX_BUF 12
#define STAT_LOCKED 0x00000001
#define STAT_VALID  0x00000002
#define STAT_DWR    0x00000004
#define STAT_KRDWR  0x00000008
#define STAT_WAITED 0x00000010
#define STAT_OLD    0x00000020
#define STAT_NUM 6



//func
int hash(int blkno);
struct buf_header *search_hash(int blkno);
void insert_head(struct buf_header *h,struct buf_header *p);
void insert_tail(struct buf_header *h,struct buf_header *p);
void remove_from_hash(struct buf_header *p);
struct buf_header *getblk(int blkno);
void insert_head_freelist(struct buf_header *p);
void insert_tail_freelist(struct buf_header *p);
void remove_from_freelist(struct buf_header *p);
void brelse(struct buf_header *buffer);

//stat_func
void set_blkno(int blkno,struct buf_header *buffer);
void set_VALID(struct buf_header *buffer);
void reset_VALID(struct buf_header *buffer);
void set_DWR(struct buf_header *buffer);
void reset_DWR(struct buf_header *buffer);
void set_KRDWR(struct buf_header *buffer);
void reset_KRDWR(struct buf_header *buffer);
void set_WAITED(struct buf_header *buffer);
void reset_WAITED(struct buf_header *buffer);
void set_OLD(struct buf_header *buffer);
void reset_OLD(struct buf_header *buffer);
void lock(struct buf_header *buffer);
void unlock(struct buf_header *buffer);





#endif /* MYHEAD_H_ */
