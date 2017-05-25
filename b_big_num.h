
/* ====================================================
#   Copyright (C)2017 All rights reserved.
#
#   Author        : bbxytl
#   Email         : bbxytl@gmail.com
#   File Name     : b_big_num.h
#   Last Modified : 2017-05-25 14:36
#   Describe      :
#
#   Log           :
#
# ====================================================*/

#ifndef  _B_BIG_NUM_H
#define  _B_BIG_NUM_H

#include "b_memory.h"

#define SCANF_DEFAULT_LEN 10
#define DEFAULT_ALLOC_LEN  SCANF_DEFAULT_LEN

#define COMPARE_LT -1
#define COMPARE_GT 1
#define COMPARE_EQ 0
#define COMPARE_ER -99

#define B_POSITIVE  1
#define B_NEGATIVE  -1

typedef struct big_num_s{
	unsigned int alloc_len; // malloc 长度
	unsigned int len;		// num 数组长度
	int type;		// 正负
	char* data;
}big_num_t;


#define B_FROMAT_CHAR_2_NUM(x) (((x) == 0  || (x) < '0' || (x) > '9' )? 0 : (x)-'0')
#define B_FROMAT_NUM_2_CHAR(x) ((x)+'0')
void b_big_num_dump(big_num_t* a);
big_num_t* b_big_num_add(big_num_t* a, big_num_t*b, big_num_t* r);
big_num_t* b_big_num_sub(big_num_t* a, big_num_t*b, big_num_t* r);
big_num_t* b_big_num_mul(big_num_t* a, big_num_t*b, big_num_t* r);
big_num_t* b_big_num_div(big_num_t* a, big_num_t*b, big_num_t* r, big_num_t* l);

// 判断指针是否无效
unsigned int b_big_num_is_null_ptr(big_num_t* big){
	if(!big)return 1;
	if(!(big->data))return 1;
	return 0;
}

// 根据分配大小和类型生成 big 数组
big_num_t* b_big_num_malloc(unsigned int alloc_len){
	if(!(alloc_len))return 0;
	big_num_t* big = (big_num_t*)malloc(sizeof(big_num_t));
	if(!big)return 0;
	big->alloc_len = alloc_len;
	big->len = 0;
	big->type = B_POSITIVE;
	big->data = (char*)malloc(big->alloc_len * sizeof(char));
	if(!big->data)return 0;
	b_memset(big->data, big->alloc_len, 0);
	return big;
}
// 重新分配内存大小，默认增长 默认 10 个长度, 长度传 0 使用默认增长 10
unsigned int b_big_num_remalloc(big_num_t* a, unsigned int alloc_len){
	if(b_big_num_is_null_ptr(a))return 0;
	if(a->alloc_len > alloc_len){
		alloc_len = a->alloc_len + DEFAULT_ALLOC_LEN;
	}
	a->alloc_len = alloc_len;
	char* t = (char*)malloc(a->alloc_len * sizeof(char));
	if(!t)return 0;
	b_memset(t, a->alloc_len, 0);
	for(unsigned int i = 0; i < a->len; ++i){
		t[i] = a->data[i];
	}
	free(a->data);
	a->data = t;
	return 1;
}

void b_big_num_free(big_num_t* big){
	if(b_big_num_is_null_ptr(big))return;
	free(big->data);
	big->data = 0;
	free(big);
	big = 0;
}

// 比较两个大数
// absolute: 只比较其绝对值
// a > b  : 返回 1
// a == b : 返回 0
// a < b  : 返回 -1
int b_big_num_compare(big_num_t* a, big_num_t* b, unsigned int absolute){
	if(b_big_num_is_null_ptr(a))return COMPARE_ER;
	if(b_big_num_is_null_ptr(b))return COMPARE_ER;
	int f = COMPARE_EQ;
	if(a->len > b->len)f = COMPARE_GT;
	else if(a->len < b->len)f = COMPARE_LT;
	else{
		for(unsigned int i = a->len; i > 0; --i){
			if(a->data[i-1] == b->data[i-1])continue;
			if(a->data[i-1] < b->data[i-1])f = COMPARE_LT;
			else f = COMPARE_GT;
			break;
		}
	}
	if(absolute)return f;
	if(a->type == B_POSITIVE){
		if(b->type == B_POSITIVE)return f;
		else return COMPARE_GT;
	}else {
		if(b->type == B_POSITIVE)return COMPARE_LT;
		else return -f;
	}
}

