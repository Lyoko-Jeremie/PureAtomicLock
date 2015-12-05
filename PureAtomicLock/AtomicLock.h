#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

// sleeptimeָ�������ȴ�ʱ��  ��sleeptime==0��ʹ��this_thread::yield��Ϊ��ѡ�ȴ�
template<int sleeptime>
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
		while (!this->AB.compare_exchange_strong(Old, true))
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
	std::atomic_bool AB;

	template<int s>
	void sleep()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(s));
	}
	template<>
	void sleep<0>()
	{
		std::this_thread::yield();
	}
};

#endif	// ATOMICLOCK_H