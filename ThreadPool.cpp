#include "ThreadPool.h"

#include <iostream>

#include "SafeQueue.h"

ThreadPool::ThreadPool(unsigned int countThreads) {
	for (int i = 0; i < countThreads; i++) {
		threads.push_back(std::thread(&ThreadPool::work, this));
	}
}

ThreadPool::~ThreadPool() {
	finish_ = true;				// завершаем работу функций work() во всех запущенных потоках
	safeQueue.stop();			// выводит из ожидания запущенные в потоках методы pop очереди SafeQueue
	for (auto& th : threads)
		if (th.joinable())
			th.join();			// и присоединяем все потоки
}

void ThreadPool::submit(std::function<void()> func) {
	safeQueue.push(func);
}

void ThreadPool::work() {
	while (!finish_) {
		std::function<void()> task;
		if (safeQueue.pop(task)) 
			task();
	}
}