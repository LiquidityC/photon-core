#include "application.h"
#include "time.h"
#include "main.h"

void setup()
{
	// Select our pins to toggle
	pinMode(red_led, OUTPUT);
	pinMode(yellow_led, OUTPUT);
	pinMode(green_led, OUTPUT);
	pinMode(blue_led, OUTPUT);
	pinMode(main_led, OUTPUT);
	pinMode(light_sensor_power, OUTPUT);
	pinMode(light_sensor_read, INPUT);
	pinMode(buzzer, OUTPUT);

	// Web functions
	Particle.function("ok", okToggle);
	Particle.function("error", errorToggle);
	Particle.function("alarm", alarmToggle);
	Particle.function("download", downloadToggle);
	Particle.function("upload", uploadToggle);

	// Fix time
	Time.zone(1);

	// Initial state
	digitalWrite(light_sensor_power, HIGH);
	resetState();

	Particle.publish("photon", "started", 60, PRIVATE);
}

void loop()
{
	if (alarm) {
		runAlarm();
	} else {
		runNetwork();
	}
	checkBuzzer();
}

void runNetwork()
{
	if (upload) {
		digitalWrite(blue_led, HIGH);
	}
	if (download) {
		digitalWrite(yellow_led, HIGH);
	}
	delay(100);
	if (upload) {
		digitalWrite(blue_led, LOW);
	}
	if (download) {
		digitalWrite(yellow_led, LOW);
	}
	delay(100);
}

void runAlarm()
{
	digitalWrite(green_led, HIGH);
	digitalWrite(red_led, HIGH);
	digitalWrite(yellow_led, HIGH);
	digitalWrite(blue_led, HIGH);
	digitalWrite(main_led, HIGH);
	delay(250);
	digitalWrite(green_led, LOW);
	digitalWrite(red_led, LOW);
	digitalWrite(yellow_led, LOW);
	digitalWrite(blue_led, LOW);
	digitalWrite(main_led, LOW);
	delay(250);
}

int downloadToggle(String command)
{
	if (command.startsWith("on")) {
		download = true;
	} else {
		download = false;
	}
	return 1;
}

int uploadToggle(String command)
{
	if (command.startsWith("on")) {
		upload = true;
	} else {
		upload = false;
	}
	return 1;
}

int okToggle(String command)
{
	resetState();
	digitalWrite(green_led, HIGH);
	return 1;
}

int errorToggle(String command)
{
	resetState();
	digitalWrite(red_led, HIGH);
	return 1;
}

int alarmToggle(String command)
{
	resetState();
	alarm = true;
	return 1;
}

void resetState()
{
	alarm = false;
	download = false;
	upload = false;

	digitalWrite(red_led, LOW);
	digitalWrite(yellow_led, LOW);
	digitalWrite(green_led, LOW);
	digitalWrite(blue_led, LOW);
	digitalWrite(main_led, LOW);
}

void checkBuzzer()
{
	int hour, weekday;
	bool light_on, buzzer_on;

	hour = Time.hour();
	weekday = Time.weekday();
	light_on = analogRead(light_sensor_read) > 1000;
	buzzer_on = pinReadFast(buzzer) == HIGH;

	if (weekday > 0 && weekday < 6) {
		if (!buzzer_on && hour >= 23 && light_on) {
			digitalWrite(buzzer, HIGH);
			buzzer_on = true;
		} else if (buzzer_on && !light_on) {
			digitalWrite(buzzer, LOW);
			buzzer_on = false;
		}
	} else {
		if (!buzzer_on && hour == 0 && light_on) {
			digitalWrite(buzzer, HIGH);
			buzzer_on = true;
		} else if (buzzer_on && !light_on) {
			digitalWrite(buzzer, LOW);
			buzzer_on = false;
		}
	}
}
