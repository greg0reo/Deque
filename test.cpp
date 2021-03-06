
/*
 * Compile with -ldl.
 */

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
#include "Deque.hpp"

// May assume memcpy()-able.
// May assume = operator.

/*
 * Track memory allocation.
 */

// This functions like an assert(), but is careful not to do anything that
// calls malloc(), so can be used before malloc has been initialized.
#define xyzzy_check(e) do {                                                    \
    if (!(e)) {                                                                \
        const char s[] = #e "\n";                                              \
        write(2, s, sizeof s);                                                 \
        abort();                                                               \
    }                                                                          \
} while (0)

size_t alloc_call_count;
size_t total_bytes_allocated;

namespace {
    bool initialized;
    void *(*default_malloc)(size_t);
    void *(*default_realloc)(void *, size_t);
    void *(*default_calloc)(size_t, size_t);
    void
    alloc_init() {
        if (!initialized) {
            // Cast through int to avoid warnings on some versions of g++.
            default_malloc = (void*(*)(size_t)) (uintptr_t) dlsym(RTLD_NEXT, "malloc"); xyzzy_check(default_malloc != nullptr);
            default_realloc = (void*(*)(void*,size_t)) (uintptr_t) dlsym(RTLD_NEXT, "realloc"); xyzzy_check(default_realloc != nullptr);
            default_calloc = (void*(*)(size_t,size_t)) (uintptr_t) dlsym(RTLD_NEXT, "calloc"); xyzzy_check(default_calloc != nullptr);
            initialized = true;
        }
    }
}

void *
malloc(size_t size) {
    alloc_init();
    void *ptr = default_malloc(size);
    alloc_call_count++;
    total_bytes_allocated += size;
    return ptr;
}

void *
realloc(void *p, size_t size) {
    alloc_init();
    void *ptr = default_realloc(p, size);
    alloc_call_count++;
    total_bytes_allocated += size;
    return ptr;
}

void *
calloc(size_t num, size_t size) {
    alloc_init();
    void *ptr = default_calloc(num, size);
    alloc_call_count++;
    total_bytes_allocated += num*size;
    return ptr;
}

/*
 * Test for class MyClass.
 */

struct MyClass {
    int id;
    char name[10];
};

bool
MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {
    return o1.id < o2.id;
}

bool
MyClass_less_by_name(const MyClass &o1, const MyClass &o2) {
    return strcmp(o1.name, o2.name) < 0;
}

void
MyClass_print(const MyClass *o) {
    printf("%d\n", o->id);
    printf("%s\n", o->name);
}

Deque_DEFINE(MyClass)

/*
 * Test for int.
 */

bool
int_less(const int &o1, const int &o2) {
    return o1 < o2;
}

Deque_DEFINE(int)

