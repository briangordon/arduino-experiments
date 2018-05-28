// This goes high when the alarm goes off. External pulldown resistor.
int alarmPin = 3;

// Controls the transistor powering the audio indicator driver.
int buzzerControlPin = 4;

// Controls the high(er) voltage relay connected to the LED array.
int ledRelayPin = 5;

// The time that we started alarming, or 0 if we're not alarming.
unsigned long alarmStartedMillis = 0;

void setup() {
	pinMode(alarmPin, INPUT);
	pinMode(ledRelayPin, OUTPUT);
	pinMode(buzzerControlPin, OUTPUT);

	digitalWrite(ledRelayPin, LOW);
	digitalWrite(buzzerControlPin, LOW);
}

void loop() {
	int alarmGoingOff = digitalRead(alarmPin);

	if (!alarmGoingOff) {
		alarmStartedMillis = 0;
		digitalWrite(ledRelayPin, LOW);
		digitalWrite(buzzerControlPin, LOW);
		return;
	}

	if (alarmStartedMillis == 0) {
		alarmStartedMillis = millis();
	}

	// The LEDs should be on for one second, off for one second.
	if ((millis() - alarmStartedMillis) % 2000 < 1000) {
		digitalWrite(ledRelayPin, HIGH);
	} else {
		digitalWrite(ledRelayPin, LOW);
	}

	// The buzzer should be on for 200ms, off for 300ms.
	if ((millis() - alarmStartedMillis) % 500 < 200) {
		digitalWrite(buzzerControlPin, HIGH);
	} else {
		digitalWrite(buzzerControlPin, LOW);
	}
}
