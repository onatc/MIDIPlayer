#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include <test_config.hpp>
#include "midiplayer.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void testInput();
  void testStop();
  void testPlay();
  void testPause();
  void testMute();
  void testSlider();
  
private:

  MIDIPlayer widget;

};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QPushButton *>("play");
    QVERIFY2(w, "Could not find play button");
  }

  {
    auto w = widget.findChild<QPushButton *>("pause");
    QVERIFY2(w, "Could not find pause button");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("stop");
    QVERIFY2(w, "Could not find stop button");
  }

  {
    auto w = widget.findChild<QSlider *>("volume");
    QVERIFY2(w, "Could not find volume slider");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("mute");
    QVERIFY2(w, "Could not find mute button");
  }
}

QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

void TestMIDIPlayer::testInput()
{
	auto w = widget.findChild<QLineEdit *>("inputpath");
	w->setText("C:\\Users");
	std::string x = w->text().toStdString();
	QVERIFY2(x == "C:\\Users", "Wrong input path");
}

void TestMIDIPlayer::testStop()
{
	auto w = widget.findChild<QPushButton *>("stop");

	w->clicked(true);
	std::string x = w->text().toStdString();
	QVERIFY2(!w->isEnabled(), "Stop is enabled at the beginning!");
}

void TestMIDIPlayer::testPlay()
{
	
	auto w = widget.findChild<QPushButton *>("play");
	auto y = widget.findChild<QPushButton *>("pause");
	auto z = widget.findChild<QPushButton *>("stop");
	auto a = widget.findChild<QLineEdit *>("inputpath");

	a->setText(QString::fromStdString(TEST_FILE_DIR + "/test1.mid"));

	QVERIFY2(w->isEnabled(), "Play is not enabled at the beginning!");
	QVERIFY2(!y->isEnabled(), "Stop is enabled at the beginning!");
	QVERIFY2(!z->isEnabled(), "Pause is enabled at the beginning!");
	QVERIFY2(a->isEnabled(), "Input path is not enabled at the beginning!");

	w->clicked(true);

	QVERIFY2(!w->isEnabled(), "Play is enabled after pressed!");
	QVERIFY2(z->isEnabled(), "Stop is not enabled after play!");
	QVERIFY2(y->isEnabled(), "Pause is not enabled after play!");
	QVERIFY2(!a->isEnabled(), "Input path is enabled after play!");

	z->clicked(true);
}

void TestMIDIPlayer::testPause()
{
	auto w = widget.findChild<QPushButton *>("pause");

	w->clicked(true);

	QVERIFY2(!w->isEnabled(), "Pause is enabled at the beginning!");
}

void TestMIDIPlayer::testMute()
{
	auto x = widget.findChild<QSlider *>("volume");
	auto w = widget.findChild<QPushButton *>("mute");

	QVERIFY2(x->value() == 50, "Initial volume is wrong");

	w->clicked(true);

	QVERIFY2(x->value() == 0, "Mute isn't working!");
}

void TestMIDIPlayer::testSlider()
{
	auto w = widget.findChild<QSlider *>("volume");
	w->setValue(25);
	QVERIFY2(w->value() == 25, "Correct volume (25)");
	w->setValue(75);
	QVERIFY2(w->value() == 75, "Correct volume (75)");
	w->setValue(100);
	QVERIFY2(w->value() == 100, "Correct volume (100)");
	w->setValue(120);
	QVERIFY2(w->value() == 100, "Correct volume (120)");

}