int
main() {
    FILE *devnull = fopen("/dev/null", "w");
    assert(devnull != 0);
    {
        Deque_MyClass deq;
	printf("after instantiation\n");
        Deque_MyClass_ctor(&deq, MyClass_less_by_id);
	std::cout << deq.amount << "\n" << std::endl;
	deq.size(&deq);
	printf("after ctor\n");

        assert(deq.size(&deq) == 0);
	printf("after size\n");
        // size() should return a size_t.
        assert(typeid(std::size_t) == typeid(decltype(deq.size(&deq))));
        assert(deq.empty(&deq));
	printf("after empty\n");

        // Should print "---- Deque_MyClass, 14".
        printf("---- %s, %d\n", deq.type_name, (int) sizeof(deq.type_name));
        // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
	printf("during type name\n");
        assert(sizeof deq.type_name == 14); // used to be commented out
	printf("after type_name\n");

	printf("before pushback\n");
        deq.push_back(&deq, MyClass{1, "Joe"});
	printf("middle pushback0\n");
        deq.push_back(&deq, MyClass{2, "Mary"});
	printf("middle pushback1\n");
        deq.push_back(&deq, MyClass{3, "Tom"});
	printf("after push_back\n\n");

	for(int i=0; i < deq.array_size; i++){
		cout << i << ": " << endl;
		MyClass_print(&deq.array[i]);
	}
	printf("\n\n\n\n");


	printf("before push front\n");
        deq.push_front(&deq, MyClass{0, "Mike"});
	printf("middle push front\n");
        deq.push_front(&deq, MyClass{-1, "Mary"});
	printf("after push_front\n\n");

	printf("before print\n");
        MyClass_print(&deq.front(&deq));
	printf("middle print\n");
        MyClass_print(&deq.back(&deq));
	printf("end print\n\n");


	for(int i = 0; i < deq.array_size; i++){
		cout << i << ": " << endl;
		MyClass_print(&deq.array[i]);
	}

	printf("\n\n\n\n");


	printf("front:\n");
        assert(deq.front(&deq).id == -1);
	printf("back:\n");
        assert(deq.back(&deq).id == 3);

	printf("testing testing, this is a test, and i am testing");
	printf("pop front:\n");
        deq.pop_front(&deq);
	printf("pop back:\n");
        deq.pop_back(&deq);
	printf("deq.front\n");
        assert(deq.front(&deq).id == 0);
	printf("deq.back\n");
        assert(deq.back(&deq).id == 2);

	printf("size again\n");
        assert(deq.size(&deq) == 3);
	printf("after size\nbefore iterator\n");

        for (Deque_MyClass_Iterator it = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
  		printf("index= %d\n", it.index);
//		it.index++;
//		it.dec(&it);
            MyClass_print(&it.deref(&it));
		printf("we get it2222\n");
        }

	printf("post iterator\n");
	printf("before multiple iterators\n");

        // Multiple iterators?
        for (Deque_MyClass_Iterator it1 = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it1, deq.end(&deq)); it1.inc(&it1)) {
            MyClass_print(&it1.deref(&it1));
            for (Deque_MyClass_Iterator it2 = deq.begin(&deq);
             !Deque_MyClass_Iterator_equal(it2, deq.end(&deq)); it2.inc(&it2)) {
                MyClass_print(&it2.deref(&it2));
                for (Deque_MyClass_Iterator it3 = deq.begin(&deq);
                 !Deque_MyClass_Iterator_equal(it3, deq.end(&deq)); it3.inc(&it3)) {
                    MyClass_print(&it3.deref(&it3));
                }
            }
        }
	printf("after multiterators\nbefore end decrement\n");


        // Test decrement of end.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it).id == 2);
        }

	printf("after end decrement\nbefore using at\n");


        // printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) {
            MyClass_print(&deq.at(&deq, i));
        }

	printf("after using at\nbefore testing for reference return\n");

        // Test that front(), back(), at(), and deref() are returning a reference.
        // Change via at().
        assert(deq.at(&deq, 0).id == 0);
        deq.at(&deq, 0).id = 100;
        assert(deq.at(&deq, 0).id == 100);

	printf("after testing reference return\n");
	printf("\n");

        // Change via front().
	cout << deq.front(&deq).id << endl;
        assert(deq.front(&deq).id == 100);
	printf("1\n");
        deq.front(&deq).id = 0;
	printf("2\n");
        assert(deq.front(&deq).id == 0);
	printf("3\n");
	printf("middle of testing changing via front\n");

        assert(deq.at(&deq, 0).id == 0); // Verify with at() also.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it).id == 2);
            it.deref(&it).id = 200;
            assert(it.deref(&it).id == 200);
            // Change using back().
            assert(deq.back(&deq).id == 200);
            deq.back(&deq).id = 2;
            assert(deq.back(&deq).id == 2);
            assert(it.deref(&it).id == 2); // Verify with iterator also.
        }
	printf("right before deq.clear\n");

        deq.clear(&deq);
	printf("after deq.clear\n");
	printf("\n");

        deq.dtor(&deq);

        // Test equality.  Two deques compare equal if they are of the same
        // length and all the elements compare equal.  It is undefined behavior
        // if the two deques were constructed with different comparison
        // functions.
        {
            Deque_MyClass deq1, deq2;
            Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
            Deque_MyClass_ctor(&deq2, MyClass_less_by_id);
	    printf("is it here?");

            deq1.push_back(&deq1, MyClass{1, "Joe"});
            deq1.push_back(&deq1, MyClass{2, "Jane"});
            deq1.push_back(&deq1, MyClass{3, "Mary"});
            deq2.push_back(&deq2, MyClass{1, "Joe"});
            deq2.push_back(&deq2, MyClass{2, "Jane"});
            deq2.push_back(&deq2, MyClass{3, "Mary"});

            assert(Deque_MyClass_equal(deq1, deq2));

            deq1.pop_back(&deq1);
            assert(!Deque_MyClass_equal(deq1, deq2));
            deq1.push_back(&deq1, MyClass{4, "Mary"});
            assert(!Deque_MyClass_equal(deq1, deq2));

            deq1.dtor(&deq1);
            deq2.dtor(&deq2);
        }

    }

    // Test that it can handle other types.  Tests are the same, more or less.
    {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);

        assert(deq.size(&deq) == 0);
        assert(deq.empty(&deq));
	printf("should print ---------\n");

        // Should print "---- Deque_int, 10".
        printf("---- %s, %d\n", deq.type_name, int(sizeof(deq.type_name)));
        // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
        assert(sizeof deq.type_name == 10);

        deq.push_back(&deq, 1);
        deq.push_back(&deq, 2);
        deq.push_back(&deq, 3);
        deq.push_front(&deq, 0);
        deq.push_front(&deq, -1);

        printf("%d\n", deq.front(&deq));
        printf("%d\n", deq.back(&deq));
        assert(deq.front(&deq) == -1);
        assert(deq.back(&deq) == 3);

        deq.pop_front(&deq);
        deq.pop_back(&deq);
        assert(deq.front(&deq) == 0);
        assert(deq.back(&deq) == 2);

        assert(deq.size(&deq) == 3);
	printf("before a for loop\n");

        for (Deque_int_Iterator it = deq.begin(&deq);
         !Deque_int_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
            printf("%d\n", it.deref(&it));
        }
	printf("after for loop\nbefore test decrement\n");

        // Test decrement.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it) == 2);
        }

        // printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) {
            printf("%d: %d\n", int(i), deq.at(&deq, i));
        }

        deq.clear(&deq);

        deq.dtor(&deq);
    }

    // Test equality.  It is undefined behavior if the two deques were constructed with different
    // comparison functions.
    {
        Deque_int deq1, deq2;
        Deque_int_ctor(&deq1, int_less);
        Deque_int_ctor(&deq2, int_less);

        deq1.push_back(&deq1, 1);
        deq1.push_back(&deq1, 2);
        deq1.push_back(&deq1, 3);
        deq2.push_back(&deq2, 1);
        deq2.push_back(&deq2, 2);
        deq2.push_back(&deq2, 3);

        assert(Deque_int_equal(deq1, deq2));

        deq1.pop_back(&deq1);
        assert(!Deque_int_equal(deq1, deq2));
        deq1.push_back(&deq1, 4);
        assert(!Deque_int_equal(deq1, deq2));

        deq1.dtor(&deq1);
        deq2.dtor(&deq2);
    }

    // Test performance.
    {
        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;
        std::uniform_real_distribution<float> action;
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);

        for (int i = 0; i < 10000; i++) {
            deq.push_back(&deq, i);
        }

        // In one end, out the other.
        for (int i = 0; i < 20000000; i++) {
            // fprintf(stderr, "iter: %d\n", i);
            deq.push_back(&deq, i);
            deq.pop_front(&deq);
        }
        for (int i = 0; i < 20000000; i++) {
            deq.push_front(&deq, i);
            deq.pop_back(&deq);
        }

        // To do some computation, to prevent compiler from optimizing out.
        size_t sum = 0, max_size = 0;
        // Random.
        int pf = 0, pb = 0, pof = 0, pob = 0;
        for (int i = 0; i < 10000000; i++) {
            if (action(e) > .55) {
                if (rand(0, 1)(e) == 0) {
                    deq.push_back(&deq, i);
                    pf++;
                } else {
                    deq.push_front(&deq, i);
                    pb++;
                }
            } else {
                if (deq.size(&deq) > 0) {
                    if (rand(0, 1)(e) == 0) {
                        deq.pop_back(&deq);
                        pob++;
                    } else {
                        deq.pop_front(&deq);
                        pof++;
                    }
                }
            }
            max_size = std::max(deq.size(&deq), max_size);
            // Randomly access one element.
            if (deq.size(&deq) > 0) {
                sum += deq.at(&deq, rand(0, deq.size(&deq) - 1)(e));
            }
        }
        // Print it out to prevent optimizer from optimizing out the at() calls.
        fprintf(devnull, "%lu", sum);
        printf("%zu max size\n", max_size);
        printf("%d push_backs, %d push_fronts, %d pop_backs, %d pop_fronts, %d size\n", pb, pf, pob, pof, (int) deq.size(&deq));
        deq.dtor(&deq);
    }

    // Test random access performance
    {
       size_t sum = 0;
       int lo = 0, hi = 10000000;
       Deque_int deq;
       Deque_int_ctor(&deq, int_less);

       for(int i = lo; i < hi; i++) {
          deq.push_back(&deq, i);
       }

       for(int i = lo; i < hi; i++) {
          sum += deq.at(&deq, i);
       }
       printf("Sum of all integers between %d and %d calculated using a deque is %lu.\n", lo, hi, sum);
       deq.dtor(&deq);
    }


	printf("right before sorting test\n\n\n");

    // Test sort.
    // You must be able to work with the correct less-than function.
    {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);
        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 10000; i++) {
            deq.push_back(&deq, rand(-1000000, 1000000)(e));
        }

	printf("immediately before sort\n");

        deq.sort(&deq, deq.begin(&deq), deq.end(&deq));

	printf("after sort, before dtor\n");

        deq.dtor(&deq);
    }
	printf("oh boy time to start testing again\n");
	printf("\n");


    // Sorting Test 2
    {
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

       for (int i=0;i<10000;i++) {
           deq1.push_back(&deq1, i);
       }

       for (int i=20000;i>=10000;i--) {
           deq1.push_back(&deq1,i);
       }

       deq1.push_back(&deq1,20001);
      
       auto iter1 =  deq1.end(&deq1);
       iter1.dec(&iter1);	
       
       auto iter2 = deq1.begin(&deq1);
       
       for (int i=0;i<10000;i++) {
           iter2.inc(&iter2);
       }

       deq1.sort(&deq1, iter2,iter1);

       Deque_int deq2;
       Deque_int_ctor(&deq2 , int_less);

       for(int i=0;i<=20001;i++) {
           deq2.push_back(&deq2,i);
       }

	printf("no way we get here\n");

      assert(Deque_int_equal(deq1, deq2)); 

	printf("do we come out on the other end?\n");

      deq1.dtor(&deq1);
      deq2.dtor(&deq2);
    }

    // Test sort with different comparators.
    {
        Deque_MyClass sort_by_id, sorted_by_id;
        Deque_MyClass sort_by_name, sorted_by_name;

        Deque_MyClass_ctor(&sort_by_id, MyClass_less_by_id);
        Deque_MyClass_ctor(&sorted_by_id, MyClass_less_by_id);
        Deque_MyClass_ctor(&sort_by_name, MyClass_less_by_name);
        Deque_MyClass_ctor(&sorted_by_name, MyClass_less_by_name);

        sort_by_id.push_back(&sort_by_id, MyClass{1, "Bob"});
        sort_by_id.push_back(&sort_by_id, MyClass{3, "Sheldon"});
        sort_by_id.push_back(&sort_by_id, MyClass{2, "Alex"});

        sorted_by_id.push_back(&sorted_by_id, MyClass{1, "Bob"});
        sorted_by_id.push_back(&sorted_by_id, MyClass{2, "Alex"});
        sorted_by_id.push_back(&sorted_by_id, MyClass{3, "Sheldon"});

        sort_by_name.push_back(&sort_by_name, MyClass{1, "Bob"});
        sort_by_name.push_back(&sort_by_name, MyClass{3, "Sheldon"});
        sort_by_name.push_back(&sort_by_name, MyClass{2, "Alex"});

        sorted_by_name.push_back(&sorted_by_name, MyClass{2, "Alex"});
        sorted_by_name.push_back(&sorted_by_name, MyClass{1, "Bob"});
        sorted_by_name.push_back(&sorted_by_name, MyClass{3, "Sheldon"});

        assert(!Deque_MyClass_equal(sort_by_id, sorted_by_id));
        sort_by_id.sort(&sort_by_id, sort_by_id.begin(&sort_by_id), sort_by_id.end(&sort_by_id));
        assert(Deque_MyClass_equal(sort_by_id, sorted_by_id));

        assert(!Deque_MyClass_equal(sort_by_name, sorted_by_name));
        sort_by_name.sort(&sort_by_name, sort_by_name.begin(&sort_by_name), sort_by_name.end(&sort_by_name));
        assert(Deque_MyClass_equal(sort_by_name, sorted_by_name));

        sort_by_id.dtor(&sort_by_id);
        sorted_by_id.dtor(&sorted_by_id);
        sort_by_name.dtor(&sort_by_name);
        sorted_by_name.dtor(&sorted_by_name);
    }

    // Performance testing for sorting
    {
      
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 1000000; i++) {
            deq1.push_back(&deq1, rand(-1000000, 1000000)(e));
        }

       auto iter1 = deq1.begin(&deq1);
       auto iter2 = deq1.begin(&deq1);

       for(int i=0;i<10;i++)
           iter1.inc(&iter1);

       for(int i=0;i<20;i++)
           iter2.inc(&iter2);

       for(int i=0;i<1000000;i++)
       	   deq1.sort(&deq1, iter1,iter2);

       deq1.dtor(&deq1);

    }

   // Print allocation info
   printf("%ld allocations totalling %ld bytes\n", alloc_call_count, total_bytes_allocated);
   int rv = fclose(devnull);
   assert(rv == 0);
}
