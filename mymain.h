/*
 * mymain.h
 *
 *  Created on: 2008/10/14
 *      Author: yuichi
 */

#ifndef MYMAIN_H_
#define MYMAIN_H_

#include "buf_administer.h"

//extern

extern struct buf_header buf[MAX_BUF];
extern struct buf_header hash_header[NHASH];
extern struct buf_header freelist_head;


#endif /* MYMAIN_H_ */
