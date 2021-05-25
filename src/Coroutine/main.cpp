#include <future>
#include <iostream>
#include <experimental/generator>
using namespace std;
/*@if compiler error occur for coroutine, then should use the /await option to compiler
   
   -reference
   fibonacci async: https://yizhang82.dev/cpp-coroutines-async-fibonacci
   fibonacci generator using yield: https://luncliff.github.io/posts/MSVC-Coroutine-%EC%95%8C%EC%95%84%EB%B3%B4%EA%B8%B0.html

   This is not an example of "yield" with a previous state. It is maybe an asynchronous example,
   and the previous Fibonacci values ​​are not shared and recalculated in each number.

   TODO: find another example for c++ corutine
*/


future<int> async_add(int a, int b)
{
	auto fut = std::async([=]() {
		int c = a + b;
		return c;
		});
	return fut;
}

future<int> async_fib(int n)
{
	if (n <= 2)
		co_return 1;

	int a = 1;
	int b = 1;
	// iterate computing fib(n)
	for (int i = 0; i < n - 2; ++i)
	{
		int c = co_await async_add(a, b);
		a = b;
		b = c;
	}

	co_return b;
}

future<void> test_async_fib()
{
	for (int i = 1; i < 10; ++i)
	{
		int ret = co_await async_fib(i);
		cout << "async_fib(" << i << ") returns " << ret << endl;
	}
}

int main()
{
	auto fut = test_async_fib();
	fut.wait();

	return 0;
}