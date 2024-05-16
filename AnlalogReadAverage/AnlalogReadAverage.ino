/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int nReads=1;
  long sumOfReads=0; //use a datatype for the sum that can store a large number
  for(int i=0;i<nReads;i++) {   //repeat what is in the curly brackets nReads times
    int sensorValue = analogRead(A0);
    sumOfReads=sumOfReads+sensorValue;
  }
  // print out the value you read:
  Serial.println(sumOfReads/nReads);
  delay(1);  // delay in between reads for stability
}
