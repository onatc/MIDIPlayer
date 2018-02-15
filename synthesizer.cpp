#include "synthesizer.hpp"
#include "midi.hpp"

using namespace std;


void Synthesizer::synthesize(MessageQueue * msg, SDBuffer * buffer)
{
	SDBuffer temp;
	while (true) {
		string filename;
		string command;

		msg->wait_and_pop(command);		//wait until play
		if (command == "play") {
			if (!temp.empty()) {
				buffer->clear();
				temp.swap_pause(*buffer);

			}
			msg->wait_and_pop(filename);
			TrackListType tl = readMIDIFromFile(filename);
			Track t = tl.front();
			DefaultInstrument ins(t);
			double n = 0;
			double sample = 0;
			string smp_rate;
			msg->wait_and_pop(smp_rate);
			int sample_rate = std::stoi(smp_rate);
			
			while (command == "play") {

				msg->try_pop(command);

				if (command == "stop") {
					buffer->clear();
					temp.clear();
					break;
				}

				if (command == "pause") {
					buffer->swap_pause(temp);
					break;
				}

				double delta = n / sample_rate;
				sample = ins.sample(delta, sample_rate);
				sample = (int16_t)sample;
				//cout << sample << endl;
				buffer->push(sample);
				n++;

			}

		}

		if (command == "stop") {
			buffer->clear();
			temp.clear();

		}
	}
}
