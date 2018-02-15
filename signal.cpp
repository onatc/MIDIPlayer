#include "signal.hpp"


Signal::Signal(unsigned int sampleRate)
{
	sample_rate = sampleRate;
}

unsigned int Signal::getSampleRate()
{
	return sample_rate;
}

void Signal::normalize()
{

	for (unsigned int i = 0; i < signal.size(); i++)
	{
		signal[i] = (int16_t)signal[i];
	}
	
}

void Signal::push_back(double value)
{
	signal.push_back(value);
}

std::size_t Signal::size()
{
	return signal.size();
}

double Signal::operator[](std::size_t index)
{
	return signal.at(index);
}
