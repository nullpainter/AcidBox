// #include "BeatDetector.h"

// /**
//  * Analog to Digital Conversion needs to be configured to free running mode
//  * in order to read the sound sensor values at a high frequency.
//  *
//  * See: http://maxembedded.com/2011/06/the-adc-of-the-avr/
//  */
// void setupADC() {
//   ADCSRA = 0xe0 + 7;  // "ADC Enable", "ADC Start Conversion", "ADC Auto Trigger Enable" and divider.
//   ADMUX = 0x0;        // use adc0. Use ARef pin for analog reference (same as analogReference(EXTERNAL)).
//   ADMUX |= 0x40;      // Use Vcc for analog reference.
//   DIDR0 = 0x01;       // turn off the digital input for adc0
// }

// void loopBeatDetector() {
//   if (LOG_FREQUENCY_DATA) {
//     readAudioSamples();
//     getFrequencyData();
//     logFrequencyData();
//   } else {
//     //Serial.print(String(millis()));
//     readAudioSamples();
//     if (PERFORM_BEAT_DETECTION) {
//       getFrequencyData();
//       processFrequencyData();
//       updateBeatProbability();
//       updateLightIntensityBasedOnBeats();
//     } else {
//       updateLightIntensityBasedOnAmplitudes();
//     }

//     updateLights();
//   }
//   // Serial.println("");
// }

// /**
//  * Will read the sound sensor values from pin A0.
//  */
// void readAudioSamples() {
//   long currentAverage = 0;
//   long currentMaximum = 0;
//   long currentMinimum = MAXIMUM_SIGNAL_VALUE;

//   for (int i = 0; i < FHT_N; i++) {  // save 256 samples
//     while (!(ADCSRA & /*0x10*/ _BV(ADIF)))
//       ;                 // wait for adc to be ready (ADIF)
//     sbi(ADCSRA, ADIF);  // restart adc
//     byte m = ADCL;      // fetch adc data
//     byte j = ADCH;
//     int k = ((int)j << 8) | m;  // form into an int

//     currentMinimum = min(currentMinimum, k);
//     currentMaximum = max(currentMaximum, k);
//     currentAverage += k;

//     k -= 0x0200;  // form into a signed int
//     k <<= 6;      // form into a 16b signed int
//     k <<= FreqGainFactorBits;

//     fht_input[i] = k;  // put real data into bins
//   }

//   currentAverage /= FHT_N;

//   int signalDelta = currentMaximum - currentAverage;
//   currentSignal = currentAverage + (2 * signalDelta);

//   constrain(currentSignal, 0, currentMaximum);

//   processHistoryValues(
//     signals,
//     frequencyMagnitudeSampleIndex,
//     currentSignal,
//     totalSignal,
//     averageSignal,
//     signalVariance);

//   //logValue("A", (float) currentAverage / MAXIMUM_SIGNAL_VALUE, 10);
//   //logValue("M", (float) currentMaximum / MAXIMUM_SIGNAL_VALUE, 10);
//   logValue("S", (float)currentSignal / MAXIMUM_SIGNAL_VALUE, 20);
// }

// /**
//  * Will run the Fast Hartley Transform to convert the time domain signals
//  * to the frequency domain.
//  *
//  * See: http://wiki.openmusiclabs.com/wiki/ArduinoFHT
//  */
// void getFrequencyData() {
//   fht_window();   // window the data for better frequency response
//   fht_reorder();  // reorder the data before doing the FHT
//   fht_run();      // process the data in the FHT
//   fht_mag_log();  // get the magnitude of each bin in the FHT
// }

// void logFrequencyData() {
// #ifdef FreqSerialBinary
//   // print as binary
//   Serial.write(255);                     // send a start byte
//   Serial.write(fht_log_out, FHT_N / 2);  // send out the data
// #else
//   // print as text
//   for (int i = 0; i < FHT_N / 2; i++) {
//     Serial.print(fht_log_out[i]);
//     Serial.print(',');
//   }
// #endif
// }

// /**
//  * Will extract insightful features from the frequency data in order
//  * to perform the beat detection.
//  */
// void processFrequencyData() {
//   // each of the methods below will:
//   //  - get the current frequency magnitude
//   //  - add the current magnitude to the history
//   //  - update relevant features
//   processOverallFrequencyMagnitude();
//   processFirstFrequencyMagnitude();
//   processSecondFrequencyMagnitude();

//   // prepare the magnitude sample index for the next update
//   frequencyMagnitudeSampleIndex += 1;
//   if (frequencyMagnitudeSampleIndex >= FREQUENCY_MAGNITUDE_SAMPLES) {
//     frequencyMagnitudeSampleIndex = 0;  // wrap the index
//   }
// }

