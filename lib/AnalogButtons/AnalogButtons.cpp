#include "AnalogButtons.h"
#include <Arduino.h>

Button::Button(uint16_t value, void (*clickFunction)(void), void (*holdFunction)(void) , uint16_t holdDuration , uint16_t holdInterval) {
	this->value = value;
	this->duration = holdDuration;
	this->interval = holdInterval;
	this->isHeldDown = false;
	this->isClicked = false;
	this->clickFunction = clickFunction;
	this->holdFunction = holdFunction;
}

/**
 * Constructor
 * @param pin Analog Pin that will read al the data
 * @param margin Let's you have a better accuracy when press a button, avodind the wrong data collection
 * @param debounce it let you have more control when pressing the button Max = 255
 */

AnalogButtons::AnalogButtons(uint8_t pin, uint8_t margin ,uint8_t debounce) {
	this->pin = pin;
	this->margin = margin;
	this->debounce = debounce;
	this->previusTimeDebounce = 0;
}

/**
 * Add the Button to the list of buttons inside the AnalogButtons Object
 * @param button Button you want to add
 */
void AnalogButtons::add(Button button) {
	if (buttonsCount < ANALOGBUTTONS_MAX_SIZE) {
    	buttons[buttonsCount++] = button;
  	}
}

/**
 * Set the debounce yo want for the button, sometimes it works better with highter or lower, that depends on you harware button
 * @param debounce time in milliseconds of the preferred debounce
 */
void AnalogButtons::setDebounce(uint16_t debounce) {
	this->debounce = debounce;
}

/**
 * Checks the values asigned on every button with the respective +-margin for avoiding wrong data
 */
void AnalogButtons::check() {
	if (millis() - previusTimeDebounce > debounce) {

		uint16_t reading = analogRead(pin);
		for (uint8_t i = 0; i < buttonsCount; i++) {
			if (reading >= buttons[i].value - margin && reading <= buttons[i].value + margin) {

				if (buttons[i].isClicked == false) {
					buttons[i].pressed();
					previousMillis = millis();
					buttons[i].isClicked = true;

				} else {
					if (buttons[i].isHeldDown == true && ((millis() - previousMillis) > buttons[i].interval)) {
						buttons[i].held();
						previousMillis = millis();
					} else if (buttons[i].isHeldDown != true && ((millis() - previousMillis) > buttons[i].duration)) {
						buttons[i].isHeldDown = true;
						buttons[i].held();
						previousMillis = millis();
					}
				}
				previusTimeDebounce = millis();
				// The first matching button is the only one that gets triggered
				return;
			} else {
				buttons[i].isHeldDown = false;
				buttons[i].isClicked = false;
			}
		}
	}
		// If execution reaches this point then no button has been pressed during this check


}
