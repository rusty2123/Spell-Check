#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
using namespace std;

template <typename T>
class safe_queue {
	queue<T> work_queue;
	mutable mutex the_mutex;
	condition_variable cv;

public:

	safe_queue() = default;

	safe_queue(const safe_queue& sq)
	{
		lock_guard<mutex> lk(sq.the_mutex);
		work_queue = sq.the_queue;
	}

	safe_queue(safe_queue&& sq)
	{
		lock_guard<mutex> lk(the_mutex);
		work_queue = move(sq.the_queue);
	}

	auto dequeue()
	{
		unique_lock<mutex> lk(the_mutex);
		cv.wait(lk, [this]() { return !work_queue.empty(); });
		T front = move(work_queue.front());
		work_queue.pop();
		lk.unlock();
		return front;
	}

	void enqueue(T t)
	{
		the_mutex.lock();
		work_queue.push(move(t));
		the_mutex.unlock();
		cv.notify_one();
	}
};