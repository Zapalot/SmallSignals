#include <Wire.h>
#include "Adafruit_ADS1X15.h"

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

int analogInPin = A3;
unsigned long lastMicros = 0;

// for 50Hz-Notch-Filtering (https://github.com/tttapa/Arduino-Filters)
#include <Filters.h>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/Notch.hpp>
#include <Filters/Butterworth.hpp>
////////// Sampling frequency
const double f_s = 404; // Hz // this needs to be adjusted to the actual sampling frequency
// Notch frequency (-∞ dB)
const double f_c = 50; // Hz //
// Normalized notch frequency
const double f_n = 2 * f_c / f_s;
// Create Finite Impulse Response notch filters for 50 and 100 Hz
auto filter1 = simpleNotchFIR(f_n);     // fundamental
auto filter2 = simpleNotchFIR(2 * f_n); // second harmonic
auto filterLowPass = butter<6>(f_n);
void setup()
{

  Serial.begin(115200);


  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  /// Felix: this is not what is written in the datasheet, they just tell you to say within +-0.3V of the supply voltages.
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV


  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
  Wire.setClock(400000);
  ads.setGain(GAIN_FOUR);
    ads.setDataRate(RATE_ADS1115_860SPS);
  ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, false); // start single conversion of Differential Voltages
}

void loop()
{
  unsigned long newMicros = micros();
  unsigned long diffMicros = newMicros - lastMicros;
  lastMicros = newMicros;
  // start a non-blocking conversion while
  while (!ads.conversionComplete()); // wait for last conversion to finish
  int16_t diffresult = ads.getLastConversionResults();
  ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, false); // start single conversion of Differential Voltages
                                                               // now, while the next conversion is happening, we have time to process the results.
  float filteredResult = filterLowPass(filter1(diffresult));
  // Serial.print(analogRead(analogInPin));
  // Serial.print("\t");

  Serial.print(diffresult);
  Serial.print("\t");
  Serial.print(filteredResult);
  Serial.print("\t");
  Serial.print(1e6f/(float)diffMicros);
  Serial.print("\n");
}
