
/* ====================================================
#   Copyright (C)2017 All rights reserved.
#
#   Author        : bbxytl
#   Email         : bbxytl@gmail.com
#   File Name     : bit_array.h.h
#   Last Modified : 2017-05-25 14:24
#   Describe      :
#
#   Log           :
#
# ====================================================*/

#ifndef  _B_BIT_ARRAY_H
#define  _B_BIT_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
typedef struct bit_array_t{
	unsigned int s;		// 分配的 bit 位数
	unsigned int* arr;
}bit_array;

bit_array *b_bit_array_malloc(unsigned int size)
{
	if(size < 1) return 0;
	bit_array* barr = (bit_array*) malloc(sizeof(bit_array));
	barr->s = size;
	barr->arr = (unsigned int*)malloc((sizeof(unsigned int))*(size/(sizeof(unsigned int))+1));
	memset(barr->arr,0,barr->s);
	return barr;
}
void b_bit_array_free(bit_array* barr)
{
	free(barr->arr);
	free(barr);
}
// 下标从 0 开始 ，访问出错，返回-1
int b_bit_array_get_bit(bit_array* barr, unsigned int i)
{
	if(i >= barr->s)return -1;
	return barr->arr[i/(sizeof(unsigned int))]&(1ll<<(i%(sizeof(unsigned int)))) ? 1:0;
}
int b_bit_array_set_true(bit_array* barr, unsigned int i)
{
	if(i >= barr->s)return 0;
	barr->arr[i/(sizeof(unsigned int))] |= (1ll<<(i%(sizeof(unsigned int))));
	return 1;
}

int b_bit_array_set_false(bit_array* barr, unsigned int i)
{
	if(i >= barr->s)return 0;
	barr->arr[i/(sizeof(unsigned int))] &= (~(1ll<<(i%(sizeof(unsigned int)))));
	return 1;
}

int b_bit_array_nega_bit(bit_array* barr, unsigned int i)
{
	if(i >= barr->s)return 0;
	int b = b_bit_array_get_bit(barr, i);
	if(b == -1)return 0;
	if(b == 0) b_bit_array_set_true(barr, i);
	if(b == 1) b_bit_array_set_false(barr, i);
	return 1;
}


#endif // _B_BIT_ARRAY_H



