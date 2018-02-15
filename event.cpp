#include "event.hpp"


TempoEvent::TempoEvent(intmax_t t)
{
	tempo = t;	
}

intmax_t TempoEvent::getTempo() const
{
	return tempo;
}

NoteEvent::NoteEvent(bool on, int8_t notenum, int8_t notevelocity)
{
	ON = on;
	NOTENUM = notenum;
	NOTEVELOCITY = notevelocity;

}

bool NoteEvent::getNoteOn()
{
	return ON;
}

int8_t NoteEvent::getNoteNumber() const
{
	return NOTENUM;
}

int8_t NoteEvent::getNoteVelocity() const
{
	return NOTEVELOCITY;
}

MIDIEvent::MIDIEvent(TempoEvent e, intmax_t ticktime)
{
	
	event_.tempo = e;
	tick_time = ticktime;
	eventType_ = tempo;
}

MIDIEvent::MIDIEvent(EndTrackEvent e, intmax_t ticktime)
{
	event_.endtrack = e;
	tick_time = ticktime;
	eventType_ = endtrack;
}

MIDIEvent::MIDIEvent(NoteEvent e, intmax_t ticktime)
{
	event_.note = e;
	tick_time = ticktime;
	eventType_ = note;
}

bool MIDIEvent::isTempoEvent() const
{
	if (eventType_ == tempo)
	{
		return true;
	}

	return false;
}

bool MIDIEvent::isEndTrackEvent() const
{
	if (eventType_ == endtrack)
	{
		return true;
	}

	return false;
}

bool MIDIEvent::isNoteEvent() const
{
	if (eventType_ == note)
	{
		return true;
	}

	return false;
}

intmax_t MIDIEvent::getTickTime() const
{
	return tick_time;
}

TempoEvent MIDIEvent::asTempoEvent() const
{
	
	return event_.tempo;
}

NoteEvent MIDIEvent::asNoteEvent() const
{
	
	return event_.note;
	
}
