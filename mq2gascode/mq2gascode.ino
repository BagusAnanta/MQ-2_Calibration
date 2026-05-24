#define MQ2pin 0 // A0 pin

// scope detecting concentration gas LPG (this value from MQ-2 datasheet reference)
#define minTreshold  200
#define maxTreshold 5000

#define airreference 9.8 // air reference based datasheet

// define regression linear calibrate (based calculation from datasheet for LPG gas graph)
// but this slope and intercept at log10 result

#define slope -0.471624821296122 // m
#define intercept 1.30581824559981 // c

long adcBitReference = 1023; // ADC bit reference, arduino 10 bit = 1023 ADC, ESP32 12 bit = 2045 ADC sampling

long sensorValue = 0;
float Vcc = 4.25; // board voltage (default 5.0VDC) but in here I set 4.25 based measure Voltage 
float Vout = 0.0;
float RL = 10000.0; // (10k ohm based Fix resistor at board) read PCB wired for makesure resistor value used manufacture
float Rs = 0.0; 
float R0 = 1675613.797; // this R0 from experiment at clean air (clean air room situation) 
float RsperR0Ratio = 0.0;
float ppm = 0.0;

// now using this, 
/* long readVcc(){
  long result;

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));

  result = ADCL;
  result |= ADCH << 8;

  result = 1125300L / result;
  Vcc = result / 1000.0;
} */

float vout(long readAnalog){
  return readAnalog * (Vcc / adcBitReference);
}

float getR0(long readAnalog){
  Vout = vout(readAnalog);  // Vout : 1023 is 10 // Vout : 1023 is 10 bit ADC reference for Arduino, make sure check your ADC bit reference before
  Rs = (Vcc - Vout) / Vout * RL;

  R0 = Rs / airreference;

  return R0;
}

float getPPM(long readAnalog, float R0){
  Vout = vout(readAnalog); // Vout : 1023 is 10 bit ADC reference for Arduino, make sure check your ADC bit reference before
  Rs = (Vcc - Vout) / Vout * RL;

  RsperR0Ratio = Rs/R0;

  // regression 
  ppm = pow(10, ((slope) * log10(RsperR0Ratio) + intercept));

  return ppm;
}


void setup() {
  // put your setup code here, to run once:

  // warming up sensor
  Serial.begin(9600);
  Serial.println("MQ2 warming up");
  delay(20000);

  Serial.println("Time,SensorValue");

}

void loop() {

  sensorValue = analogRead(MQ2pin);

  // Run this for get R0
  // float R0data = getR0(sensorValue);
  
  float data = getPPM(sensorValue, R0);

//  Serial.print("Rs: ");
//  Serial.println(Rs);
//
//  Serial.print("R0: ");
//  Serial.println(R0);
//
//  Serial.print("RsperR0Ratio: ");
//  Serial.println(RsperR0Ratio);

//  Serial.print("Log: ");
//  Serial.println(log10(RsperR0Ratio));

  Serial.print(millis());
  Serial.print(",");
  Serial.println(data);

  if(data <= minTreshold){
    Serial.println("No gas Detected");  
  } else if(data >= minTreshold && data <= maxTreshold){
    Serial.println("Gas Detected, warning");
  }


  delay(2000);

}
