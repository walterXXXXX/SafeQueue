#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <conio.h>

#include "ThreadPool.h"

std::mutex consoleMutex; // мьютекс вывода в консоль

void func1() {
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::lock_guard lk(consoleMutex);
	std::cout << __FUNCTION__;
	std::cout << " (200мс) - выполнена в потоке c id=" << std::this_thread::get_id() << std::endl;
}

void func2() {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::lock_guard lk(consoleMutex);
	std::cout << __FUNCTION__;
	std::cout << " (500мс) - выполнена в потоке c id=" << std::this_thread::get_id() << std::endl;
}

void func3() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::lock_guard lk(consoleMutex);
	std::cout << __FUNCTION__;
	std::cout << " (1000мс) - выполнена в потоке c id=" << std::this_thread::get_id() << std::endl;
}

void func4() {
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::lock_guard lk(consoleMutex);
	std::cout << __FUNCTION__;
	std::cout << " (2000мс) - выполнена в потоке c id=" << std::this_thread::get_id() << std::endl;
}

std::vector<std::function<void()>> tasks; // каталог задач

std::function<void()> getTask(std::vector<std::function<void()>> tasks) {
	int size = tasks.size();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, size - 1);
	int taskNum = dis(gen);
	return tasks.at(taskNum); // возвращает копию задачи из каталога случайным образом
}

int main() {
	setlocale(LC_ALL, "");
	// заполнение каталога задач
	tasks.push_back(func1);
	tasks.push_back(func2);
	tasks.push_back(func3);
	tasks.push_back(func4);
	tasks.push_back([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		std::lock_guard lk(consoleMutex);
		std::cout << __FUNCTION__;
		std::cout << " (1200мс) - выполнена в потоке c id=" << std::this_thread::get_id() << std::endl;
		}
	);

	std::cout << "Демонстрация работы потокобезопасного пула задач. \n";
	std::cout << "Для завершения нажмите любую клавишу... \n \n";

	ThreadPool thread_pool(std::thread::hardware_concurrency() - 2);

	// каждую секунду запускает в пул потоков две новые задачи. пока не будет нажжата любая клавиша 
	while (true) {
		thread_pool.submit(getTask(tasks));
		thread_pool.submit(getTask(tasks));
		bool buttonPressed = false;
		auto start = std::chrono::steady_clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < 1) {
			buttonPressed = _kbhit();
			if (buttonPressed) break;
		}
		if (buttonPressed) break;
	}

	// после нажатия клавиши, деструктор пула потоков завершает все запущенные задачи 
	// и останавливает ожидающие потоки

	return 0;
}