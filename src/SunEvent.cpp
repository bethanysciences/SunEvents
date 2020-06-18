/*  SunEvent.cpp
    2020-06-16
    Get sunrise and sunset DateTime object
    Created by Bob Smith bob@bethanysciences.net on
    https://github.com/bethanysciences/SunEvent
*/

#include "Arduino.h"
#include <math.h>
#include <SunEvent.h>
#include <RTClib.h>

SunEvent::SunEvent(float latitude, float longitude, float timezone) {
  _latitude  = latitude;
  _longitude = longitude;
  _timezone  = timezone;
}

DateTime SunEvent::sunset(DateTime call) {
    int secPastMidnight = sunriseSet(false, call.year(), call.month(), call.day(), true) * 60;
    DateTime event = DateTime(call.year(), call.month(), call.day(), 0, 0, 0);
    return DateTime(event + secPastMidnight);
}

DateTime SunEvent::sunrise(DateTime call) {
    int secPastMidnight = sunriseSet(true, call.year(), call.month(), call.day(), true) * 60;
    DateTime event = DateTime(call.year(), call.month(), call.day(), 0, 0, 0);
    return DateTime(event + secPastMidnight);
}

/******************************************************************************/
/*                                  PRIVATE                                   */
/******************************************************************************/
int SunEvent::sunriseSet(bool isRise, int y, int m, int d, bool isDST) {
    float jday, newJday, timeUTC, newTimeUTC;
    int timeLocal;
    jday    = jDay(y, m, d);
    timeUTC = sunriseSetUTC(isRise, jday, _latitude, _longitude);
    newJday    = jday + timeUTC / (60 * 24);
    newTimeUTC = sunriseSetUTC(isRise, newJday, _latitude, _longitude);
    if (!isnan(newTimeUTC)) {
        timeLocal  = (int) round(newTimeUTC + (_timezone * 60));
        timeLocal += (isDST) ? 60 : 0;
    } 
    else timeLocal = -1;
    return timeLocal;
}
float SunEvent::sunriseSetUTC(bool isRise, float jday, float latitude, float longitude) {
    float t         = fractionOfCentury(jday);
    float eqTime    = equationOfTime(t);
    float solarDec  = sunDeclination(t);
    float hourAngle = hourAngleSunrise(latitude, solarDec);
    hourAngle = isRise ? hourAngle : -hourAngle;
    float delta   = longitude + radToDeg(hourAngle);
    float timeUTC = 720 - (4 * delta) - eqTime; // in minutes
    return timeUTC;
}
/* --------- difference of mean solar time and apparent solar time ---------- */
float SunEvent::equationOfTime(float t) {
    float epsilon = obliquityCorrection(t);
    float l0      = geomMeanLongSun(t);
    float e       = eccentricityEarthOrbit(t);
    float m       = geomMeanAnomalySun(t);
    float y = tan(degToRad(epsilon) / 2);
    y *= y;
    float sin2l0 = sin(2.0 * degToRad(l0));
    float sinm   = sin(degToRad(m));
    float cos2l0 = cos(2.0 * degToRad(l0));
    float sin4l0 = sin(4.0 * degToRad(l0));
    float sin2m  = sin(2.0 * degToRad(m));
    float Etime = y * sin2l0 - 2.0 * e * sinm + 
          4.0 * e * y * sinm * cos2l0 - 0.5 * y * 
          y * sin4l0 - 1.25 * e * e * sin2m;
    return radToDeg(Etime) * 4.0;                               // in minutes
}
/* ---------------------- Obliquity of the ecliptic ------------------------- */
float SunEvent::meanObliquityOfEcliptic(float t) {
    float seconds = 21.448 - t * (46.8150 + t * (0.00059 - t * 0.001813));
    float e0      = 23 + (26 + (seconds / 60)) / 60;
    return e0;                                                  // in degrees
}
float SunEvent::eccentricityEarthOrbit(float t) {
    float e = 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
    return e;                                                   // unitless
}
/* --------------------------- SOLAR DECLINATION ---------------------------- */
float SunEvent::sunDeclination(float t) {
    float e      = obliquityCorrection(t);
    float lambda = sunApparentLong(t);
    float sint  = sin(degToRad(e)) * sin(degToRad(lambda));
    float theta = radToDeg(asin(sint));
    return theta;                                               // in degrees
}
float SunEvent::sunApparentLong(float t) {
    float o      = sunTrueLong(t);
    float omega  = 125.04 - 1934.136 * t;
    float lambda = o - 0.00569 - 0.00478 * sin(degToRad(omega));
    return lambda;                                              // in degrees
}
float SunEvent::sunTrueLong(float t) {
    float l0 = geomMeanLongSun(t);
    float c  = sunEqOfCenter(t);
    float O  = l0 + c;
    return O;                                                   // in degrees
}
float SunEvent::sunEqOfCenter(float t) {
    float m     = geomMeanAnomalySun(t);
    float mrad  = degToRad(m);
    float sinm  = sin(mrad);
    float sin2m = sin(mrad * 2);
    float sin3m = sin(mrad * 3);
    float C = sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) +
              sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
    return C;                                                   // in degrees
}
/* ------------------------------- HOUR ANGLE ------------------------------- */
float SunEvent::hourAngleSunrise(float lat, float solarDec) {
    float latRad = degToRad(lat);
    float sdRad  = degToRad(solarDec);
    float HAarg  = (cos(degToRad(90.833)) / (cos(latRad) * cos(sdRad)) - tan(latRad) * tan(sdRad));
    float HA     = acos(HAarg);
    return HA;                                // in radians (for sunset, use -HA)
}
/* ---------------------------- SHARED FUNCTIONS ---------------------------- */
float SunEvent::obliquityCorrection(float t) {
    float e0    = meanObliquityOfEcliptic(t);
    float omega = 125.04 - 1934.136 * t;
    float e     = e0 + 0.00256 * cos(degToRad(omega));
    return e;                                                   // in degrees
}
float SunEvent::geomMeanLongSun(float t) {
    float L0 = 280.46646 + t * (36000.76983 + t * 0.0003032);
    while (L0 > 360) L0 -= 360;
    while (L0 < 0) L0 += 360;
    return L0;                                                  // in degrees
}
float SunEvent::geomMeanAnomalySun(float t) {
    float M = 357.52911 + t * (35999.05029 - 0.0001537 * t);
    return M;                                                   // in degrees
}
/* ------------- Convert Gregorian calendar date to Julian Day -------------- */
float SunEvent::jDay(int year, int month, int day) {
    if (month <= 2) {
        year  -= 1;
        month += 12;
    }
    int A = floor(year/100);
    int B = 2 - A + floor(A/4);
    return floor(365.25 * (year + 4716)) + 
           floor(30.6001 * (month + 1)) + day + B - 1524.5;
}
/* ------ Return fraction of time elapsed this century, AD 2000–2100 -------- */
float SunEvent::fractionOfCentury(float jd) { return (jd - 2451545) / 36525; }
float SunEvent::radToDeg(float rad) { return 180 * rad / PI; }
float SunEvent::degToRad(float deg) { return PI * deg / 180; }
/* ------------------------- Zero-pad time component ------------------------ */
bool SunEvent::zeroPadTime(char *str, byte timeComponent) {
    if (timeComponent >= 100) { return false; }
    str[0] = (floor(timeComponent / 10)) + '0';
    str[1] = (timeComponent % 10) + '0';
    return true;
}
