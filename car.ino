		 #include <Adafruit_MotorShield.h>
		 #include <Wire.h>   //Протокол I2C
		 #include "utility/Adafruit_MS_PWMServoDriver.h"
		 unsigned long Ch3Value,Ch4Value;
		 unsigned long last3, last4;
		 int pinCh3 = 12;
		 int pinCh4 = 11;
		 Adafruit_MotorShield AFMS = Adafruit_MotorShield();
		 Adafruit_DCMotor *Motor1 = AFMS.getMotor(4); //Передний мост
		 Adafruit_DCMotor *Motor2 = AFMS.getMotor(2); //Поворотный
		 Adafruit_DCMotor *Motor3 = AFMS.getMotor(3); //Задний мост
		 byte SPEED_FORWARD = 0; // текущая скорость моторов
		 byte SPEED_BACKWARD = 0; // текущая скорость моторов
		 byte SPEED_LEFT = 0;
		 byte SPEED_RIGHT = 0;
		 //UltraSonic Sensor
		 #define echoPin 5 // Echo Pin
		 #define trigPin 6 // Trigger Pin
		 long duration, distance; // Duration used to calculate distance
		 int maximumRange = 200; // Maximum range needed
		 int minimumRange = 0; // Minimum range needed
		 int dist1 = 0; //растояния в областях
		 int criticalDistance = 5; //см
		 int stopDistance = 10; //см

		 void setup() {
		    Serial.begin(9600);
		    pinMode(pinCh3, INPUT);
		    pinMode(pinCh4, INPUT);
		    last3 = pulseIn(pinCh3, HIGH);
		    last4 = pulseIn(pinCh4, HIGH);

		    //UltraSonic Sensor
		    pinMode(trigPin, OUTPUT);
		    pinMode(echoPin, INPUT);
		 
                    motorInit();
		 }

		 void loop() {
		    Ch3Value = pulseIn(pinCh3, HIGH);
		    if (Ch3Value == 0){Ch3Value = last3;}
		    else { last3 = Ch3Value;}
		    Ch4Value = pulseIn(pinCh4, HIGH);
		    if (Ch4Value == 0){Ch4Value = last4;}
		    else { last4 = Ch4Value;}
		    SPEED_FORWARD = 0;
		    SPEED_BACKWARD = 0;

                    if (last3 <= 1000) {SPEED_BACKWARD = 0; SPEED_FORWARD = 0;}
                    else {
                       if (last3 <= 1440) { SPEED_BACKWARD = map(last3, 1000, 1440, 255, 0); }
                       else if (last3 > 1465) { SPEED_FORWARD = map(last3, 1466, 1900, 0, 255); }
                    }

                     if (SPEED_FORWARD == 0) {
                        //Движение назад
                        motor1SetSpeed(SPEED_BACKWARD);
                        motor3SetSpeed(SPEED_BACKWARD);

		        Motor1->run(BACKWARD);
		        Motor3->run(BACKWARD);
                     } else {
		           motor1SetSpeed(SPEED_FORWARD);
		             motor3SetSpeed(SPEED_FORWARD);

		             Motor1->run(FORWARD);
		             Motor3->run(FORWARD);
		       }

		     SPEED_RIGHT = 0;
		     SPEED_LEFT = 0;

		     if (last4 < 1000) {SPEED_LEFT = 0; SPEED_RIGHT = 0;}
		     else {
		       if (last4 < 1425) { SPEED_LEFT = map(last4, 1000, 1430, 255, 0); }
		       else if (last4 > 1500) { SPEED_RIGHT = map(last4, 1500, 1950, 0, 255); }
		     }

		     if (SPEED_LEFT == 0) {
		       //Движение вправо
		       motor2SetSpeed(SPEED_RIGHT);

		       Motor2->run(FORWARD);
		       }

		     else {
		       //Движение влево
		       motor2SetSpeed(SPEED_LEFT);

		       Motor2->run(BACKWARD);
		       }

		 Serial.print("3:"); Serial.print(last3); Serial.print(" 4:"); Serial.println(last4);
		 Serial.print("MOTOR F:"); Serial.print(SPEED_FORWARD); Serial.print(" B:"); Serial.print(SPEED_BACKWARD); Serial.print(" LEFT:"); Serial.print(SPEED_LEFT);Serial.print(" RIGHT:"); 
                 Serial.print(SPEED_RIGHT);Serial.println("");

		 }

		 // инициализация моторов
		 void motorInit() {
		    AFMS.begin(); //Соединение с контроллером
		 }

		 // установить скорость 0--255
		 void motor1SetSpeed(int speed) {
		    // скорость мотора 0--255
		    if (speed > 255)    speed = 255;
		    if (speed < 0)    speed = 0;

		    Motor1->setSpeed(speed);
		 }

		 // установить скорость 0--255
		 void motor3SetSpeed(int speed) {
		    // скорость мотора 0--255
                    if (speed > 255)    speed = 255; 
                    if (speed < 0)    speed = 0;

		    Motor3->setSpeed(speed);
		 }

		 // установить скорость 0--255
		 void motor2SetSpeed(int speed) {
		    // скорость мотора 0--255
		    if (speed > 255)    speed = 255;
		    if (speed < 0)    speed = 0;

		    Motor2->setSpeed(speed);
		 }

		 //UltraSonic Sensor
		 int readDistance(){
		    digitalWrite(trigPin, LOW);
		    delayMicroseconds(2);
		    digitalWrite(trigPin, HIGH);
		    delayMicroseconds(10);
		    digitalWrite(trigPin, LOW);
		    duration = pulseIn(echoPin, HIGH);

		    //Calculate the distance (in cm) based on the speed of sound.
		    distance = duration/58.2;

		    if (distance >= maximumRange || distance <= minimumRange){
		       Serial.println("Distance: -1");
		       return -1;
		    } else {
		       Serial.println(distance);
		       return distance;
		    }
		 }
