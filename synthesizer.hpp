/** \file Synthesizer Module
 */
#ifndef SYNTHESIZER_HPP
#define SYNTHESIZER_HPP

#include "track.hpp"
#include "signal.hpp"
#include "instrument.hpp"
#include "message_queue.hpp"
#include "shared_double_buffer.hpp"
#include <thread>

/** \fn Synthesize a Track into a Signal.
 * Returned signal is empty on failure.
 * \param track the track to synthesize.
 */
class Synthesizer {
public:
	static void synthesize(MessageQueue *msg, SDBuffer *buffer);
};


#endif
