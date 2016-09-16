#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;



template <typename T>
class safe_queue {
	queue<T> work_queue;
	mutable mutex the_mutex;

public:

	safe_queue() = default;

	safe_queue(const safe_queue& sq) {
		lock_guard<mutex> lk(sq.the_mutex);
		the_queue = sq.the_queue;
	}

	safe_queue(safe_queue&& sq) {
		lock_guard<mutex> lk(the_mutex);
		the_queue = move(sq.the_queue);
	}

	bool dequeue(T& t) {
		lock_guard<mutex> lk(the_mutex);
		if (the_queue.empty()) {
			return false;
		}
		else {
			t = move(the_queue.front());
			the_queue.pop();
			return true;
		}
	}

	unique_ptr<T> dequeue() {
		lock_guard<mutex> lk(the_mutex);
		if (the_queue.empty()) {
			return null_ptr;
		}
		else {
			T* pt = new T(move(the_queue.front()));
			the_queue.pop();
			return make_unique(pt);
		}
	}

	void enqueue(T t) {
		lock_guard<mutex> lk(the_mutex);
		the_queue.push(move(t));
	}
};

safe_queue<int> my_queue;

void some_thread() {
	int i;

	if (my_queue.pop(i)) {
		cout << "Popped: " << i << endl;
	}
}

int main() {
	my_queue.push(7);
}