// 取两个大数中大的那个
big_num_t* b_big_num_max(big_num_t* a, big_num_t*b, unsigned int absolute){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	int f = b_big_num_compare(a,b,absolute);
	if(f == COMPARE_GT)return a;
	if(f == COMPARE_LT)return b;
	return a;
}
// 取两个大数中小的那个
big_num_t* b_big_num_min(big_num_t* a, big_num_t*b, unsigned int absolute){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	int f = b_big_num_compare(a,b,absolute);
	if(f == COMPARE_GT)return b;
	if(f == COMPARE_LT)return a;
	return a;
}

// 反转存储
static void b_big_num_reverse(big_num_t* big){
	if(b_big_num_is_null_ptr(big))return;
	for(unsigned int i = 0, j = big->len - 1; i < j; ++i,--j){
		char t = big->data[i];
		big->data[i] = big->data[j];
		big->data[j] = t;
	}
}

// max_len 传入要输入的大数位数
// 返回值：输入的大数字符串
big_num_t* b_big_num_scanf(unsigned int max_len){
	if(!(max_len))max_len = SCANF_DEFAULT_LEN;
	big_num_t* big = b_big_num_malloc(max_len+1);
	if(b_big_num_is_null_ptr(big))return 0;
	unsigned int idx = 0;
	char n;
	scanf("%c", &n);
	if(n == '-' || n == '+'){
		big->type = (n == '-') ? B_NEGATIVE : B_POSITIVE;
		scanf("%c", &n);
	}else if(n >= '0' && n <= '9'){
		big->type = B_POSITIVE;
	}
	while(n >= '0' && n <= '9'){
		big->data[idx++] = n;
		if(idx >= big->alloc_len)break;
		scanf("%c", &n);
	}
	big->len = idx;
	b_big_num_reverse(big);
	return big;
}
// 将 a 的数据 copy 到 b , 不会重写 b 的 alloc_len
// 失败返回 0
unsigned int b_big_num_copy(big_num_t* a, big_num_t* b){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	if(b->alloc_len < a->len)return 0;
	b->type = a->type;
	b->len = a->len;
	for(unsigned int i = 0; i < a->len; ++i){
		b->data[i] = a->data[i];
	}
	return 1;
}

big_num_t* b_big_num_init_from_int(big_num_t* a, int num){
	unsigned int len = 0;
	unsigned int type = num >= 0 ? B_POSITIVE : B_NEGATIVE;
	num = type ? num : 0-num;
	int n= num;
	while(n){
		n /= 10;
		++len;
	}
	big_num_t* big = a;
	if(b_big_num_is_null_ptr(big)){
		big = b_big_num_malloc(len+1);
		if(b_big_num_is_null_ptr(big))return 0;
	}else{
		b_memset(big->data, big->alloc_len, 0);
		// for(unsigned int i = 0; i < big->alloc_len; ++i){
			// big->data[i] = '0';
		// }
	printf("\n\nbig: ");
	b_big_num_dump(big);
	}
	big->type = type;
	n= num;
	len = 0;
	while(n){
		big->data[len++] = n % 10 + '0';
		n /= 10;
	}
	big->len = len;
	return big;
}

big_num_t* b_big_num_init_from_big(big_num_t* src, big_num_t* to){
	if(b_big_num_is_null_ptr(src))return 0;
	big_num_t* big = to;
	if(b_big_num_is_null_ptr(big)){
		big = b_big_num_malloc(src->alloc_len);
		to = big;
	}
	if(b_big_num_is_null_ptr(big))return 0;
	if(!b_big_num_copy(src, big))return 0;
	return big;
}

// 输出 大数的数据部分，包括其正负标志
// f :0 , 不输出正数前的 + 标示
// f : 非 0 ，输出正数前的 + 标示
// 如果 big 表示 负数，则 - 号永远输出
void b_big_num_print(big_num_t* big, unsigned int f){
	if(b_big_num_is_null_ptr(big))return;
	if(big->type == B_NEGATIVE){
		printf("-");
	}else if(f){
		printf("+");
	}
	unsigned int len = 0;
	while(len < big->len && big->data[len]) ++len;
	for(unsigned int i = 0; i < len; ++i){
		printf("%d",B_FROMAT_CHAR_2_NUM(big->data[len - i - 1]));
	}
	printf("\n");
}

