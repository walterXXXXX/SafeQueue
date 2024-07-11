#pragma once

#include <thread>
#include <vector>
#include <functional>

#include "SafeQueue.h"

class ThreadPool {
public:
	ThreadPool(unsigned int countThreads);
	~ThreadPool();

	void submit(std::function<void()> func); // помещает в потокобезопасную очередь задачу

private:
	void work(); // выбирает из очереди очередую задачу и исполняет ее, работает в каждом потоке вектора threads

	std::vector<std::thread> threads;			// вектор потоков для выполнения задач
	SafeQueue<std::function<void()>> safeQueue; // потокобезопасная очередь задач
	bool finish_ = false;						// флаг завершения работы потоков
};

