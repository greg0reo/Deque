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
					\
					\
		/*char type_name[holder];			*/	\
					\
		bool (*comp)(const t &, const t &);			\
		int (*comparison)(const void *, const void *, void *);			\
		int (*compHelp)(const void*, const void *, void *);			\
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
						\
						\
							\
	bool Deque_##t##_empty(Deque_##t *deq){		\
		if(deq->size(deq) == 0){							\
			return true;						\
		}						\
		return false;						\
								\
	}							\
								\
	void Deque_##t##_push_back(Deque_##t *deq, t myClass){		\
	    if(deq->array_size == 0){				\
	        deq->array_size = 1;				\
	        deq->array = (t *) realloc(deq->array, deq->array_size * sizeof(t));		\
	    }							\
	    if(deq->tail - deq->head >= deq->array_size){	\
	        /*resizing the array goes here */		\
/*		cout << "------BACK-RESIZING------\n" << endl;		\
*/	        deq->array_size *= 2;				\
	        t * temp_array = (t *)malloc(deq->array_size * sizeof(t));			\
	        for(int i = 0; i < deq->array_size/2; i++){	\
/*		    cout << "here?" << endl;		\
*/	            temp_array[i] = deq->array[(deq->head + i) % (deq->array_size/2)];		\
	        }						\
	        deq->head = 0;					\
	        deq->tail = (deq->array_size/2);		\
	        deq->array = temp_array;			\
								\
/*		cout << "head: " << deq->head << "tail: " << deq->tail << "size: " << deq->array_size << "back\n"<<endl;							\
*/								\
	    }							\
/*		cout << "back pushing to this tail: " << deq->tail  << "\n" << endl;	\
*/	    deq->array[deq->tail % deq->array_size] = myClass;	\
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
/*		cout << "------FRONT-RESIZING------\n" << endl;		\
*/	        deq->array_size *= 2;				\
	        t * temp_array = (t *)malloc(deq->array_size * sizeof(t));		\
	        for(int i = 0; i < deq->array_size/2; i++){		\
	            temp_array[i] = deq->array[(deq->head + i) % (deq->array_size/2)];		\
	        }						\
	        deq->head = 0;					\
	        deq->tail = (deq->array_size/2);		\
	        deq->array = temp_array;			\
/*		cout << "head: " << deq->head << "tail: " << deq->tail << "size: " << deq->array_size << "front\n"<<endl;				\
*/	    }							\
	    deq->array[(deq->head + deq->array_size -1) % deq->array_size] = myClass;		\
	    deq->head = (deq->head + deq->array_size -1) % deq->array_size;		\
/*		cout << "front pushing to this head: " << deq->head << "\n" << endl;	\
*/	    if(deq->head >= deq->tail){				\
	        deq->tail += deq->array_size;			\
/*		cout << "I AM TRIGGERED" << endl;								\
*/	    }							\
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
/*		cout << "front, head is: " << deq->head << endl;	\
		cout << "front, array size is: " <<deq->array_size << endl;							\
*/		return deq->array[deq->head];			\
	}							\
							\
	t &Deque_##t##_back(Deque_##t *deq){			\
/*		cout << "back, tail is: " << deq-> tail << endl;	\
*/		return deq->array[(deq->tail - 1) % deq->array_size];			\
	}							\
	void Deque_##t##_pop_front(Deque_##t *deq){		\
		if(deq->head != deq->tail){							\
			deq->head = (deq->head+1)%deq->array_size;				\
			if(deq->tail > deq->array_size && deq->tail > deq->head + deq->array_size){							\
				deq->tail -= deq->array_size;				\
			}				\
		}					\
							\
	}							\
							\
	void Deque_##t##_pop_back(Deque_##t *deq){		\
		if(deq->head != deq->tail){					\
			if(deq->tail != 0){					\
				deq->tail--;					\
			}else{				\
				deq->tail = deq->array_size-1;				\
			}				\
		}					\
							\
	}							\
							\
	size_t Deque_##t##_size(Deque_##t *deq){		\
