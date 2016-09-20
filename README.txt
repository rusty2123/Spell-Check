As you answer these questions, remember to make explicit references to your
code.  For example, don't just say, "I did such and so;" instead say, "I did
such and so on main.cpp:13-15."  You may use the notation file:line or
file:line:col to refer to places in your code.

1.  Explain why your queue is thread-safe.  Tell, for each operation supported
    by the queue (i.e., public method), what steps you took to make it
    thread-safe.  Tell why are sure no one may bypass your safeguards to cause
    race conditions.

	The safe_queue's copy and move constructors in safe_queue.cpp: are thread safe because
	they hold an exclusive lock while the entire operation takes place.

	dequeue() in safe_queue.cpp: is thread safe because it obtains an exclusive lock while 
	it waiting on the condition variable for something to be enqueued and when it removes the front of the queue. Multiple
	threads also cannot cause problems by doing this at the same time because enqueue() will only call notify_one(). This
	only allows one thread to dequeue at a time.
	
	enqueue(T t) in safe_queue.cpp: is thread safe because it obtains an exclusive lock when called. After pushing data into
	the queue, it will unlock the mutex and call notify_one(). Calling notify_one() AFTER the mutex is unlocked is important
	because it is possible for another thread to be notified and try to obtain the queue before the queue is unlocked.
	

2.  How many threads did you create to handle requests?  Why did you select
    this number?  Be sure to reference where in your code you create the
    threads.
	
	We created as many threads as the user's computer would allow. If that number is 
	unspecified then we only create two. We did this on main.cpp:69-72. We selected
	these numbers to ensure the highest multi-processing capabilities possible, and
	to always make sure the program is multi-threaded.

3.  You should have had at least three global variables in your program: the
    word list, the distance table, and the cache.  List, for each of these
    variables, what steps you took to ensure that all accesses to them were
    thread-safe.  Explain your reasoning.

	The word list is const in spell.cpp:38, so that ensures that it is a read-only variable. Read- 
	only variables are in no danger to multiple threads. We made the distance table in spell.cpp:60
	thread_local so that each thread got its own. The thread table does not need to 
	be shared among threads, and it would interfere with other threads replies if 
	it were. 
	The cache however had to be shared among threads because the data had to be saved 
	for future requests else the cache is pointless. It would also be more efficient
	for every thread to add to the cache so that other threads may utilize requests
	which have already been handled. Therefore, a thread-safe map was created in safe_map.cpp. 
	It used	shared_locks for keeping other threads from modifying the data while a thread
	read the data. It would use exclusive locks to write to the map to ensure that other
	threads neither read un-updated information or wrote at the same time.

4.  Explain why your map is thread-safe.  Tell, for each operation supported
    by the map (i.e., public method), what steps you took to make it
    thread-safe.  Tell why you can be sure no one may bypass your safeguards
    to cause race conditions.

	The safe_map's copy and move constructors in safe_map.cpp:17-25 are thread safe because
	they hold an exclusive lock while the entire operation takes place.

	find(KeyType key) and get_value(KeyType key) in safe_map.cpp:27-43 are thread safe because they hold shared locks.
	They use shared locks because they only read from the map, and this ensuures that no other
	threads modify the data while it is read. 

	add_item(KeyType key, ValueType value) and remove_item(KeyType key) in safe_map.cpp: are 
	thread safe because they hold exclusive locks when they modify data in the map. This ensures
	that no other threads read the un-updated data or modify it at the same time.	

5.  Where in your code do you read and update the cache?  How much
    syncrhonization do you expect for these operations: how likely are they to
    block?
	
	We read and update the code in main.cpp:36-39. We expect high amounts of blocking when
	using the map because whenever the threads do not find their word, they will write.
	However, as the cache increases in size the threads will block eachother less because 
	they will more likely find their word and only have to read the data. This is due to the
	shared_lock allowing any amount of readers. The clean-up thread will actually increase
	the amount of blocking because it decreases the size of the cache.

6.  Explain your strategy for removing entries from the cache.  Explain why
    your approach is thread-safe.
	
	

7.  I asked you to create a second map to store timestamps.  Another option
    would have been to store the timestamps in the cache along with the
    results.  Explain the difference between these two operations in terms of
    synchronization: is one approach more likely to cause blocking than
    another?

8.  (Optional) Tell me something interesting you learned from doing this
    project.
	
	You can store void functions into a queue. However, they have to be queued as lambdas.
	Also, it has taught me that the member initialization list is necessary sometimes.

9.  (Optional) Tell me something you didn't like about this project.  Was it
    something you think should not changed about the project, or just
    something inherently difficult about the project?

	I do not like this extensive write up. I like to write code. 

10. (Optional) Tell me something about your implementation that you are proud
    of.  For example, something that was difficult to do but that you
    eventually figured out; something that you feel you did particularly well;
    or something that you did that was beyond what was asked.