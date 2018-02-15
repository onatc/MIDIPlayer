#include "midiplayer.hpp"
#include <thread>
#include "synthesizer.hpp"
#include <QButtonGroup>
#include <QWidget>
#include <QLayout>
#include <QRadioButton>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QSlider>
#include <QMessageBox>
#include <fstream>

using namespace std;

// Implement class MIDIPlayer here
MIDIPlayer::MIDIPlayer(QWidget * parent) {

	msqe = new MessageQueue;
	my_buffer = new SDBuffer;
	t = new std::thread(&Synthesizer::synthesize, msqe, my_buffer);

	layout = new QGridLayout;

	QGroupBox *midi_input = new QGroupBox(tr("MIDI Input"), this);
	QGridLayout *input_layout = new QGridLayout(midi_input);
	input_browse = new QPushButton(tr("Browse"), midi_input);
	input_path = new QLineEdit(midi_input);
	input_layout->addWidget(input_path, 0, 0);
	input_layout->addWidget(input_browse, 0, 1);


	QGroupBox *audio_cntrl = new QGroupBox(tr("Audio Controls"), this);
	play = new QPushButton(tr("Play"), audio_cntrl);
	pause = new QPushButton(tr("Pause"), audio_cntrl);
	stop = new QPushButton(tr("Stop"), audio_cntrl);
	mute = new QPushButton(tr("Mute"), audio_cntrl);
	slider = new QSlider(Qt::Horizontal, this);
	QLabel *vol = new QLabel("Volume: ", this);
	slider->setRange(0, 100);
	slider->setFixedWidth(100);
	slider->setValue(50);

	QGridLayout *audio_layout = new QGridLayout(audio_cntrl);
	audio_layout->addWidget(play, 0, 0);
	audio_layout->addWidget(pause, 0, 1);
	audio_layout->addWidget(stop, 0, 2);
	audio_layout->addWidget(vol, 1, 0);
	audio_layout->addWidget(slider, 1, 1);
	audio_layout->addWidget(mute, 1, 2);

	connect(input_browse, SIGNAL(clicked()), this, SLOT(inputBrowse()));
	connect(play, SIGNAL(clicked()), this, SLOT(playF()));
	connect(pause, SIGNAL(clicked()), this, SLOT(pauseF()));
	connect(stop, SIGNAL(clicked()), this, SLOT(stopF()));
	connect(mute, SIGNAL(clicked()), this, SLOT(muteF()));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(volumeF()));

	layout->addWidget(midi_input);
	layout->addWidget(audio_cntrl);

	input_path->setObjectName("inputpath");
	play->setObjectName("play");
	pause->setObjectName("pause");
	stop->setObjectName("stop");
	slider->setObjectName("volume");
	mute->setObjectName("mute");

	this->setLayout(layout);

	//Disable
	pause->setDisabled(true);
	stop->setDisabled(true);

	//Audio
	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

	auto rates = info.supportedSampleRates();

	sampleRate = rates.at(0);
	for (int i = 1; i < rates.size(); ++i) {
		if (rates.at(i) < sampleRate)
			sampleRate = rates.at(i);
	}

	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!info.isFormatSupported(format)) {
		qWarning()
			<< "Raw audio format not supported by backend, cannot play audio.";
	}
	else {
		audio = new QAudioOutput(format);
		connect(audio, SIGNAL(notify()), this, SLOT(handleNotify()));

		buffer_size = audio->bufferSize();
		buffer_size = buffer_size > 0 ? buffer_size : 1024;

		audio->setNotifyInterval(25);

		device = audio->start();
		handleNotify();
	}


}

void  MIDIPlayer::handleNotify() {

	int bytesFree = audio->bytesFree();
	int bytesToWrite = bytesFree > buffer_size ? buffer_size : bytesFree;
	int samplesToWrite = bytesToWrite >> 1;
	bytesToWrite = 2 * samplesToWrite;

	my_vector.reserve(bytesToWrite);

	double s = 0;
	for (int i = 0; i < samplesToWrite; ++i) {
		if (my_buffer->empty()) {
			my_buffer->swap();
		}
		my_buffer->try_pop(s);

		//cout << s << endl;

		int16_t temp = static_cast<int16_t>(lround(s));
		char *byte1 = reinterpret_cast<char *>(&temp);
		char *byte2 = ++byte1;
		my_vector.push_back(*byte1);
		my_vector.push_back(*byte2);
	}

	if (bytesToWrite > 0) {
		device->write(my_vector.data(), bytesToWrite);
	}

	my_vector.clear();
}



void MIDIPlayer::inputBrowse()
{
	inputfile = QFileDialog::getOpenFileName(this, QDir::currentPath());
	input_path->setText(inputfile);
}


void MIDIPlayer::playF()
{
	input_value = input_path->text().toStdString();
	std::ifstream trial(input_value, std::ios::binary);
	if (input_value.length() > 4) {
		if (input_value.substr(input_value.length() - 4, 4) != ".mid")
		{
			int ret = QMessageBox::information(this, tr("Invalid File"),
				tr("Invalid file name, only MIDI files are accepted."),
				QMessageBox::Ok | QMessageBox::Default);

		}
		else {
			if (!trial.is_open())
			{
				int ret = QMessageBox::information(this, tr("Invalid File"),
					tr("Invalid file name, file cannot be opened."),
					QMessageBox::Ok | QMessageBox::Default);

			}
			else {
				msqe->push("play");
				msqe->push(input_path->text().toStdString());
				msqe->push(std::to_string(sampleRate));
				stop->setDisabled(false);
				pause->setDisabled(false);
				play->setDisabled(true);
				input_path->setDisabled(true);
				input_browse->setDisabled(true);
			}
			
		}
	}
	else
	{
		int ret = QMessageBox::information(this, tr("Invalid File"),
			tr("Invalid file name, only MIDI files are accepted."),
			QMessageBox::Ok | QMessageBox::Default);

	}
	

}

void MIDIPlayer::pauseF()
{
	msqe->push("pause");
	play->setDisabled(false);
	stop->setDisabled(false);
	input_path->setDisabled(true);
	input_browse->setDisabled(true);
	
}

void MIDIPlayer::stopF()
{
	msqe->push("stop");
	play->setDisabled(false);
	stop->setDisabled(true);
	pause->setDisabled(true);
	input_path->setDisabled(false);
	input_browse->setDisabled(false);

}

void MIDIPlayer::muteF()
{
	msqe->push("mute");
	slider->setValue(0);
}

void MIDIPlayer::volumeF()
{
	volume = slider->value() / 100.0;
	audio->setVolume(volume);
}



