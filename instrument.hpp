/** \file The DefaultInstrument Module
 */
#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "event.hpp"
#include "track.hpp"

/** The default instrument. A simple fixed length note instrument using a
 * a sinusoid weighted by an envelope function.
 */
class DefaultInstrument{
public:

  /** No default constructor. A DefaultInstrument must have an associated 
   * track. 
   */
  DefaultInstrument() = delete;

  /** Cannot be copied. 
   */
  DefaultInstrument(const DefaultInstrument & ) = delete;

  /** Cannot be assigned.
   */
  DefaultInstrument & operator=(const DefaultInstrument & ) = delete;

  /** Construct a Default Instrument using the specified track.
   * \param track the track to read events from. 
   */
  DefaultInstrument(const Track & track);

  /** Returns true if the DefaultInstrument has stopped processing events.
   * This can occur e.g. if all events have been consumed or a EndTrackEvent
   * has been processed.
   */
  bool halted();

  /** Return the sample after the (real) time increment. Samples the 
   * instrument based on its state after processing all events prior 
   * to this time.
   * \param deltaT the time increment in seconds.
   */
  double sample(double deltaT, int samprate);

private:

	Track trck;
	bool halt;
	double frequency(NoteEvent x);
	double envelope(double x);
	Track::ConstIteratorType it;
	Track::ConstIteratorType it_plus;
	intmax_t tempo;
	struct Notes {
		NoteEvent note;
		intmax_t tick_time;
		double on_time;
		int x;
	};

	std::vector<Notes> notes_on;
};


#endif
