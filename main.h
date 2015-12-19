/*
 * Functions
 */
void resetState();
void runAlarm();
void runNetwork();

int okToggle(String);
int errorToggle(String);
int uploadToggle(String);
int downloadToggle(String);
int alarmToggle(String);

/*
 * Variables
 */

// LEDs
int red_led = A0;
int yellow_led = A1;
int green_led = D0;
int blue_led = D1;
int main_led = D7;

// Flags
bool alarm = false;
bool upload = false;
bool download = false;
