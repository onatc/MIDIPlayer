#include <thread>
#include <iostream>
#include <QApplication>
#include "midiplayer.hpp"
#include "synthesizer.hpp"
#include "message_queue.hpp"

using namespace std;

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);

	MIDIPlayer q;
	 
	q.show();


	return app.exec();
}