// void processOverallFrequencyMagnitude() {
//   currentOverallFrequencyMagnitude = getFrequencyMagnitude(
//     fht_log_out,
//     OVERALL_FREQUENCY_RANGE_START,
//     OVERALL_FREQUENCY_RANGE_END);

//   processHistoryValues(
//     overallFrequencyMagnitudes,
//     frequencyMagnitudeSampleIndex,
//     currentOverallFrequencyMagnitude,
//     totalOverallFrequencyMagnitude,
//     averageOverallFrequencyMagnitude,
//     overallFrequencyMagnitudeVariance);
// }

// void processFirstFrequencyMagnitude() {
//   currentFirstFrequencyMagnitude = getFrequencyMagnitude(
//     fht_log_out,
//     FIRST_FREQUENCY_RANGE_START,
//     FIRST_FREQUENCY_RANGE_END);

//   processHistoryValues(
//     firstFrequencyMagnitudes,
//     frequencyMagnitudeSampleIndex,
//     currentFirstFrequencyMagnitude,
//     totalFirstFrequencyMagnitude,
//     averageFirstFrequencyMagnitude,
//     firstFrequencyMagnitudeVariance);
// }

// void processSecondFrequencyMagnitude() {
//   currentSecondFrequencyMagnitude = getFrequencyMagnitude(
//     fht_log_out,
//     SECOND_FREQUENCY_RANGE_START,
//     SECOND_FREQUENCY_RANGE_END);

//   processHistoryValues(
//     secondFrequencyMagnitudes,
//     frequencyMagnitudeSampleIndex,
//     currentSecondFrequencyMagnitude,
//     totalSecondFrequencyMagnitude,
//     averageSecondFrequencyMagnitude,
//     secondFrequencyMagnitudeVariance);
// }

// byte getFrequencyMagnitude(byte frequencies[], const int startIndex, const int endIndex) {
//   int total = 0;
//   int average = 0;
//   int maximum = 0;
//   int minimum = MAXIMUM_SIGNAL_VALUE;
//   int current;

//   for (int i = startIndex; i < endIndex; i++) {
//     current = frequencies[i];
//     total += current;
//     maximum = max(maximum, current);
//     minimum = min(minimum, current);
//   }

//   average = total / (endIndex - startIndex);

//   int value = average;
//   //int value = maximum - average;

//   //logValue("F", (float) value / 128, 10);

//   return value;
// }

// void processHistoryValues(byte history[], int &historyIndex, int &current, int &total, int &average, int &variance) {
//   total -= history[historyIndex];   // subtract the oldest history value from the total
//   total += (byte)current;           // add the current value to the total
//   history[historyIndex] = current;  // add the current value to the history

//   average = total / FREQUENCY_MAGNITUDE_SAMPLES;

//   // update the variance of frequency magnitudes
//   long squaredDifferenceSum = 0;
//   for (int i = 0; i < FREQUENCY_MAGNITUDE_SAMPLES; i++) {
//     squaredDifferenceSum += pow(history[i] - average, 2);
//   }
//   variance = (double)squaredDifferenceSum / FREQUENCY_MAGNITUDE_SAMPLES;
// }

// /**
//  * Will update the beat probability, a value between 0 and 1
//  * indicating how likely it is that there's a beat right now.
//  */
// void updateBeatProbability() {
//   beatProbability = 1;
//   beatProbability *= calculateSignalChangeFactor();
//   beatProbability *= calculateMagnitudeChangeFactor();
//   beatProbability *= calculateVarianceFactor();
//   beatProbability *= calculateRecencyFactor();

//   if (beatProbability >= beatProbabilityThreshold) {
//     lastBeatTimestamp = millis();
//     durationSinceLastBeat = 0;
//   }

//   logValue("B", beatProbability, 5);
// }

// /**
//  * Will calculate a value in range [0:2] based on the magnitude changes of
//  * different frequency bands.
//  * Low values are indicating a low beat probability.
//  */
// float calculateSignalChangeFactor() {
//   float aboveAverageSignalFactor;
//   if (averageSignal < 75 || currentSignal < 150) {
//     aboveAverageSignalFactor = 0;
//   } else {
//     aboveAverageSignalFactor = ((float)currentSignal / averageSignal);
//     aboveAverageSignalFactor = constrain(aboveAverageSignalFactor, 0, 2);
//   }

//   logValue("SF", aboveAverageSignalFactor / 2, 2);
//   return aboveAverageSignalFactor;
// }

// /**
//  * Will calculate a value in range [0:1] based on the magnitude changes of
//  * different frequency bands.
//  * Low values are indicating a low beat probability.
//  */
// float calculateMagnitudeChangeFactor() {
//   float changeThresholdFactor = 1.1;
//   if (durationSinceLastBeat < 750) {
//     // attempt to not miss consecutive beats
//     changeThresholdFactor *= 0.95;
//   } else if (durationSinceLastBeat > 1000) {
//     // reduce false-positives
//     changeThresholdFactor *= 1.05;
//   }