/*		std::cout << "this is size\n" << std::endl;	\
*/		return deq->tail - deq->head;				\
/*		return deq->amount;			*/	\
	}							\
							\
/*	Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *deq){	\
	}							\
*/							\
							\
	t &Deque_##t##_at(Deque_##t *deq, int i){			\
		return deq->array[(deq->head + i) % deq->array_size];		\
							\
							\
	}							\
							\
	void Deque_##t##_clear(Deque_##t *deq){			\
		deq->head = 0;					\
		deq->tail = 0;					\
	}							\
							\
	bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2){	\
		if(deq1.size(&deq1) != deq2.size(&deq2)){				\
			cout << "there is a problem with the size\n" << endl; \
			return false;			\
		}				\
		for(int i = 0; i < deq2.size(&deq2); i++){		\
/*			if(deq1.comp(deq1.at(&deq1, i), deq2.at(&deq2, i)) || deq2.comp(deq2.at(&deq2, i), deq1.at(&deq1, i))){				\
							\
							\
/*			if(deq1.comp(deq1.at(&deq1, i), deq2.at(&deq2, i))){	\
/*			cout << "there is a problem with comp\n" << endl;		\
				cout << "deq1\n" << endl;		\
				return false;		\
			}			\
*/			if(deq2.comp(deq2.at(&deq2, i), deq1.at(&deq1, i))){	\
				cout << i << endl;		\
				return false;		\
			}			\
		}				\
		return true;				\
	}							\
/*							\
	int Deque_##t##_compHelp(const void *one, const void* two){						\
		if(this.comp(*((t*) one), *((t*)two))){					\
			return -1;				\
		}					\
		if(this.comp(*((t*)two), *((t*)one))){					\
			return 1;				\
		}					\
		return 0;					\
	}						\
*/							\
	int t##_comparison(const void *one, const void *two, void *deq){						\
		bool (*comp)(const t &, const t &) = (bool (*)(const t &, const t &))deq;				\
		const t *one2 = (const t*)one;				\
		const t *two2 = (const t*)two;					\
		if(comp(*one2, *two2)){					\
			return -1;				\
		}					\
		if(comp(*two2, *one2)){					\
			return 1;				\
		}					\
		return 0;					\
	}						\
								\
							\
	int Deque_##t##_compHelp(const void *one, const void *two, void *deq){						\
						\
							\
		t one2 = *(t *)one;					\
		t two2 = *(t *)two;					\
		Deque_##t deq2 = *(Deque_##t *)deq;					\
		if(deq2.comp(one2, two2)){					\
			return -1;				\
		}					\
		if(deq2.comp(two2, one2)){					\
			return 1;				\
		}					\
		return 0;					\
	}							\
							\
							\
	void swap(t *xp, t *yp){						\
		t temp = *xp;					\
		*xp = *yp;					\
		*yp = temp;					\
							\
							\
	}						\
							\
	void bubbleSort(t *array, int n, Deque_##t *deq){						\
		int i, j;						\
		t temp;					\
		for(i = 0; i < n-1; i++){					\
			for(j = 0; j < n-i-1; j++){				\
				if(deq->comp(array[j], array[j+1])){			\
					swap(&array[j], &array[j+1]);		\
							\
							\
/*					temp = array[j];		\
					array[j] = array[j+1];		\
					array[j+1] = temp;		\
*/							\
/*				if(array->comp(array->at(array, j), array->at(array, j+1))){			\
					temp = array->at(array, j);		\
					array->at(array, j) = array->at(array, j+1);		\
					array->at(array, j+1) = temp;		\
*/				}			\
							\
			}				\
							\
		}					\
							\
	}						\
							\
							\
							\
	void Deque_##t##_sort(Deque_##t *deq, Deque_##t##_Iterator itr1, Deque_##t##_Iterator itr2){\
		int num_elements = (deq->array_size + itr1.index - itr2.index) % deq->array_size;					\
		t *tempArray = (t*)malloc(num_elements * sizeof(Deque_##t));		\
		for(int i = 0; i < num_elements; i++){					\
			tempArray[i] = deq->at(deq, (itr1.index +i) % deq->array_size);			\
		}					\
/*		cout << "before qsort_r\n" <<endl;					\
/*		qsort_r(tempArray, num_elements, sizeof(t), deq->compHelp, (void *)deq->comp);				\
*		cout << "after qsort_r\n" << endl;					\
							\
		qsort_r(tempArray, num_elements, sizeof(t), deq->compHelp, deq);					\
*/							\
		bubbleSort(tempArray, num_elements, deq);							\
					\
		for(int i = 0; i < num_elements; i++){					\
							\
			deq->at(deq, (itr1.index + i) % deq->array_size) = tempArray[i];				\
		}					\
		free(tempArray);					\
	}							\
							\
							\
	void Deque_##t##_dtor(Deque_##t *deq){			\
		free(deq->array);						\
	}							\
								\
								\
								\
