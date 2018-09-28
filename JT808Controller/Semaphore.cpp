#include "Semaphore.h"


Semaphore::Semaphore() : __mtx(), __condvar()
{
}


Semaphore::~Semaphore()
{
}

void Semaphore::IncreaseLevel(long long increase)
{
	{
		std::lock_guard<std::mutex> lock(this->__mtx);
		this->__semaphoreLevel += increase;
	}
	this->__condvar.notify_all();
}

void Semaphore::DecreaseLevel(long long decrease)
{
	{
		std::lock_guard<std::mutex> lock(this->__mtx);
		this->__semaphoreLevel -= decrease;
	}
	this->__condvar.notify_all();
}

bool Semaphore::Request(long long _g)
{
	std::unique_lock<std::mutex> lock(this->__mtx);
	this->__condvar.wait(lock, [this, _g] { return this->__semaphoreLevel >= _g || !this->__running; });
	if (!this->__running) return false;
	this->__semaphoreLevel -= _g;
	return true;
}

void Semaphore::Stop()
{
	this->__running = false;
	this->__condvar.notify_all();
}
