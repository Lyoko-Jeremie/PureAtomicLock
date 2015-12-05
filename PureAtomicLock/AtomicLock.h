#ifndef ATOMICLOCK_H
#define ATOMICLOCK_H

#include <atomic>
#include <chrono>
#include <thread>

//#include <iostream>

// 警告 不能用此对自旋锁象代替标准库的mutex使用，此实现没有内存屏障特性，不能保证内存可见性和前后语句的顺序一致性
// 此实现以牺牲一致性与适用性换取性能
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
		while (!this->AB.compare_exchange_strong(Old, true))	// 使用CAS模式保证原子性
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
	// 实际的内核对象  在大部分平台下的实现呈现出lock-free特性
	std::atomic_bool AB;

	// 两种不同的延迟实现 使用模板参数在编译期决定实际使用的版本 以避免增加运行时时间消耗
	// 使用偏特化方式自动选择
	// 缺陷 在编译期便确定下了延迟时间 无法在运行时自动配置延迟
	template<int s>
	void sleep()
	{
		//std::cout << "1";
		std::this_thread::sleep_for(std::chrono::milliseconds(s));
	}
};


// 偏特化版本在这里
// GCC不支持VS版本的在类内部进行的偏特化处理  标准写法要写在类外部
template<>	// 类的模板参数
template<>	// 函数的模板参数
inline		// 内联表示
void		// 返回参数
AtomicLock<0>	// 类模板特化
::sleep<0>()	// 函数模板特化
{
	//std::cout << "0";
	std::this_thread::yield();
}

#endif	// ATOMICLOCK_H
