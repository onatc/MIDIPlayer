#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP

#include <QWidget>
#include <QState>
#include <QObject>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QRadioButton>
#include <QAudioOutput>
#include <QSlider>
#include "message_queue.hpp"
#include "shared_double_buffer.hpp"
#include "instrument.hpp"
#include "wav.hpp"
#include "signal.hpp"
#include "track.hpp"
#include "midi.hpp"
#include "synthesizer.hpp"
#include <thread>
#include <cmath>
#include <random>



// define class MIDIPlayer here
class MIDIPlayer : public QWidget {
	Q_OBJECT
	
public:
	MIDIPlayer(QWidget * parent = nullptr);

private slots:
	void inputBrowse();
	void playF();
	void pauseF();
	void stopF();
	void muteF();
	void volumeF();
	void handleNotify();


private:

	std::thread *t;
	MessageQueue *msqe;
	SDBuffer *my_buffer;

	QGridLayout *layout;
	QLineEdit *input_path;
	QString inputfile;
	std::string input_value;
	QPushButton *input_browse;


	QPushButton *play;
	QPushButton* pause;
	QPushButton *stop;
	QPushButton *mute;
	QSlider *slider;
	QAudioOutput *audio;
	QIODevice *device;
	std::size_t buffer_size;
	std::vector<char> my_vector;


	double volume;
	int sampleRate;


};
  
#endif