// 完整的 dump 出大数的数据机构
void b_big_num_dump(big_num_t* a){
	if(b_big_num_is_null_ptr(a))return;
	printf("dump big_num_t:\n");
	printf("alloc_len:\t%d\n",a->alloc_len);
	printf("type:\t%d\n",a->type);
	printf("len:\t%d\n",a->len);
	printf("data:\t");
	b_big_num_print(a,1);
	printf("\n");
}

big_num_t* b_big_num_add(big_num_t* a, big_num_t*b, big_num_t* r){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	big_num_t* c = 0;
	if(a->type == b->type && a->type == B_NEGATIVE){
		a->type = b->type = B_POSITIVE;
		c = b_big_num_add(a,b,r);
		a->type = b->type = B_NEGATIVE;
		c->type = B_NEGATIVE;
		return c;
	}
	if(a->type != b->type){
		if(a->type == B_NEGATIVE){
			a->type = B_POSITIVE;
			c = b_big_num_sub(b,a,r);
			if(a != c) a->type = B_NEGATIVE;
			return c;
		}else{
			b->type = B_POSITIVE;
			c = b_big_num_sub(a,b,r);
			if(b != c) b->type = B_NEGATIVE;
			return c;
		}
	}
	// a,b 都是正数的情况
	if(!b_big_num_is_null_ptr(r)){
		c = r;
	}else{
		unsigned int c_size = a->alloc_len > b->alloc_len ? a->alloc_len : b->alloc_len;
		c = b_big_num_malloc(c_size + 1);
	}
	if(b_big_num_is_null_ptr(c))return 0;
	c->type = B_POSITIVE;
	// 计算部分
	big_num_t* p = a;
	big_num_t* q = b;
	if(a->len < b->len){
		p = b;
		q = a;
	}
	// 检查内存是否足够
	if(c->alloc_len < p->len){
		b_big_num_remalloc(c,0);
	}
	// 根据类型计算
	unsigned int i = 0, n = 0, carry = 0;
	for(; i < q->len; ++i){
		n = B_FROMAT_CHAR_2_NUM(p->data[i]) + B_FROMAT_CHAR_2_NUM(q->data[i]) + carry;
		carry = n / 10;
		c->data[i] = B_FROMAT_NUM_2_CHAR(n%10);
	}
	for(; i < p->len; ++i){
		n = B_FROMAT_CHAR_2_NUM(p->data[i]) + carry;
		carry = n / 10;
		c->data[i] = B_FROMAT_NUM_2_CHAR(n%10);
	}
	if(carry){
		c->data[i++] = B_FROMAT_NUM_2_CHAR(carry);
	}
	c->len = i;
	return c;
}

