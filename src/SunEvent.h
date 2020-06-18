/*  SunEvent.h
    2020-06-16
    Get sunrise and sunset DateTime object
    Created by Bob Smith bob@bethanysciences.net on
    https://github.com/bethanysciences/SunEvent
*/

#ifndef SunEvent_h
#define SunEvent_h

  #include "Arduino.h"
  #include <math.h>
  #include <RTClib.h>

  class SunEvent {
    public:
        SunEvent(float, float, float);
        DateTime sunrise(const DateTime);
        DateTime sunset(const DateTime);
    private:
      float _latitude, _longitude;
      int   _timezone;
      int   sunriseSet(bool, int, int, int, bool);
      float sunriseSetUTC(bool, float, float, float);
      float equationOfTime(float);
      float meanObliquityOfEcliptic(float);
      float eccentricityEarthOrbit(float);
      float sunDeclination(float);
      float sunApparentLong(float);
      float sunTrueLong(float);
      float sunEqOfCenter(float);
      float hourAngleSunrise(float, float);
      float obliquityCorrection(float);
      float geomMeanLongSun(float);
      float geomMeanAnomalySun(float);
      float jDay(int, int, int);
      float fractionOfCentury(float);
      float radToDeg(float);
      float degToRad(float);
      static bool zeroPadTime(char*, byte);
    protected:
        uint8_t yOff;       ///< Year offset from 2000
        uint8_t m;          ///< Month 1-12
        uint8_t d;          ///< Day 1-31
        uint8_t hh;         ///< Hours 0-23
        uint8_t mm;         ///< Minutes 0-59
        uint8_t ss;         ///< Seconds 0-59
  };

#endif
