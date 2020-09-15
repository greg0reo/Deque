#ifndef DEQUE_H
#define DEQUE_H

//test
//https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/

#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <typeinfo>
#include <iostream>

using namespace std;

#define Deque_DEFINE(t)	\
	\
	struct Deque_##t##_Iterator;	\
					\
/*	struct Deque_##t;		*/\
					\
	struct Deque_##t { 		\
					\
					\
					\
		int head;		\
		int tail;		\
/*		char type_name[10];	*/\
		t *array;		\
		size_t amount;		\
		int array_size;		\
		int array_fill;		\
		int index;		\
					\
		/*char type_name[holder];			*/	\
					\
					\
					\
		bool (*empty)(Deque_##t *);		/*done*/			\
		void (*push_back)(Deque_##t *, t);	/*done*/			\
		void (*push_front)(Deque_##t *, t);	/*done*/			\
		t &(*front)(Deque_##t *);		/*done*/			\
		t &(*back)(Deque_##t *);		/*done*/			\
		void (*pop_front)(Deque_##t *);		/*done*/			\
		void (*pop_back)(Deque_##t *);		/*done*/			\
		size_t (*size)(Deque_##t *);		/*done*/			\
		Deque_##t##_Iterator (*begin)(Deque_##t *);			\
		Deque_##t##_Iterator (*end)(Deque_##t *);			\
		bool (*equal)(Deque_##t, Deque_##t);				\
		void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);\
											\
		t &(*at)(Deque_##t *, int);	\
		void (*clear)(Deque_##t *);	\
		void (*dtor)(Deque_##t *);	\
						\
/*		char type_name[6 + sizeof(t)];	*/				\
/*		char type_name[sizeof(#t) + sizeof("Deque_") -1];		*/\
/*		char type_name[sizeof("Deque_"##t)] = "Deque_"##t;		*/\
		char type_name[sizeof("Deque_"#t)] = "Deque_"#t;				\
/*		char type_name[strlen("Deque_"#t) + 1];			*/	\
		\
		\
		\
	};	\
		\
		\
		\
		\
		\
		\
		\
		\
		\
		\
						\
	Deque_##t##_Iterator begin(Deque_##t *deq){						\
		Deque_##t##_Iterator itr;					\
		itr.index = 0;				\
		itr.deq = deq;				\
		itr.inc = &Deque_##t##_Iterator_inc;				\
		itr.dec = &Deque_##t##_Iterator_dec;				\
		itr.deref = &Deque_##t##_Iterator_deref;				\
		itr.equal = &Deque_##t##_Iterator_equal;				\
	}					\
						\
						\
							\
	bool Deque_##t##_empty(Deque_##t *deq){		\
	}							\
								\
	void Deque_##t##_push_back(Deque_##t *deq, t myClass){		\
	    if(deq->array_size == 0){				\
	        deq->array_size = 1;				\
	        deq->array = (t *) realloc(deq->array, deq->array_size * sizeof(t));		\
	    }							\
	    if(deq->tail - deq->head >= deq->array_size){	\
	        /*resizing the array goes here */		\
		cout << "------BACK-RESIZING------\n" << endl;		\
	        deq->array_size *= 2;				\
	        t * temp_array = (t *)malloc(deq->array_size * sizeof(t));			\
	        for(int i = 0; i < deq->array_size/2; i++){	\
		    cout << deq->head << endl;		\
	            temp_array[i] = deq->array[(deq->head + i) % (deq->array_size/2)];		\
	        }						\
	        deq->head = 0;					\
	        deq->tail = (deq->array_size/2);		\
	        deq->array = temp_array;			\
								\
		cout << "head: " << deq->head << "tail: " << deq->tail << "size: " << deq->array_size << "back\n"<<endl;							\
								\
	    }							\
		cout << "back pushing to this tail: " << deq->tail  << "\n" << endl;	\
	    deq->array[deq->tail % deq->array_size] = myClass;	\
	    deq->tail++;					\
								\
	}							\
								\
								\
	void Deque_##t##_push_front(Deque_##t *deq, t myClass){	\
	    if(deq->array_size == 0){				\
	        deq->array_size = 1;				\
	        deq->array = (t *) realloc(deq->array, deq->array_size * sizeof(t));		\
	    }							\
	    if((deq->tail - deq->head) >= deq->array_size){	\
	        /* resizing of the array goes here */		\
		cout << "------FRONT-RESIZING------\n" << endl;		\
	        deq->array_size *= 2;				\
	        t * temp_array = (t *)malloc(deq->array_size * sizeof(t));		\
	        for(int i = 0; i < deq->array_size/2; i++){		\
	            temp_array[i] = deq->array[(deq->head + i) % (deq->array_size/2)];		\
	        }						\
	        deq->head = 0;					\
	        deq->tail = (deq->array_size/2);		\
	        deq->array = temp_array;			\
		cout << "head: " << deq->head << "tail: " << deq->tail << "size: " << deq->array_size << "front\n"<<endl;				\
	    }							\
	    deq->array[(deq->head + deq->array_size -1) % deq->array_size] = myClass;		\
	    deq->head = (deq->head + deq->array_size -1) % deq->array_size;		\
		cout << "front pushing to this head: " << deq->head << "\n" << endl;	\
	    if(deq->head >= deq->tail){				\
	        deq->tail += deq->array_size;			\
		cout << "I AM TRIGGERED" << endl;								\
	    }							\
	}							\
								\
								\
								\
								\
							\
/*	void Deque_##t##_push_back(Deque_##t *deq, t myClass){	\
   		printf("this is push back\n");			\
		if(deq->array_size == 0){			\
			printf("yeo this array is zero sized\n");		\
			deq->array_size = 1;			\
			deq->array = (t *) realloc(deq->array, deq->array_size * sizeof(t));	\
			printf("after resize\n");			\
			\
			\
		}			\
		if (deq->array_fill++ >= deq->array_size){		 		\
			printf(" 1\n");		\
			deq->array_size *= 2;				\
			deq->array = (t *) realloc(deq->array, deq->array_size * sizeof(t));	\
			printf(" 3\n"); 	\
		}						\
		deq->array[deq->tail] = myClass;		\
		printf("4\n");			\
		deq->tail++;		\
	}							\
								\
	void Deque_##t##_push_front(Deque_##t *deq, t myClass){	\
		printf("this is push front\n");			\
					\
					\
					\
			\
			\
	}							\
*/							\
							\
	t &Deque_##t##_front(Deque_##t *deq){		\
		cout << "front, head is: " << deq->head << endl;	\
		return deq->array[deq->head];			\
	}							\
							\
	t &Deque_##t##_back(Deque_##t *deq){			\
		cout << "back, tail is: " << deq-> tail << endl;	\
		return deq->array[(deq->tail - 1) % deq->array_size];			\
	}							\
	void Deque_##t##_pop_front(Deque_##t *deq){		\
		if(deq->head != deq->tail){							\
			deq->head++;				\
		}					\
							\
	}							\
							\
	void Deque_##t##_pop_back(Deque_##t *deq){		\
		if(deq->head != deq->tail){					\
			deq->tail--;				\
		}					\
							\
							\
	}							\
							\
	size_t Deque_##t##_size(Deque_##t *deq){		\
		std::cout << "this is size\n" << std::endl;	\
		return deq->tail - deq->head;				\
/*		return deq->amount;			*/	\
	}							\
							\
	Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *deq){	\
	}							\
							\
	Deque_##t##_Iterator Deque_##t##_end(Deque_##t *deq){	\
	}							\
	t &Deque_##t##_at(Deque_##t *deq, int i){			\
		return deq->array[(deq->head + i) % deq->array_size];		\
							\
							\
	}							\
							\
	void Deque_##t##_clear(Deque_##t *deq){			\
	}							\
							\
	bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2){	\
							\
	}							\
							\
	void Deque_##t##_sort(Deque_##t *deq, Deque_##t##_Iterator itr1, Deque_##t##_Iterator itr2){\
							\
	}							\
							\
	void Deque_##t##_dtor(Deque_##t *deq){			\
	}							\
								\
								\
								\
