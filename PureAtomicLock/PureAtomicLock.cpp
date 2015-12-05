// PureAtomicLock.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "AtomicLock.h"

#include <string>
#include <iostream>
#include <mutex>
#include <vector>
using namespace std;

AtomicLock AL;
size_t a = 0;
vector<thread> pt;

void th();

int main()
{
	for (int i = 0; i != 10000; ++i)
	{
		pt.emplace_back(th);
	}
	for (size_t i = 0; i != pt.size(); ++i)
	{
		pt.at(i).join();
	}
	cout << a << endl;
	system("pause");
    return 0;
}


void th()
{
	//{
	//	lock_guard<decltype(AL)> alg(AL);
	//	cout << "lock free : " << AL.is_lock_free() << endl;
	//}
	for (size_t i = 0; i != 1000; i++)
	{
		lock_guard<decltype(AL)> alg(AL);
		//cout << "th()" << endl;
		++a;
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		--a;
	}
}


