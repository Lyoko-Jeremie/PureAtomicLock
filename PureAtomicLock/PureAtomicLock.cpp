// PureAtomicLock.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "AtomicLock.h"

#include <string>
#include <iostream>
#include <mutex>
#include <vector>
using namespace std;

AtomicLock<5> AL; // ʹ��Ĭ��ģ�����
long long int a = 0;
vector<thread> pt;

void th();

int main()
{
	cout << "is_lock_free : " << AL.is_lock_free();
	for (int i = 0; i != 10000; ++i)
		// 3K�Ǹ��� 3K���¿�����0 3K��ʼ0�ͻ���� 3Kʱʹ�ò���5 ��ʱ��΢С����������
		// VSʵ�ֵ�yield��GCC���ܺõ�sleep_for�෴
	{
		pt.emplace_back(th);	// �͵ع���
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
		lock_guard<decltype(AL)> alg(AL);	// decltype�������Զ�������������
		//cout << "th()" << endl;
		++a;
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		std::this_thread::yield();
		--a;
		std::this_thread::yield();
	}
}


