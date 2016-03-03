#include <ArrayButtons.h>
#include <Arduino.h>

Button::Button(uint16_t value, void (*clickFunction)(void), void (*holdFunction)(void) , uint16_t holdDuration , uint16_t holdInterval) {
	this->value = value;
	this->duration = holdDuration;
	this->interval = holdInterval;
	this->heldDown = false;
	this->clicked = false;
	this->clickFunction = clickFunction;
	this->holdFunction = holdFunction;
}

/**
 * Constructor
 * @param pin Analog Pin that will read al the data
 * @param margin Let's you have a better accuracy when press a button, avodind the wrong data collection
 * @param debounce it let you have more control when pressing the button Max = 255
 */

ArrayButtons::ArrayButtons(uint8_t pin, uint8_t margin ,uint8_t debounce) {
	this->pin = pin;
	this->margin = margin;
	this->debounce = debounce;
	this->previusTimeDebounce = 0;
}

/**
 * Add the Button to the list of buttons inside the ArrayButtons Object
 * @param button Button you want to add
 */
void ArrayButtons::add(Button button) {
	if (buttonsCount < ARRAYBUTTONS_MAX_SIZE) {
    	buttons[buttonsCount++] = button;
  	}
}

/**
 * Checks the values asigned on every button with the respective +-margin for avoiding wrong data
 */
void ArrayButtons::check() {
	if (millis() - previusTimeDebounce > debounce) {

		uint16_t reading = analogRead(pin);
		for (uint8_t i = 0; i < buttonsCount; i++) {
			if (reading >= buttons[i].getValue() - margin && reading <= buttons[i].getValue() + margin) {

				if (buttons[i].isClicked() == false) {
					buttons[i].pressed();
					previousMillis = millis();
					buttons[i].setClicked(true);

				} else {
					if (buttons[i].isHeldDown() == true && ((millis() - previousMillis) > buttons[i].getInterval())) {
						buttons[i].held();
						previousMillis = millis();
					} else if (buttons[i].isHeldDown() != true && ((millis() - previousMillis) > buttons[i].getDuration())) {
						buttons[i].setHeldDown(true);
						buttons[i].held();
						previousMillis = millis();
					}
				}
				previusTimeDebounce = millis();
				// The first matching button is the only one that gets triggered
				return;
			} else {
				buttons[i].setHeldDown(false);
				buttons[i].setClicked(false);
			}
		}
	}
		// If execution reaches this point then no button has been pressed during this check


}