//   // current overall magnitude is higher than the average, probably
//   // because the signal is mainly noise
//   float aboveAverageOverallMagnitudeFactor = ((float)currentOverallFrequencyMagnitude / averageOverallFrequencyMagnitude);
//   aboveAverageOverallMagnitudeFactor -= 1.05;
//   aboveAverageOverallMagnitudeFactor *= 10;
//   aboveAverageOverallMagnitudeFactor = constrain(aboveAverageOverallMagnitudeFactor, 0, 1);

//   // current magnitude is higher than the average, probably
//   // because the there's a beat right now
//   float aboveAverageFirstMagnitudeFactor = ((float)currentFirstFrequencyMagnitude / averageFirstFrequencyMagnitude);
//   aboveAverageOverallMagnitudeFactor -= 0.1;
//   aboveAverageFirstMagnitudeFactor *= 1.5;
//   aboveAverageFirstMagnitudeFactor = pow(aboveAverageFirstMagnitudeFactor, 3);
//   aboveAverageFirstMagnitudeFactor /= 3;
//   aboveAverageFirstMagnitudeFactor -= 1.25;

//   aboveAverageFirstMagnitudeFactor = constrain(aboveAverageFirstMagnitudeFactor, 0, 1);

//   float aboveAverageSecondMagnitudeFactor = ((float)currentSecondFrequencyMagnitude / averageSecondFrequencyMagnitude);
//   aboveAverageSecondMagnitudeFactor -= 1.01;
//   aboveAverageSecondMagnitudeFactor *= 10;
//   aboveAverageSecondMagnitudeFactor = constrain(aboveAverageSecondMagnitudeFactor, 0, 1);

//   float magnitudeChangeFactor = aboveAverageFirstMagnitudeFactor;
//   if (magnitudeChangeFactor > 0.15) {
//     magnitudeChangeFactor = max(aboveAverageFirstMagnitudeFactor, aboveAverageSecondMagnitudeFactor);
//   }

//   if (magnitudeChangeFactor < 0.5 && aboveAverageOverallMagnitudeFactor > 0.5) {
//     // there's no bass related beat, but the overall magnitude changed significantly
//     magnitudeChangeFactor = max(magnitudeChangeFactor, aboveAverageOverallMagnitudeFactor);
//   } else {
//     // this is here to avoid treating signal noise as beats
//     //magnitudeChangeFactor *= 1 - aboveAverageOverallMagnitudeFactor;
//   }

//   logValue("AO", aboveAverageOverallMagnitudeFactor, 2);
//   logValue("A1", aboveAverageFirstMagnitudeFactor, 10);
//   logValue("A2", aboveAverageSecondMagnitudeFactor, 10);

//   logValue("M", magnitudeChangeFactor, 1);

//   return magnitudeChangeFactor;
// }

// /**
//  * Will calculate a value in range [0:1] based on variance in the first and second
//  * frequency band over time. The variance will be high if the magnitude of bass
//  * frequencies changed in the last few milliseconds.
//  * Low values are indicating a low beat probability.
//  */
// float calculateVarianceFactor() {
//   // a beat also requires a high variance in recent frequency magnitudes
//   float firstVarianceFactor = ((float)(firstFrequencyMagnitudeVariance - 50) / 20) - 1;
//   firstVarianceFactor = constrain(firstVarianceFactor, 0, 1);

//   float secondVarianceFactor = ((float)(secondFrequencyMagnitudeVariance - 50) / 20) - 1;
//   secondVarianceFactor = constrain(secondVarianceFactor, 0, 1);

//   float varianceFactor = max(firstVarianceFactor, secondVarianceFactor);

//   logValue("V", varianceFactor, 1);

//   return varianceFactor;
// }

// /**
//  * Will calculate a value in range [0:1] based on the recency of the last detected beat.
//  * Low values are indicating a low beat probability.
//  */
// float calculateRecencyFactor() {
//   float recencyFactor = 1;
//   durationSinceLastBeat = millis() - lastBeatTimestamp;

//   int referenceDuration = MINIMUM_DELAY_BETWEEN_BEATS - SINGLE_BEAT_DURATION;
//   recencyFactor = 1 - ((float)referenceDuration / durationSinceLastBeat);
//   recencyFactor = constrain(recencyFactor, 0, 1);

//   //logValue("R", recencyFactor, 5);

//   return recencyFactor;
// }

