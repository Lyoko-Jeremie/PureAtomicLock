#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

class AtomicLock
{
public:
	// Ĭ��ʹ��std::this_thread::yield()������
	AtomicLock()
		:AB(false), sleeptime(0) {}
	// ���趨������ʱ�� ��λms
	AtomicLock(long long sleeptime)
		:AB(false), sleeptime(sleeptime) {}
	~AtomicLock() {}

	// ����ʽ��ȡ������
	void lock()
	{
		bool Old = false;
		while (!this->AB.compare_exchange_strong(Old, true))
		{	// ���Ǽ��AB�Ƿ�Ϊfalse  Ϊfalse���ȡ��  ��������
			Old = false;
			if (this->sleeptime)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(this->sleeptime));
			}
			else
			{
				std::this_thread::yield();
			}
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
	std::atomic_bool AB;
	long long sleeptime;	// �������ʱ��
};

#endif	// ATOMICLOCK_H