big_num_t* b_big_num_sub(big_num_t* a, big_num_t* b, big_num_t* r){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	big_num_t* c = 0;
	if(a->type == B_POSITIVE){
		if(b->type == B_NEGATIVE){
			b->type = B_POSITIVE;
			c = b_big_num_add(a,b,r);
			if(b != c) b->type = B_NEGATIVE;
			return c;
		}else if(b_big_num_compare(a,b,1) == COMPARE_LT){
			c = b_big_num_sub(b,a,r);
			c->type = c->type == B_POSITIVE ? B_NEGATIVE : B_POSITIVE;
			return c;
		}
	}else{
		if(b->type == B_NEGATIVE){
			a->type = b->type = B_POSITIVE;
			c = b_big_num_sub(b,a,r);
			unsigned int type = c->type;
			a->type = b->type = B_NEGATIVE;
			c->type = type;
			return c;
		}else{
			a->type = B_POSITIVE;
			c = b_big_num_add(a,b,r);
			unsigned int type = c->type;
			a->type = B_NEGATIVE;
			c->type = type == B_POSITIVE ? B_NEGATIVE : B_POSITIVE;
			return c;
		}
	}
	if(a->len < b->len){
		c = b_big_num_sub(b,a,r);
		c->type = c->type == B_POSITIVE ? B_NEGATIVE : B_POSITIVE;
		return c;
	}
	// 正数 - 正数
	if(!b_big_num_is_null_ptr(r)){
		c = r;
	}else{
		unsigned int c_size = a->alloc_len > b->alloc_len ? a->alloc_len : b->alloc_len;
		c = b_big_num_malloc(c_size + 1);
	}
	if(b_big_num_is_null_ptr(c))return 0;
	c->type = B_POSITIVE;
	// 计算部分
	if(c->alloc_len < (a->len > b->len ? a->len : b->len)){
		if(!b_big_num_remalloc(c,0))return 0;
	}
	// 根据类型计算
	unsigned int i = 0, borrow = 0;
	int n = 0;
	for(; i < b->len; ++i){
		n = B_FROMAT_CHAR_2_NUM(a->data[i]) - B_FROMAT_CHAR_2_NUM(b->data[i]) - borrow;
		if(n < 0){
			n += 10;
			borrow = 1;
		}else borrow = 0;
		c->data[i] = B_FROMAT_NUM_2_CHAR(n);
	}
	for(; i < a->len; ++i){
		n = B_FROMAT_CHAR_2_NUM(a->data[i]) - borrow;
		if(n < 0){
			n += 10;
			borrow = 1;
		}else borrow = 0;
		c->data[i] = B_FROMAT_NUM_2_CHAR(n);
	}
	for(i = a->len -1; i > 0; --i){
		if(c->data[i] != '0')break;
	}
	c->len = ++i;
	return c;
}
// 左移一位，相当于乘以10，返回最高位
unsigned int b_big_num_move_left(big_num_t* a){
	if(b_big_num_is_null_ptr(a))return 0;
	a->len += 1;
	if(a->len > a->alloc_len){
		b_big_num_remalloc(a,0);
	}
	for(unsigned int i = a->len; i > 0; --i){
		a->data[i] = a->data[i-1];
	}
	a->data[0] = '0'; // 最低位 补 0
	return B_FROMAT_CHAR_2_NUM(a->data[a->len-1]);
}

// 右移一位，相当于除以10，返回除以10后的余数
unsigned int b_big_num_move_right(big_num_t* a){
	if(b_big_num_is_null_ptr(a))return 0;
	char c = a->data[0];
	for(unsigned int i = 0; i < a->len - 1; ++i){
		a->data[i] = a->data[i+1];
	}
	a->len -= 1;
	return B_FROMAT_CHAR_2_NUM(c);
}

big_num_t* b_big_num_mul(big_num_t* a, big_num_t*b, big_num_t* r){
	if(b_big_num_is_null_ptr(a))return 0;
	if(b_big_num_is_null_ptr(b))return 0;
	int atype = a->type;
	int btype = b->type;
	big_num_t* c = 0;
	big_num_t* t = 0;
	big_num_t* p = a;
	big_num_t* q = b;
	// 正数相乘
	if(!b_big_num_is_null_ptr(r)){
		if(r == a || r == b){
			t = b_big_num_init_from_big(r, t);
			if(r == a) p = t;
			else q = t;
		}
		c = b_big_num_init_from_int(r, 0);
	}else{
		unsigned int c_size = a->alloc_len + b->alloc_len;
		c = b_big_num_malloc(c_size + 1);
	}
	if(b_big_num_is_null_ptr(c))return 0;
	if(c->alloc_len < (p->len + q->len + 1)){
		b_big_num_remalloc(c,0);
	}
	c->len = c->alloc_len;
	printf("\n\np: ");
	b_big_num_dump(p);
	printf("q: ");
	b_big_num_dump(q);
	printf("c: ");
	b_big_num_dump(c);
	// 计算部分
	if(p->len < q->len){
		big_num_t* pq = p;
		p = q;
		q = pq;
	}
	// 根据类型计算
	unsigned int i = 0, j = 0, k = 0, n = 0, carry = 0;
	for(i = 0; i < q->len;++i){
		carry = 0;
		for(j = 0; j < p->len; ++j){
			k = i+j;
			n = B_FROMAT_CHAR_2_NUM(p->data[j]) * B_FROMAT_CHAR_2_NUM(q->data[i]) + carry + B_FROMAT_CHAR_2_NUM(c->data[k]);
			carry = n / 10;
			c->data[k] = B_FROMAT_NUM_2_CHAR(n%10);
		}
		if(carry) c->data[++k] = B_FROMAT_NUM_2_CHAR(carry);
		c->len = ++k;
	}
	b_big_num_free(t);
	c->type = atype == btype ? B_POSITIVE : B_NEGATIVE;
	return c;
}

