#include <thread>
#include <algorithm>
#include <vector>
#include <print>
#include <ranges>
#include <cstdlib>
#include "MagicTimer.h"

//#include "rust/cxx.h"
//#include "rust_practice_lab/src/lib.rs.h"

struct Item
{
	int value;
	int weight;
};

Item generateRandomItem()
{
	Item i;
	i.weight = rand() % 100;
	i.value = rand() % 100;
	return i;
}


long someFunc(int someNumber)
{
	for (int i = 0; i < 100; i++)
	{
		someNumber = someNumber + i;
		if (i % 100 == 0)
		{
			someNumber = someNumber * 2;
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return someNumber;
}

void someVoidFunc(int sleepMillis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis));
}

class SomeClass
{
public:
	long someMemberFunc(int someNumber)
	{
		for (int i = 0; i < 100; i++)
		{
			someNumber = someNumber + i;
			if (i % 100 == 0)
			{
				someNumber = someNumber * 2;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		return someNumber;
	}

	long someConstMemberFunc(int someNumber) const
	{
		for (int i = 0; i < 100; i++)
		{
			someNumber = someNumber + i;
			if (i % 100 == 0)
			{
				someNumber = someNumber * 2;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		return someNumber;
	}

	void someVoidMemberFunc(int sleepMillis)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis));
	}
};

template<typename T>
void printVector(const std::vector<T>& vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (i != 0 && i != vector.size())
		{
			std::cout << ", ";
		}
		std::cout << vector[i];
	}
	std::cout << "\n";
}


int main()
{
	std::cout << "Testing Rust functions! \n";
	//hello();
	//counting_until(50);
	//uint64_t fibo_rec = fibonacci_recursive(5);
	//uint64_t fibo_it = fibonacci_iterative(5);
	//std::cout << "A: " << fibo_rec << ", B: " << fibo_it << std::endl;


	Timer t = Timer();


	t.start();
	std::cout << "Hello MagicTimer" << std::endl << std::endl;
	std::cout << "********** Test 1: Simple Test ********** \n";
	//t.stop().printTime(DurationFormat::NANOSECONDS);
	t.stop().printTime(DurationFormat::HOURS);


	int a;
	t.reset().start();
	std::cout << "\n********** Test 2: Time Lambda Function ********** \n";
	t.timeIt(DurationFormat::MICROSECONDS, [&]() { a = someFunc(10); });
	std::cout << "Result: " << a << "\n";


	std::cout << "\n********** Test 3: Time Lambda Function 2 ********** \n";
	t.timeIt(DurationFormat::SECONDS, 
		[]() {
			SomeClass a;
			int b = a.someMemberFunc(10);
			std::cout << "Result: " << b << "\n";
		});


	std::cout << "\n********** Test 4 Time Lambda Function 2 in Minutes ********** \n";
	t.timeIt(DurationFormat::MINUTES,
		[]() {
			SomeClass a;
			int b = a.someMemberFunc(10);
			std::cout << "Result: " << b << "\n";
	});


	std::cout << "\n********** Test 5 Time Class Member Function ********** \n";
	SomeClass obj;
	long res_test_5 = t.timeIt(DurationFormat::MICROSECONDS, &SomeClass::someMemberFunc, &obj, 10);
	std::cout << "Result: " << res_test_5 << "\n";


	std::cout << "\n********** Test 6 Time Class Const Member Function ********** \n";
	long res_test_6 = t.timeIt(DurationFormat::MICROSECONDS, &SomeClass::someConstMemberFunc, &obj, 10);
	std::cout << "Result: " << res_test_6 << "\n";


	std::cout << "\n********** Test 7 Time Class Void Member Function ********** \n";
	t.timeIt(DurationFormat::MICROSECONDS, &SomeClass::someVoidMemberFunc, &obj, 10);


	std::cout << "********** Test 8 Time Free Function With Return Value ********** \n";
	long res = t.timeIt(DurationFormat::MILLISECONDS, someFunc, 10);
	std::cout << "Result: " << res << "\n\n";


	std::cout << "********** Test 9 Time Free Function Without Return Value ********** \n";
	t.timeIt(DurationFormat::MILLISECONDS, someVoidFunc, 10);


	std::cout << "********** Test 10 On Std Functions ********** \n";
	std::vector<int> vec = { 5, 3, 4, 1, 2 };
	std::cout << "Vector: ";
	printVector(vec);
	t.timeIt(DurationFormat::NANOSECONDS, std::ranges::sort, vec.begin(), vec.end());
	std::cout << "Sorted vector: ";
	printVector(vec);


	std::cout << "\n********** Test 11 Get Time Intervals ********** \n";
	t.reset().start();
	std::cout << "Started timer! Do something..." << std::endl;
	t.printTime(DurationFormat::MICROSECONDS);
	std::cout << "Do something again..." << std::endl;
	t.printTime(DurationFormat::MICROSECONDS);
	std::cout << "And again..." << std::endl;
	t.printTime(DurationFormat::MICROSECONDS);
	std::cout << "Stopping timer!" << std::endl;
	t.stop().reset();


	return 0;
}
