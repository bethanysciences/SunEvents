# Almanac for specific Latitude and Longitudes
  
Operates independantly not needing an Internet connection  
  
[Bob Smith](https://github.com/bethanysciences/almanac)  
  
Time grabbed from [Adafruit's](https://www.adafruit.com/product/3013) I2C Maxim DS3231 basecReal Time Clock (RTC) module using Adafruit's [RTClib](https://github.com/adafruit/RTClib) library, a fork of JeeLab's [RTClib](https://git.jeelabs.org/jcw/rtclib) library.  
  
Tide events calculated using a derivative of tinytideclock.ino and based on Luke Miller's library of NOAA harmonic data [Tide_calculator](https://github.com/millerlp/Tide_calculator) derived from David Flater's XTide application [xtide](https://flaterco.com/xtide/xtide.html).
  
Sun events calculated using SunEvents library outputting sunrise and sets to a RTClib DateTime object, a aerivative of [DM Kishi's library](https://github.com/dmkishi/Dusk2Dawn), based on a port of [NOAA's Solar  Calculator](https://www.esrl.noaa.gov/gmd/grad/solcalc/)  
  
This application and project are open source using MIT License see license.txt.  
  
See included library github directories for their respecive licenses
