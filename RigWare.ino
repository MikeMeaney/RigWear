//reWrite.ino
//Pin for Hand in LED
const int handInLed = 11; 
//Pin for submitButton 
const int submitButton = 5; 
//Pin for Laser 
const int laser1 = 9;
// Pin for PhotoCell
const int photoCell = A0;

// Place to Store Calibration
int sensorValue = 0;
int sensorMin = 1023;
int sensorMax = 0;
float fudgeFactor = 0.70;

int ambCal;
int laserCal;
int baseCal; 

unsigned long start, finished, elapsed; 

void setup () {
	Serial.begin(115200);
	Serial.print('[');
	//Beginning of Program 
		Serial.println(F("Starting Program"));
		delay (1000);
		Serial.println(F("Testing Circuit"));
		
		//Test HandinLed
		pinMode (handInLed,OUTPUT);
		digitalWrite (handInLed,HIGH);
		delay (1000);
		digitalWrite (handInLed,LOW);
		
		//Test Laser
		Serial.println(F("Testing Laser"));
		pinMode(laser1, OUTPUT);
		
		LEDfadeIn(laser1, 2);
		LEDfadeOut(laser1, 2);

		//digitalWrite(laser1,HIGH);
		//delay (1000);
		//digitalWrite(laser1, LOW);
		//delay (1000);
		//Completion of Test 
		Serial.println(F("Testing Completed"));
		delay(1000);
		
		//Calibration for Ambient Light 
		Serial.println(F("Beginning Base Line Ambient Calbration"));
		digitalWrite (laser1,LOW); // Turn off laser
		delay(250);
		baseCal = analogRead(A0); //Save value
		Serial.println (baseCal); 
		Serial.println(F("Calibration for Ambient Light Completed"));

		//Calibration for Laser 
		Serial.println(F("Calibration for Lasers Beginning"));
		digitalWrite (laser1,HIGH); //turn laser on
		delay(250);
		laserCal = analogRead(A0);
		Serial.println ("Calibration for Lasers Completed");
		delay(1000);
		Serial.println(laserCal);
		delay(1000);
		Serial.println(F("Calibration and Testing Completed"));
		Serial.println(F("Starting Test"));
		digitalWrite(laser1, HIGH);
		delay (1000);
		Serial.println(F("Awaiting for Insertion of Hand"));
			Serial.print(']');

}
	void loop(){
		//Debug
	    // Serial.print(F("Awaiting Hand Insertion: "));
	    // Serial.print(laserCal);
	    // Serial.print(" | ");
	    // Serial.println(analogRead(A0));

		//This is the meat and potatoes of the whole deal
		//Once the analog value dips below the calibrated value
		if (analogRead(A0) <= laserCal*fudgeFactor) {
				//Serial.print(F("---- There be a hand in me! ---- :  "));
				Serial.print('\0');
				start = millis(); 
				Serial.println(start);

				//The magic is that while it's at or below base cal
				//the program can only count the amount of time up
				while(analogRead(A0) <= laserCal*fudgeFactor){
					// Serial.print(F("Elapsed: "));
					digitalWrite(handInLed, HIGH);
					elapsed = timeDiff(start);
					// Serial.println(elapsed);
				}

			//The only way we can get here is if the while statement
			//gets broken out of. 	
			// Serial.print(F("Time Elapsed ")) ;
			// Serial.println(elapsed);
			// '\t' tells the client that the hand is 
			// removed and next series of data is he elapsed time as a string. 
			Serial.print('\t');  
			Serial.print(elapsed);
			//delay(250); // prevent dataCorruption
			LEDfadeOut(handInLed, 1);
			//Insert command to send data to computer,
			
			//Serial.println(F("Hand Removed at"));

			} 
			else {
			//The only way that we can arrive here is when
			//fuck-all-nothing (FAN) is occuring.
			digitalWrite(handInLed, LOW);

			}
							
	}						
//Time Calculations 

long timeDiff (long s) {
	
	//Place to hold value to return (the difference the two value)
	long diff = 0;
	
	//Get the current millis at time of calling time difference
	long currentMillis = millis();
	
	//Calculate differencce between start and now
	diff = currentMillis - s;

	return diff; 
}


void LEDfadeOut(int LEDpin, int d){
	for(int i = 255; i > 0; i--){
		analogWrite(LEDpin, i);
		delay(d);
	}
}

void LEDfadeIn(int LEDpin, int d){
	for(int i = 0; i < 255; i++){
		analogWrite(LEDpin, i);
		delay(d);
	}
}