// 判断一个大数是否为 0
unsigned int b_big_num_is_zero(big_num_t* a){
	if(b_big_num_is_null_ptr(a))return 1;
	for(unsigned int i = 0; i < a->len; ++i){
		if(B_FROMAT_CHAR_2_NUM(a->data[i]))return 0;
	}
	return 1;
}

unsigned int b_big_num_add_int(big_num_t*a, unsigned int n){
	if(b_big_num_is_null_ptr(a))return 0;

	return 1;
}

unsigned int b_big_num_sub_int(big_num_t*a, unsigned int n){
	if(b_big_num_is_null_ptr(a))return 0;
	return 1;
}
/*
big_num_t* b_big_num_div(big_num_t* a, big_num_t*b, big_num_t* r, big_num_t* l){
	big_num_t* c = 0;
	big_num_t* d = 0;
	if(a->type != b->type){
		int atype = a->type;
		int btype = b->type;
		a->type = B_POSITIVE;
		b->type = B_POSITIVE;
		c = b_big_num_div(a,b,r,l);

		a->type = atype;
		b->type = btype;
		c->type = a->type == b->type ? B_POSITIVE : B_NEGATIVE;
	}

	// 正数相乘
	if(!b_big_num_is_null_ptr(r)){
		c = r;
		if(c == a || c == b) return 0;
	}else{
		unsigned int c_size = a->alloc_len > b->alloc_len ? a->alloc_len : b->alloc_len;
		c = b_big_num_malloc(c_size + 1);
	}
	if(b_big_num_is_null_ptr(c))return 0;
	if(!b_big_num_is_null_ptr(l)){
		d = l;
		if(l == a || l == b) return 0;
	}else{
		unsigned int d_size = a->alloc_len > b->alloc_len ? a->alloc_len : b->alloc_len;
		d = b_big_num_malloc(d_size + 1);
	}
	if(b_big_num_is_null_ptr(d))return 0;
	// 计算
	int atype = a->type;
	int btype = b->type;
	a->type = B_POSITIVE;
	b->type = B_POSITIVE;
	big_num_t* t = b_big_num_malloc(c->alloc_len + 1);
	while(1){
		t = b_big_num_sub(a, b, t);
		if(b_big_num_is_zero(t))break;
		b_big_num_add_one(c);
		b_big_num_init_from_int(d, 1);

	}

	a->type = atype;
	b->type = btype;
	return c;
}
*/
big_num_t* b_big_num_rem(big_num_t* a, big_num_t*b){
	// big_num_t* c = b_big_num_div(a,b);
	// big_num_t* d = b_big_num_mul(b,c);
	// big_num_t* e = b_big_num_sub(a,d);
	// b_big_num_free(c);
	// b_big_num_free(d);
	// return e;
	return 0;
}





/*
void add(int *a, int *b, int *c, int N) {
    int i, carry = 0;
    for(i = N - 1; i >= 0; i--) {
        c[i] = a[i] + b[i] + carry;
        if(c[i] < STORE_TYPE_INT_DIGIT)
            carry = 0;
        else { // 进位
            c[i] = c[i] - STORE_TYPE_INT_DIGIT;
            carry = 1;
        }
    }
}
void sub(int *a, int *b, int *c, int N) {
    int i, borrow = 0;
    for(i = N - 1; i >= 0; i--) {
        c[i] = a[i] - b[i] - borrow;
        if(c[i] >= 0)
            borrow = 0;
        else { // 借位
            c[i] = c[i] + STORE_TYPE_INT_DIGIT;
            borrow = 1;
        }
    }
}
void mul(int *a, int b, int *c, int N) { // b 为乘数
    int i, tmp, carry = 0;
    for(i = N - 1; i >=0; i--) {
        tmp = a[i] * b + carry;
        c[i] = tmp % STORE_TYPE_INT_DIGIT;
        carry = tmp / STORE_TYPE_INT_DIGIT;
    }
}
void div(int *a, int b, int *c, int N) { // b 为除数
    int i, tmp, remain = 0;
    for(i = 0; i < N; i++) {
        tmp = a[i] + remain;
        c[i] = tmp / b;
        remain = (tmp % b) * STORE_TYPE_INT_DIGIT;
    }
}
*/

#endif // _B_BIG_NUM_H


