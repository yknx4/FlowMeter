#include "FlowMeter_main.h"
#include "xxhash.h"
 
// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com
/*****************/
/* VARIABLES */
/****************/
int segTrans=0;
int hash;
char buffer [33];
const int seconds = 60;
const double qValue = 5.72;
#define NO_SENSORS 3
const int noSensors  = NO_SENSORS;
 const int hallsensor[] ={1,1,1};    //The pin location of the sensor
const int leds[] = {13,13,13};    //The pin location of the sensor
int literPerMin[NO_SENSORS];
int totalCalculated[NO_SENSORS];

 
 /*************
 SENSOR 1
*************/
volatile int sensor1_vCount; //measuring the rising edges of the signal
int sensor1_tCount=0;
  /*************
 SENSOR 2
*************/
volatile int sensor2_vCount; //measuring the rising edges of the signal
int sensor2_tCount=0;
 /*************
 SENSOR 3
*************/
volatile int sensor3_vCount; //measuring the rising edges of the signal
int sensor3_tCount=0;
 
 
double Calc,Calc2;
/*const int hallsensor = 1;    //The pin location of the sensor
const int led = 13;    //The pin location of the sensor
 */

// The setup() method runs once, when the sketch starts



void setup() //
{
	randomSeed(analogRead(0));
	//pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
	//pinMode(led, OUTPUT); //initializes digital pin 2 as an input
	setupHallSensors();
	Serial.begin(9600); //This is the setup function where the serial port is
	attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}
void resetCounters(){
	sensor1_vCount = 0;      //Set sensor 1 to 0 ready for calculations
	sensor2_vCount = 0;      //Set sensor 2  to 0 ready for calculations
	sensor3_vCount = 0;      //Set sensor 3 to 0 ready for calculations
}
void readSensorsData(){
	sei();            //Enables interrupts
	delay (1000);      //Wait 1 second
	cli();            //Disable interrupts
	segTrans++;
}

// the loop() method runs over and over again,
// as long as the Arduino has power
void loop ()
{
/*Data Reading*/
	 resetCounters();
	 
	readSensorsData();
	
	resetLeds();
	
/*Data Processing*/
	Serial.print (sensor1_vCount, DEC); //Prints the number calculated above
	Serial.print (" RAW/S\r\n"); //Prints "L/hour" and returns a  new line
	
	Calc = (sensor1_vCount * seconds /qValue); //(Pulse frequency x 60s) / 7.5Q, = flow rate in L/min
	sensor1_tCount+=sensor1_vCount;
	Serial.print (Calc); //Prints the number calculated above
	Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
	Serial.print (Calc*0.2777); //Prints the number calculated above
	Serial.print (" mL/s\r\n");
	Calc2 = ((sensor1_tCount/qValue))/seconds;
	Serial.print (Calc2); //Prints the number calculated above
	Serial.print (" L in sensor1_tCount\r\n"); //Prints "L" and returns a  new line
	Serial.print (sensor1_tCount); //Prints the number calculated above
	Serial.print (" RAW sensor1_tCount\r\n"); //Prints "L/hour" and returns a  new line
	itoa (sensor1_vCount,buffer,10);
	 
	hash = XXH32 ((void*)buffer, lenHelper(sensor1_vCount), 5027);
	 
	Serial.print(sensor1_tCount,DEC);
	Serial.print (hash, HEX); //Prints the number calculated above
	Serial.print (" -> HASH\r\n"); //Prints "L/hour" and returns a  new line
}
 
inline double calculateLitersPerMinute(int count){
	 return (count * seconds) /qValue;
}
 
int lenHelper(unsigned x) {
	// this is either a fun exercise in optimization
	// or it's extremely premature optimization.
	if(x>=100000) {
		if(x>=10000000) {
			if(x>=1000000000) return 10;
			if(x>=100000000) return 9;
			return 8;
		}
		if(x>=1000000) return 7;
		return 6;
	} else {
		if(x>=1000) {
			if(x>=10000) return 5;
			return 4;
		} else {
			if(x>=100) return 3;
			if(x>=10) return 2;
			return 1;
		}
	}
}
 
void rpm ()     //This is the function that the interupt calls
{
	sensor1_vCount++;  //This function measures the rising and falling edge of the
	digitalWrite(leds[0], HIGH);   // turn the LED on (HIGH is the voltage level)
	
	/*Temporal*/
	rpm1();
	rpm2();
	/**/
}
void rpm1 ()     //This is the function that the interupt calls
{
	sensor2_vCount++;  //This function measures the rising and falling edge of the
	digitalWrite(leds[1], HIGH);   // turn the LED on (HIGH is the voltage level)
}
void rpm2 ()     //This is the function that the interupt calls
{
	sensor3_vCount++;  //This function measures the rising and falling edge of the
	digitalWrite(leds[2], HIGH);   // turn the LED on (HIGH is the voltage level)
}

void resetLeds(){
	for (int i =0;i<noSensors;i++){
			digitalWrite(leds[i], LOW);    // turn the LED off by making the voltage LOW
	}
}

void setupHallSensors(){
	for (int i =0;i<noSensors;i++){
			pinMode(hallsensor[i], INPUT); //initializes digital pin i as an input
			pinMode(leds[i], OUTPUT); //initializes led to notify
	}
}