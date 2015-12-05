#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

// 警告 不能用此对自旋锁象代替标准库的mutex使用，此实现没有内存屏障特性，不能保证内存可见性和前后语句的顺序一致性
// 
// sleeptime指定自旋等待时间  若sleeptime==0则使用this_thread::yield作为自选等待
// 默认参数为0
template<int sleeptime = 0>
class AtomicLock
{
public:
	AtomicLock()
		:AB(false) {}
	~AtomicLock() {}

	// 阻塞式获取锁操作
	void lock()
	{
		bool Old = false;
		while (!this->AB.compare_exchange_strong(Old, true))
		{	// 总是检查AB是否为false  为false则获取锁  否则重试
			Old = false;
			this->sleep<sleeptime>();
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