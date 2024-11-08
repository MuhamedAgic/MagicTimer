#include <thread>
#include "MagicTimer.h"

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

class SomeClass
{
public:
	long someFuncInClass(int someNumber)
	{
		for (int i = 0; i < 100; i++)
		{
			someNumber = someNumber + i;
			if (i % 100 == 0)
			{
				someNumber = someNumber * 2;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		return someNumber;
	}
};


int main()
{
	Timer t = Timer();
	t.start();
	std::cout << "Hello MagicTimer" << std::endl;
	t.stop().printTime(DurationFormat::NANOSECONDS);

	int a;
	t.reset().start();
	t.timeIt([&]() { a = someFunc(10); }, DurationFormat::MICROSECONDS);
	std::cout << "Result: " << a << "\n";

	t.timeIt([]() { 
		SomeClass a;
		a.someFuncInClass(10); 
	}, DurationFormat::SECONDS); // debug


	return 0;
}
