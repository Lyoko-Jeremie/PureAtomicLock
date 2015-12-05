#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

//#include <iostream>

// ���� �����ô˶�������������׼���mutexʹ�ã���ʵ��û���ڴ��������ԣ����ܱ�֤�ڴ�ɼ��Ժ�ǰ������˳��һ����
// ��ʵ��������һ�����������Ի�ȡ����
//
// sleeptimeָ�������ȴ�ʱ��  ��sleeptime==0��ʹ��this_thread::yield��Ϊ��ѡ�ȴ�
// Ĭ�ϲ���Ϊ0
template<int sleeptime = 0>
class AtomicLock
{
public:
	AtomicLock()
		:AB(false) {}
	~AtomicLock() {}

	// ����ʽ��ȡ������
	void lock()
	{
		bool Old = false;
		while (!this->AB.compare_exchange_strong(Old, true))	// ʹ��CASģʽ��֤ԭ����
		{	// ���Ǽ��AB�Ƿ�Ϊfalse  Ϊfalse���ȡ��  ��������
			Old = false;
			this->sleep<sleeptime>();
		}
	}

	// ����
	void unlock()
	{
		this->AB.store(false);
	}

	// ����ԭ�����Ƿ�����
	bool is_lock_free() const
	{
		return this->AB.is_lock_free();
	}

private:
	// ʵ�ʵ��ں˶���  �ڴ󲿷�ƽ̨�µ�ʵ�ֳ��ֳ�lock-free����
	std::atomic_bool AB;

	// ���ֲ�ͬ���ӳ�ʵ�� ʹ��ģ������ڱ����ھ���ʵ��ʹ�õİ汾 �Ա�����������ʱʱ������
	// ʹ��ƫ�ػ���ʽ�Զ�ѡ��
	// ȱ�� �ڱ����ڱ�ȷ�������ӳ�ʱ�� �޷�������ʱ�Զ������ӳ�
	template<int s>
	void sleep()
	{
		//std::cout << "1";
		std::this_thread::sleep_for(std::chrono::milliseconds(s));
	}
};


// ƫ�ػ��汾������
// GCC��֧��VS�汾�������ڲ����е�ƫ�ػ�����  ��׼д��Ҫд�����ⲿ
template<>	// ���ģ�����
template<>	// ������ģ�����
inline		// ������ʾ
void		// ���ز���
AtomicLock<0>	// ��ģ���ػ�
::sleep<0>()	// ����ģ���ػ�
{
	//std::cout << "0";
	std::this_thread::yield();
}

#endif	// ATOMICLOCK_H
