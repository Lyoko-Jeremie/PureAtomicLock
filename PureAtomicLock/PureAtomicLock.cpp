// PureAtomicLock.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "AtomicLock.h"

#include <string>
#include <iostream>
#include <mutex>
#include <vector>
using namespace std;

AtomicLock<100> AL; // 使用默认模板参数
std::mutex MTX;
long long int a = 0;
vector<thread> pt;

void th();

int main()
{
	cout << "is_lock_free : " << AL.is_lock_free();
	for (int i = 0; i != 20000; ++i)
		// 3K是个坎 3K以下可以用0 3K开始0就会活锁 3K时使用参数5 耗时略微小于线性增长
		// VS实现的yield比GCC性能好但sleep_for相反
		// 参数10 15K时可做到6s+
		// 此时mutex 8s+ 且性能更低
		// 且参数50时仍然能落到6s+
		// 20K 时参数50 8s+
		// 此时mutex 9s+
		// 调参20 8s+
		// 调参100 8s+
	{
		pt.emplace_back(th);	// 就地构造
	}
	for (size_t i = 0; i != pt.size(); ++i)
	{
		pt.at(i).join();
	}
	cout << endl;
	cout << a << endl;
	//system("pause");
    return 0;
}


void th()
{
	//{
	//	lock_guard<decltype(AL)> alg(AL);
	//	cout << "lock free : " << AL.is_lock_free() << endl;
	//}
	for (size_t i = 0; i != 100; i++)
	{
		lock_guard<decltype(AL)> alg(AL);	// decltype编译期自动分析对象类型
		//lock_guard<decltype(MTX)> mtx(MTX);
		//cout << "th()" << endl;
		++a;
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		std::this_thread::yield();
		--a;
		std::this_thread::yield();
	}
}


