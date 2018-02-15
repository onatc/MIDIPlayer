#include "shared_double_buffer.hpp"


#include "message_queue.hpp"


void SDBuffer::push(double samp)
{
	std::unique_lock<std::mutex> lock(the_mutex);
	write_queue.push(samp);
	lock.unlock();
	the_condition_variable.notify_one();
}


bool SDBuffer::empty() const
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return the_queue.empty();
}



bool SDBuffer::try_pop(double &popped_value)
{
	std::lock_guard<std::mutex> lock(the_mutex);
	if (the_queue.empty())
	{
		return false;
	}
	popped_value = the_queue.front();
	the_queue.pop();
	return true;
}


void SDBuffer::wait_and_pop(double &popped_value)
{
	std::unique_lock<std::mutex> lock(the_mutex);
	while (the_queue.empty())
	{
		the_condition_variable.wait(lock);
	}

	popped_value = the_queue.front();
	the_queue.pop();

}

void SDBuffer::swap()
{
	std::unique_lock<std::mutex> lock(the_mutex);
	the_queue.swap(write_queue);

	lock.unlock();
	the_condition_variable.notify_one();
}

void SDBuffer::clear()
{
	std::queue<double> e1, e2;
	std::swap(the_queue, e1);
	std::swap(write_queue, e2);
}

void SDBuffer::swap_pause(SDBuffer & x)
{
	std::unique_lock<std::mutex> lock(the_mutex);

	std::swap(the_queue, x.the_queue);
	std::swap(write_queue, x.write_queue);

	lock.unlock();
	the_condition_variable.notify_one();
}


