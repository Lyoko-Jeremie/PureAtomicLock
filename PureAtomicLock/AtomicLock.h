#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

class AtomicLock
{
public:
	// 默认使用std::this_thread::yield()做自旋
	AtomicLock()
		:AB(false), sleeptime(0) {}
	// 可设定的自旋时间 单位ms
	AtomicLock(long long sleeptime)
		:AB(false), sleeptime(sleeptime) {}
	~AtomicLock() {}

	// 阻塞式获取锁操作
	void lock()
	{
		bool Old = false;
		while (!this->AB.compare_exchange_strong(Old, true))
		{	// 总是检查AB是否为false  为false则获取锁  否则重试
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

	// 解锁
	void unlock()
	{
		this->AB.store(false);
	}

	// 核心原子量是否无锁
	bool is_lock_free() const
	{
		return this->AB.is_lock_free();
	}

private:
	std::atomic_bool AB;
	long long sleeptime;	// 自旋间隔时间
};

#endif	// ATOMICLOCK_H