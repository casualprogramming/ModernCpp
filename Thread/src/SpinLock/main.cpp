/*
 *	This code is intended to help you understand.
 *	it is better to use the spinlock library or stl recommended functions.
 */


#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;


/* https://en.wikipedia.org/wiki/Test-and-set
* atomic operation
function TestAndSet(boolean_ref lock) {
	boolean initial = lock
		lock = true
		return initial
}
*/
class SpinLock 
{
public:
	virtual void lock()=0;
	virtual void unlock()=0;
};
class SpinLock_TAS :public SpinLock
{
	std::atomic_flag flag;
public:
	void lock()
	{
		//infinite loop to set true and return previous value
		while (flag.test_and_set());//if(flag == false) then escape loop : must be an atomic operation
	}
	void unlock()
	{
		flag.clear();//flag = false : not need to be an atomic operation (but it's an atomic variable, so it performs only an atomic operation)
	}
};

class SpinLock_CAS :public SpinLock
{
	std::atomic_bool flag;
	bool expected = false;
public:
	void lock()
	{
		/***
		compare_exchange_weak
		- this function always copy previous "atomic object" to "expected"
		- if object==expected then change object to desired
		***/

		//if(flag == expected) then flag=true, escape loop
		//else(flag!=expected) then expected=flag
		while (!flag.compare_exchange_weak(expected, true))
		{
			expected = false;//local variable "expected" should always have false
			//think abount the case where "expected" changes to true with comp_exchange after another thread makes flag true and escapes.
			//We should avoid comp_swap with "expected" and "flag" set to true(occur escape).
		};

	}
	void unlock()
	{
		flag.store(false);//now it make comp_swap true, so it can escape while loop.
	}
};

int iteration = 100000;
#define UseSpinLock true
SpinLock* spinlock = new SpinLock_TAS();
void myThreadAdd(int* src, int plus)
{
#if UseSpinLock
	spinlock->lock();
#endif
	for (int i = 0; i < iteration; i++)
		*src += plus;
#if UseSpinLock
	spinlock->unlock();
#endif
}

constexpr unsigned int thread_num = 8;
int test()
{
	int data = 0;
	std::vector<std::thread> threads;
	for (int i = 0; i < thread_num; i++)
	{
		threads.emplace_back(myThreadAdd, &data, 1);
	}
	for (auto& t : threads)
		t.join();
	return data;
}
int main()
{
	int data = test();
	cout <<"data:"<< data << endl;
	return 0;
}


/*
*	@reference
*	https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
*	https://en.cppreference.com/w/cpp/atomic/atomic_flag
*	https://ofstack.com/C++/24247/how-to-implement-spin-lock-detail-with-c++11-atomic-weight.html
*	https://rigtorp.se/spinlock/
*	https://en.wikipedia.org/wiki/Test-and-set
*	
*/