/* ---------------------------ITERATOR ITERATOR ITERATOR-------------------------*/								\
								\
								\
	void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *itr){\
		itr->index++;								\
	}							\
								\
	void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *itr){\
		itr->index--;						\
	}							\
								\
	t Deque_##t##_Iterator_deref(Deque_##t##_Iterator *itr){	\
		return (itr->deq->at(itr->deq, itr->index));								\
	}							\
								\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator itr1, Deque_##t##_Iterator itr2){\
								\
	}							\
								\
								\	
	struct Deque_##t##_Iterator{			\
		void (*inc)(Deque_##t##_Iterator *);	\
		void (*dec)(Deque_##t##_Iterator *);	\
		bool (*equal)(Deque_##t##_Iterator, Deque_##t##_Iterator);	\
		t &(*deref)(Deque_##t##_Iterator *);	\
							\
		int index;				\
		Deque_##t *deq;				\
	};	\
		\
	void Deque_##t##_ctor(Deque_##t *greg, bool (*comp)(const t &addr1, const t &addr2)){	\
								\
		greg->array_size = 0;				\
		greg->head = 0;					\
		greg->tail = 0;					\
								\
		greg->amount = 0;				\
		greg->array_fill = 0;				\
		greg->index = 0;				\
								\
		greg->array = (t *)malloc(sizeof(t) * 10);	\
		if(!greg->array){				\
			printf("Malloc didn't work\n");		\
			exit(1);				\
		}						\
								\
		greg->type_name[0] = '\0';			\
		strcat(greg->type_name, "Deque_");		\
		strcat(greg->type_name, #t);			\
								\
		greg->empty = &Deque_##t##_empty;		\
		greg->push_back = &Deque_##t##_push_back;	\
		greg->push_front = &Deque_##t##_push_front;	\
		greg->front = &Deque_##t##_front;		\
								\
		greg->back = &Deque_##t##_back;			\
		greg->pop_front = &Deque_##t##_pop_front;	\
		greg->pop_back = &Deque_##t##_pop_back;		\
		greg->size = &Deque_##t##_size;			\
		greg->begin = &Deque_##t##_begin;		\
		greg->end = &Deque_##t##_end;			\
		greg->equal = &Deque_##t##_equal;		\
								\
		greg->sort = &Deque_##t##_sort;			\
		greg->at = &Deque_##t##_at;			\
		greg->clear = &Deque_##t##_clear;		\
		greg->dtor = &Deque_##t##_dtor;			\
	}							\
								\
								\

#endif	
