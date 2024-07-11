#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class SafeQueue {
public:
	SafeQueue() = default;

	void push(T obj) { // потокобезопасная запись объекта в очередь
		std::unique_lock<std::mutex> lk(mutex);
		queue_.push(obj);
		newTask.notify_one(); // оповещаем любой из ожидающих потоков
	}

	bool pop(T& obj) { // потокобезопасные получение элемента очереди с проталкиванием
		std::unique_lock<std::mutex> lk(mutex);
		newTask.wait(lk, [this] {return (!queue_.empty() || stopFlag); } );
		if (stopFlag) return false;
		obj = std::move(queue_.front());
		queue_.pop();
		return true;
	}

	void stop() { // выводит метод pop из режима ожидания новой задачи, метод pop при этом возвращает false
		stopFlag = true;
		newTask.notify_all(); // оповещаем все ожидающие потоки
	}

	void start() { // возобновляет ожидание задач и проталкивание очереди методом pop
		stopFlag = false;
	}

private:
	std::queue<T> queue_;			 // непотокобезопасная очередь
	std::condition_variable newTask; // для уведомлений потоков о новой задаче 
	std::mutex mutex;				 // мьютекс потокобезопасной очереди 
	bool stopFlag = false;			 // останавливает ожидание новых объектов в методе pop()
};