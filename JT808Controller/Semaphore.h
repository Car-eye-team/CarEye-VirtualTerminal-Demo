#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
private:
	mutable std::mutex __mtx;
	std::condition_variable __condvar;
	long long __semaphoreLevel = 0;
	bool __running = true;
public:
	Semaphore();
	~Semaphore();
	void IncreaseLevel(long long increase);
	void DecreaseLevel(long long decrease);
	bool Request(long long _g);
	void Stop();
};

