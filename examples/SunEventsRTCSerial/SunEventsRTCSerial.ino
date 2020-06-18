#include <RTClib.h>
#include <SunEvent.h>

RTC_DS3231 rtc;

float GMT_OFFSET = -5;
float LATITUDE = 27.9719;
float LONGITUDE = -82.8265;
SunEvent ClearwaterBeach(LATITUDE, LONGITUDE, GMT_OFFSET);

void setup() {
    Serial.begin (57600);
    while (!Serial);
    rtc.begin();
}

void loop() {
    DateTime now = rtc.now();
    char timeFormat[] = "Now  MM/DD/YY hh:mm ap";
    Serial.println(now.toString(timeFormat));

    getSunEvents();
        
    delay(5000);
}

void getSunEvents() {
    DateTime nextrise = ClearwaterBeach.sunrise(rtc.now());    
    char rise_buf[] = "Rise MM/DD/YY hh:mm ap";
    Serial.println(nextrise.toString(rise_buf));

    DateTime nextset  = ClearwaterBeach.sunset(rtc.now());    
    char set_buf[] = "Set  MM/DD/YY hh:mm ap";
    Serial.println(nextset.toString(set_buf));
}
