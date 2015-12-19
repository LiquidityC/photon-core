#include "application.h"
#include "main.h"

void setup()
{
	// Select our pins to toggle
	pinMode(red_led, OUTPUT);
	pinMode(yellow_led, OUTPUT);
	pinMode(green_led, OUTPUT);
	pinMode(blue_led, OUTPUT);
	pinMode(main_led, OUTPUT);

	// Web functions
	Spark.function("ok", okToggle);
	Spark.function("error", errorToggle);
	Spark.function("alarm", alarmToggle);
	Spark.function("download", downloadToggle);
	Spark.function("upload", uploadToggle);

	// Initial state
	resetState();
}

void loop()
{
	if (alarm) {
		runAlarm();
	} else {
		runNetwork();
	}
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
