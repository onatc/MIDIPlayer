#ifndef SHARED_DOUBLE_BUFFER_HPP
#define SHARED_DOUBLE_BUFFER_HPP

#include <queue>
#include <mutex>
#include <condition_variable>



class SDBuffer
{
public:

	void push(double samp);

	bool empty() const;

	bool try_pop(double &popped_value);

	void wait_and_pop(double &popped_value);

	void swap();

	void clear();

	void swap_pause(SDBuffer &x);


private:
	std::queue<double> the_queue;
	std::queue<double> write_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

#endif
