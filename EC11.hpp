//
// EC-11 Rotary Encoder Helper Library for Arduino.
// Copyright (C) 2016, Aleh Dzenisiuk. 
// http://github.com/aleh/ec11
//

#include <arduino.h>

/** To wrap a serie of EC-11 encoder events, such as "clock-wise rotation by 5 steps". */
struct EC11Event {

	enum Type { 
		None = 0, 
		StepCW, 
		StepCCW 
	};

	/** Rotation direction. */
	Type type;

	/** Number of steps in the same direction. */
	uint8_t count;
	
	EC11Event() : type(None), count(0) {};
};

/**
 * Helps with reading of EC-11 kind of rotary encoders. 
 * Does not depend on concrete pin numbers, so can be fed from an interrupt handler or by simple polling.
 */
class EC11 {
	
private:

	// Here we store last 4 pin states, as pairs of bits, with the least significant ones 
	// representing the most recent state.
	uint8_t lastPinStates;
	
	// The latest not yet read event.
	EC11Event event;
	
	/** Called internally every time we see a quad of pin states representing a single defined step. */
	void addEvent(EC11Event::Type type) {
		
		if (event.type == type) {
			
			// Similar event, consolidate with the recent one by incrementing the counter, saturating it, 
			// if needed to avoid the overflow.
			if (event.count != 0xFF) {
				event.count++;
			}
			
		} else {

			// New direction, restart the step counter.
			event.type = type;
			event.count = 1;
		}
	}
				
public:

	EC11() 
		: lastPinStates(0) {};

	/** 
	 * Must be called either from an interrupt handler associated with pin changes, 
	 * or periodically from a polling loop. 
	 * The current states of both pins should be passed here. 
	 * Note that we expect that the default state of both pins is HIGH when the encoder is at a stable point.
	 */
	void checkPins(bool pinAState, bool pinBState) {
		
		uint8_t state = (pinBState << 1) | pinAState;
		if (state != (lastPinStates & 0x3)) {
		
			// OK, the state of pins has changed compared to the last known, let's record it.
			
			lastPinStates = (lastPinStates << 2) | state;
		
			// If we see a sequence of codes corresponding to a single step, then let's record an event.
			if (lastPinStates == 0b10000111) {
				addEvent(EC11Event::StepCCW);
			} else if (lastPinStates == 0b01001011) {
				addEvent(EC11Event::StepCW);
			}
		}
	}

	/** 
	 * This should be called from a normal event loop periodically to check if there are new events coming 
	 * from the encoder. When true is returned, then the EC11Event structure the `e` pointer is filled with 
	 * the most recent event (which is then marked as "read" and won't be returned the next time).
	 */
	bool read(EC11Event *e) {

		// checkPins() above might be called from an interrupt handler, so we need to make sure we won't access the event at the same time.
		noInterrupts();
		
		if (event.count == 0) {
			// Well, no events yet.
			interrupts();
			return false;
		}

		// OK, got one, let's copy it.
		*e = event;
		
		// Let's reset the counter, so only new events will be seen the next time.
		event.count = 0;
		
		// We assume this function is always called from a normal interrupt-enabled state, so simply re-enabling them.
		interrupts();
	
		return true;
	}
};
