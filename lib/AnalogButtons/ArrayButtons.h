#ifndef __ARRAYBUTTONS_H
#define __ARRAYBUTTONS_H

/*
 ArrayButtons

 In order to reduce the number of pins used by some projects, sketches can use
 this library to wire multiple buttons to one single analog pin.
 You can register a call-back function which gets called when a button is
 pressed or held down for the defined number of seconds.

 Minimum hold duration (time that must elapse before a button is considered
 being held) and hold interval (time that must elapse between each activation
 of the hold function) can both be configured.

 By default max number of buttons per pin is limited to 5 for memory
 consumption reasons, but it can be controlled defining the
 ARRAYBUTTONS_MAX_SIZE macro _before_ including this library.

 This work is largely inspired by the AnalogButtons library available in the
 Arduino Playground library collection.

 */
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef ARRAYBUTTONS_MAX_SIZE
#define ARRAYBUTTONS_MAX_SIZE 5
#endif

class Button {
private:
	uint16_t value;
	// hold information
	uint16_t duration;
	uint16_t interval;
	boolean heldDown;
	boolean clicked;


	void (*clickFunction)(void);
	void (*holdFunction)(void);

public:

	Button() {};
	Button(uint16_t value, void (*clickFunction)(void), void (*holdFunction)(void) = 0, uint16_t holdDuration = 750, uint16_t holdInterval = 250);

	// Override these function if you want
	inline void pressed() {
		if (clickFunction)
			(*clickFunction)();
	}

	// Override these function if you want
	inline void held() {
		if (holdFunction)
			(*holdFunction)();
	}


	//Getters
	inline uint16_t getValue(){
		return value;
	}

	inline uint16_t getDuration(){
		return duration;
	}

	inline uint16_t getInterval(){
		return interval;
	}

	inline bool isHeldDown(){
		return heldDown;
	}

	inline bool isClicked(){
		return clicked;
	}

	//Setters
	inline void setDuration(uint16_t duration){
		this->duration = duration;
	}

	inline void setInterval(uint16_t interval){
		this->interval = interval;
	}

	inline void setHeldDown(bool heldDown){
		this->heldDown = heldDown;
	}

	inline void setClicked(bool clicked){
		this->clicked = clicked;
	}

};




class ArrayButtons {
private:
	uint32_t previousMillis;
	uint8_t margin;
	uint8_t debounce;
	uint32_t previusTimeDebounce;

	// AnalogPin
	uint8_t pin;

	uint8_t buttonsCount;
	Button buttons[ARRAYBUTTONS_MAX_SIZE];

public:

	ArrayButtons(uint8_t pin, uint8_t margin = 20, uint8_t debounce = 100);

	void add(Button button);
	void check();

	inline void setDebounce(uint8_t debounce = 60){
		this->debounce = debounce;
	}
};

#endif
