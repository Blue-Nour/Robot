
//Code Robot final  //main

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <NewPing.h>

 
#define SS_PIN 53
#define RST_PIN 5


#define enA 3
#define in1 39
#define in2 37
#define enB 4
#define in3 43
#define in4 45

int motorSpeedA = 0;
int motorSpeedB = 0;

int Sw = 49; 
const int pinultra = 23; // Trigger Pin of Ultrasonic Sensor
const int pinson = 25; // Echo Pin of Ultrasonic Sensor

int dist;
int vitesse;

// defines variables
//long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);


MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
bool pass = 0;

// Init array that will store new NUID 
byte nuidPICC[4];


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(Sw,INPUT_PULLUP);
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);
  pinMode(pinultra, OUTPUT);
  pinMode(pinson, INPUT);

 Serial.begin(9600);   // Initiate a serial communication
 SPI.begin();      // Initiate  SPI bus
 mfrc522.PCD_Init();   // Initiate MFRC522
 Serial.println("Approximate your card to the reader...");
 Serial.println();

 Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
 Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
 Serial.println("with Arduino UNO R3");


}

void loop() {
   distance = distance_sonar(); 
   int xAxis = analogRead(A9); // Read Joysticks X-axis
   int yAxis = analogRead(A8); // Read Joysticks Y-axis 
  
 
  if(pass == 0){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  lcd.setCursor(0,0);
  Serial.print("UID tag :");
  lcd.print("UID:");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     lcd.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  lcd.print("Message : ");
  content.toUpperCase();
 // if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
  if (content.substring(1) == "47 48 C0 A7")
  {
    lcd.setCursor(0,1);
    Serial.println("Authorized access");
    Serial.println();
    lcd.print("Auth access     ");
    lcd.println();
    pass = 1;
    delay(3000);
  }
 
 else   {
    
    lcd.setCursor(0,1);
    Serial.println(" Access denied");
    lcd.print(" Access denied  ");
    delay(3000);
    return;
 }}
 else{
   // Y-axis used for forward and backward control
  if (yAxis < 470) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 255);
    motorSpeedB = map(yAxis, 470, 0, 0, 255);

  }
  else if (yAxis > 550) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
   
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 550, 1023, 0, 255);
    motorSpeedB = map(yAxis, 550, 1023, 0, 255);

  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
 
  }

  // X-axis used for left and right control
  if (xAxis < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
    
  }
  if (xAxis > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    
    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
    
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
   
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B


  if (digitalRead(49) == 1){
  
  Serial.println (distance);
  }
  else {
    Serial.print("not");
  }

}}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

int distance_sonar(){
  long duration, inches, cm;
  bool state;
  digitalWrite(pinultra, LOW);
  delayMicroseconds(2);
  digitalWrite(pinultra, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinultra, LOW);
  
  duration = pulseIn(pinson, HIGH);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print ("dist = ");
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();
  }