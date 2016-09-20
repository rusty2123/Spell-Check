#include <mutex>
#include <shared_mutex>
#include <map>

using namespace std;

template <typename KeyType, typename ValueType>
class safe_map
{
	map<KeyType, ValueType> the_map;
	shared_timed_mutex shared_mutex;

public:

	safe_map() = default;

	safe_map(const safe_map& sm) {
		lock_guard<shared_timed_mutex> lk(sm.shared_mutex);
		the_map = sm.the_map;
	}

	safe_map(safe_map&& sm) {
		lock_guard<shared_timed_mutex> lk(shared_mutex);
		the_map = move(sm.the_map);
	}

	bool find(KeyType key)
	{
		shared_lock<shared_timed_mutex> lk(shared_mutex);
		if (the_map.find(key) != the_map.end())
			return true;
		else
			return false;
	}

	auto get_value(KeyType key)
	{
		if (find(key))
		{
			shared_lock<shared_timed_mutex> lk(shared_mutex);
			return the_map[key];
		}
	}

	void add_item(KeyType key, ValueType value)
	{
		lock_guard<shared_timed_mutex> lk(shared_mutex);
		the_map[key] = value;
	}

	auto remove_item(KeyType key)
	{
		if (find(key))
		{
			lock_guard<shared_timed_mutex> lk(shared_mutex);
			return the_map.erase(the_map.find(key));
		}
	}

	auto operator[] (const KeyType& key)
	{
		shared_lock<shared_timed_mutex> lk(shared_mutex);
		return the_map[key];
	}

	auto begin()
	{
		shared_lock<shared_timed_mutex> lk(shared_mutex);
		return the_map.begin();
	}

	auto end()
	{
		shared_lock<shared_timed_mutex> lk(shared_mutex);
		return the_map.end();
	}

	void lock_shared()
	{
		shared_mutex.lock_shared();
	}

	void unlock_shared()
	{
		shared_mutex.unlock_shared();
	}
};