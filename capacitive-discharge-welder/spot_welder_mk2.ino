#define PIN_GATE1 9
#define PIN_GATE2 10
#define PIN_GATE3 11
#define PIN_GATE4 12
#define PIN_LED 13
#define PIN_PEDAL 14

// The optos are inverting so we have to invert the outputs.
#define GATE_ON 0
#define GATE_OFF 1

// We only want the welder to fire *once* after depressing the pedal, so this variable tracks 
// whether the welder has already fired. It's reset when the pedal is lifted back up.
int coolingDown = false;

void setup() {
	// Foot pedal input
	pinMode(PIN_PEDAL, INPUT_PULLUP);

	// Set up the H-bridge to state 7 *before* enabling the pins as outputs, in case the capacitor 
	// bank is already attached.
	digitalWrite(PIN_GATE1, GATE_OFF);
	digitalWrite(PIN_GATE2, GATE_OFF);
	digitalWrite(PIN_GATE3, GATE_ON);
	digitalWrite(PIN_GATE4, GATE_OFF);
	pinMode(PIN_GATE1, OUTPUT);
	pinMode(PIN_GATE2, OUTPUT);
	pinMode(PIN_GATE3, OUTPUT);
	pinMode(PIN_GATE4, OUTPUT);	
}

void loop() {
	int pedalDepressed = !digitalRead(PIN_PEDAL);

	// Set the LED to half duty cycle iff the pedal is pressed.
	analogWrite(PIN_LED, pedalDepressed << 7);

	if (!pedalDepressed) {
		coolingDown = false;
		return;
	}

	if (!coolingDown) {
		for (int i = 0; i < 10; i++) {
			pulse();
		}
		delay(20);
		for (int i = 0; i < 10; i++) {
			pulse();
		}

		coolingDown = true;
	}
}

void pulse() {
	// The high-side driver can't switch its outputs very quickly so we have to be careful to 
	// account for the time that it takes for the driver to slew up/down to the desired state. +
	// Fortunately, the rise/fall time is approximately the same as the time it takes the 
	// Arduino to perform a single digitalWrite, simplifiying the timing.
	//
	// Because we're driving a large transformer, the H-bridge is going to experience a 
	// significant inductive kickback. The exact order of toggling the gates is chosen to provide 
	// a freewheeling path for the kickback current. See timing.png and refer to the labels.
	digitalWrite(PIN_GATE2, GATE_ON);	// State 0
	delayMicroseconds(3);
	digitalWrite(PIN_GATE2, GATE_OFF);	// State 1
	digitalWrite(PIN_GATE4, GATE_ON);	// State 2
	digitalWrite(PIN_GATE3, GATE_OFF);	// State 3
	digitalWrite(PIN_GATE1, GATE_ON);	// State 4
	delayMicroseconds(3);
	digitalWrite(PIN_GATE1, GATE_OFF);	// State 5
	digitalWrite(PIN_GATE3, GATE_ON);	// State 6
	digitalWrite(PIN_GATE4, GATE_OFF);	// State 7

	// try leaving it in state 6?
}