/* ---------------------------ITERATOR ITERATOR ITERATOR-------------------------*/								\
								\
								\
	void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *itr){\
		int tempIndex = (itr->index + 1)% itr->deq->array_size;						\
								\
/*		printf("inc\n");						\
*/		itr->index = tempIndex;								\
	}							\
								\
	void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *itr){\
/*		printf("dec\n");						\
*/		itr->index--;						\
	}							\
								\
	t& Deque_##t##_Iterator_deref(Deque_##t##_Iterator *itr){	\
/*		printf("deref\n");						\
*/		return (itr->deq->at(itr->deq, itr->index));								\
	}							\
								\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator itr1, 		\
					Deque_##t##_Iterator itr2){		\
/*		printf("equal\n");						\
*/		if(itr1.index == itr2.index){					\
			if(itr1.deq == itr2.deq){				\
				return true;				\
			}						\
		}						\
		return false;						\
	}							\
								\
								\
								\
	Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *deq){						\
/*		printf("begin\n");						\
*/		Deque_##t##_Iterator itr;					\
		itr.index = 0;				\
		itr.deq = deq;				\
		itr.inc = &Deque_##t##_Iterator_inc;				\
		itr.dec = &Deque_##t##_Iterator_dec;				\
		itr.deref = &Deque_##t##_Iterator_deref;				\
		itr.equal = &Deque_##t##_Iterator_equal;				\
		return itr;						\
	}					\
						\
						\
	Deque_##t##_Iterator Deque_##t##_end(Deque_##t *deq){						\
/*		printf("end\n");					\
*/		Deque_##t##_Iterator itr;					\
		itr.index = (deq->tail) - (deq->head);					\
		itr.deq = deq;						\
		itr.inc = &Deque_##t##_Iterator_inc;				\
		itr.dec = &Deque_##t##_Iterator_dec;				\
		itr.deref = &Deque_##t##_Iterator_deref;				\
		itr.equal = &Deque_##t##_Iterator_equal;				\
		return itr;						\
						\
						\
						\
						\
						\
				\
	}						\
						\
						\
						\
	void Deque_##t##_ctor(Deque_##t *greg, bool (*comp)(const t &addr1, const t &addr2)){	\
								\
								\
		greg->array_size = 0;				\
		greg->head = 0;					\
		greg->tail = 0;					\
								\
		greg->amount = 0;				\
		greg->array_fill = 0;				\
		greg->index = 0;				\
								\
		greg->array = (t *)malloc(sizeof(t));	\
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
		greg->comp = comp;						\
		greg->comparison = t##_comparison;						\
		greg->compHelp = &Deque_##t##_compHelp; 						\
								\
		greg->sort = &Deque_##t##_sort;			\
		greg->at = &Deque_##t##_at;			\
		greg->clear = &Deque_##t##_clear;		\
		greg->dtor = &Deque_##t##_dtor;			\
	}							\
								\
								\

#endif