// /**
//  * Will update the light intensity bump based on the recency of detected beats.
//  */
// void updateLightIntensityBasedOnBeats() {
//   float intensity = 1 - ((float)durationSinceLastBeat / LIGHT_FADE_OUT_DURATION);
//   intensity = constrain(intensity, 0, 1);

//   if (intensity > lightIntensityValue) {
//     lightIntensityBumpValue = intensity;
//     lightIntensityBumpTimestamp = millis();
//   }
// }

// /**
//  * Will update the light intensity bump based on measured amplitudes.
//  */
// void updateLightIntensityBasedOnAmplitudes() {
//   float intensity;
//   if (averageSignal < 1 || currentSignal < 1) {
//     intensity = 0;
//   } else {
//     intensity = (float)(currentSignal - averageSignal) / MAXIMUM_SIGNAL_VALUE;
//     intensity *= pow(intensity, 3);

//     if (intensity < 0.1) {
//       intensity = 0;
//     } else {
//       intensity -= 0.1;
//       intensity = pow(1 + intensity, 3) - 1;
//       intensity = constrain(intensity, 0, 1);
//     }
//   }

//   logValue("I", intensity, 10);

//   if (intensity > lightIntensityValue) {
//     lightIntensityBumpValue = intensity;
//     lightIntensityBumpTimestamp = millis();
//   }
// }

// /**
//  * Will update the hat lights based on the last light intensity bumps.
//  */
// void updateLights() {
//   long durationSinceLastBump = millis() - lightIntensityBumpTimestamp;
//   float fadeFactor = 1 - ((float)durationSinceLastBump / LIGHT_FADE_OUT_DURATION);
//   fadeFactor = constrain(fadeFactor, 0, 1);

//   lightIntensityValue = lightIntensityBumpValue * fadeFactor;
//   lightIntensityValue = constrain(lightIntensityValue, 0, 1);

//   logValue("L", lightIntensityValue, 20);

//   // scale the intensity to be in range of maximum and minimum
//   float scaledLightIntensity = MINIMUM_LIGHT_INTENSITY + (lightIntensityValue * (MAXIMUM_LIGHT_INTENSITY - MINIMUM_LIGHT_INTENSITY));

//   int pinValue = 255 * scaledLightIntensity;
//   //analogWrite(HAT_LIGHTS_PIN, pinValue);

//   // also use the builtin LED, for debugging when no lights are connected
//   if (scaledLightIntensity > MAXIMUM_LIGHT_INTENSITY - ((MAXIMUM_LIGHT_INTENSITY - MINIMUM_LIGHT_INTENSITY) / 4)) {
//     digitalWrite(LED_BUILTIN, HIGH);
//     //  encoderPos=50;
//     FastLED.setBrightness(128);
//   } else {
//     digitalWrite(LED_BUILTIN, LOW);
//     FastLED.setBrightness(0);
//     //encoderPos=0;
//   }

//   // update the pulse signal
//   long durationSincePulse = millis() - lastPulseTimestamp;
//   fadeFactor = ((float)durationSincePulse / (LIGHT_PULSE_DURATION * 2));
//   if (durationSincePulse >= LIGHT_PULSE_DURATION) {
//     fadeFactor = 1 - fadeFactor;
//   }
//   fadeFactor *= 2;
//   fadeFactor = constrain(fadeFactor, 0, 1);

//   // scale the intensity to be in range of maximum and minimum
//   scaledLightIntensity = MINIMUM_LIGHT_INTENSITY + (fadeFactor * (MAXIMUM_LIGHT_INTENSITY - MINIMUM_LIGHT_INTENSITY));

//   //logValue("P", scaledLightIntensity, 10);

//   pinValue = 255 * scaledLightIntensity;


//   // FastLED.setBrightness(pinValue);
//   //analogWrite(HAT_LIGHTS_PULSE_PIN, pinValue);


//   // TEMP
//   // encoderPos = pinValue;

//   if (durationSincePulse >= LIGHT_PULSE_DELAY) {
//     lastPulseTimestamp = millis();
//   }
// }

// /**
//  * Converts the specified value into an ASCII-art progressbar
//  * with the specified length.
//  */
// String toProgressBar(float value, const int length) {
//   int amount = max(0, min(length, value * length));
//   String progressBar = "[";
//   for (int i = 0; i < amount; i++) {
//     progressBar += "=";
//   }
//   for (int i = 0; i < length - amount; i++) {
//     progressBar += " ";
//   }
//   progressBar += "]";
//   return progressBar;
// }

// void logValue(String name, boolean value) {
//   logValue(name, value ? 1.0 : 0.0, 1);
// }

// void logValue(String name, float value) {
//   logValue(name, value, 10);
// }

// void logValue(String name, float value, int length) {
//   Serial.print(" | " + name + ": " + toProgressBar(value, length));
// }
