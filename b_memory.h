
/* ====================================================
#   Copyright (C)2017 All rights reserved.
#
#   Author        : bbxytl
#   Email         : bbxytl@gmail.com
#   File Name     : b_memory.h.h
#   Last Modified : 2017-05-25 19:29
#   Describe      :
#
#   Log           :
#
# ====================================================*/

#ifndef  _B_MEMORY_H
#define  _B_MEMORY_H
#include <stdio.h>
#include <stdlib.h>
// #include <memory.h>

void b_memset(char* p, unsigned int len, char c){
	for(unsigned int i = 0; i < len; ++i){
		p[i] = c;
	}
}

#endif // _B_MEMORY.H_H


