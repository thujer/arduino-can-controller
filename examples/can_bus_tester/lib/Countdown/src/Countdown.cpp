/**
    Countdown
    Purpose: Countdown timer
	Call .set method to set time and check .timeout method.
	True value will be returned after time elapsed.
	If You need to monitor current timer value, its possible via .get method.

    @author Tomas Hujer
    @version 0.1 10/04/2019 
*/

#include <Arduino.h>

#include "Countdown.h"

void Countdown::set(unsigned long ms) {
	this->countdownDelay = ms;
	this->countdownOnStart = millis();
}

 unsigned long Countdown::get() {
	 return (this->countdownDelay - (millis() - this->countdownOnStart));
 }

bool Countdown::timeout() {

	if(millis() - this->countdownOnStart >= this->countdownDelay) {
		return true;
	} else {
		return false;
	}
}
