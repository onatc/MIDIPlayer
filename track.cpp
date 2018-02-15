#include "track.hpp"


Track::Track(intmax_t ticks_per_beat)
{
	tick_beat = ticks_per_beat;
}

intmax_t Track::getTicksPerBeat() const
{
	return tick_beat;
}

std::list<MIDIEvent>::const_iterator Track::begin() const
{
	return track.begin();
}

std::list<MIDIEvent>::const_iterator Track::end() const
{
	return track.end();
}

void Track::addEndEvent(intmax_t tick_time)
{
	MIDIEvent end(EndTrackEvent(), tick_time);
	track.push_back(end);
}

void Track::addTempoEvent(intmax_t tempo, intmax_t tick_time)
{
	MIDIEvent temp(TempoEvent(tempo), tick_time);
	track.push_back(temp);
}

void Track::addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time)
{
	bool on;
	if (code == 8) { on = false;}
	if (code == 9) { on = true; }
	else { on = false; }
	MIDIEvent note_ev(NoteEvent(on, data1, data2), tick_time);
	track.push_back(note_ev);

}
