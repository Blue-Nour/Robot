//Code Robot final
#include "SerialTransfer.h"
#include "Wire.h"
#include <NewPing.h>

#define enA 3  // vitesse moteurs gauches
#define in1 39  // direction moteurs gauches
#define in2 37
#define enB 4  // vitesse moteurs droits
#define in3 43  // direction moteurs droits
#define in4 45

int motorSpeedA = 0;
int motorSpeedB = 0;

const int pinultra = 23; // Trigger Pin of Ultrasonic Sensor
const int pinson = 25; // Echo Pin of Ultrasonic Sensor
int dist;
int vitesse;
bool Start;
#include <Servo.h>
Servo monServomoteur;
Servo monServomoteur2;
Servo monServomoteur3;

SerialTransfer myTransfer;

struct STRUCT {
  int Joy1X ;
  int Joy1Y;
  int Joy2X;
  int Joy2Y;
  bool BPBuzzer;
  bool BPJoy1;
  bool BPJoy2;
  char Mode;
  int Dist_sonar;
  char RFID_State;
} testStruct;

char arr[6];


void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);

  pinMode(pinultra, OUTPUT);
  pinMode(pinson, INPUT);

 monServomoteur.attach(7);   //servo crémaillère
  monServomoteur2.attach(8);   //servo crémaillère
  monServomoteur3.attach(6);    //servo ultrason
  Serial.begin(9600);
  monServomoteur3.write(90);
  Start = 1;


  Serial.begin(38400);
  Serial2.begin(38400);
  myTransfer.begin(Serial2);
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);


}

void loop()
{

  int droite,devant,gauche;
  droite = Turn_sonar("droite");
  gauche = Turn_sonar("gauche");
  devant = Turn_sonar("devant");
if (Start_auto){
    vitesse = 900;
    
    if (droite and gauche and devant){
      Serial.println("deriere");
      motor_auto("deriere");
      delay(700);  
      motor_auto("arret");
        
      }
    else if (droite){
      Serial.println("gauche");
      motor_auto("gauche");
      delay(700);
      motor_auto("arret");
      
    }
    else if (gauche or devant){
      Serial.println("droite");
      motor_auto("droite");
      delay(700);
      motor_auto("arret");
      
    }
    else{
      Serial.println("devant");
      motor_auto("devant");
      delay(700);
      motor_auto("arret");
      }
      }}
    
 void motor_auto(char*dir){
  if (vitesse == NULL){
  }
  if (dir == "droite"){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(enA, 200); // vitesse moteurs gauches
    analogWrite(enB, 200);
    
    
  }
  else if (dir == "gauche"){
     // Set Motor A backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 200); // vitesse moteurs gauches
    analogWrite(enB, 200);
  }
  else if (dir == "arret"){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 0); // vitesse moteurs gauches
    analogWrite(enB, 0);
  }
  else if( dir == "devant"){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 200); // vitesse moteurs gauches
    analogWrite(enB, 200);
    }
   else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(enA, 200); // vitesse moteurs gauches
    analogWrite(enB, 200);
    
    }
  }
bool Turn_sonar(char* dir){
 Cremaillere(0);
 int state;
 if (dir == "droite"){
  monServomoteur3.write(150);
  delay(300);}
 else if (dir == "gauche"){
  monServomoteur3.write(30);
  delay(300);
 }
 else if (dir == "devant"){
  monServomoteur3.write(90);
  delay(300);
 }
 delay(700);
 int Son = distance_sonar();
 if (Son < 20){
    state = 1;
    Serial.print("Oui");
    Serial.print(" ");
    Serial.println(Son);
    }
   else{state = 0;
    Serial.print("Non");
    Serial.print(" ");
    Serial.println(Son);}
return state;    
    
}


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
  return cm;
  }
 void Cremaillere(int dir){
 //Placer ici le code pour la cremaillere 
 if( dir){ monServomoteur.write(180);
    monServomoteur2.write(0);
    monServomoteur3.write(88);}
 else{monServomoteur.write(40);    //crémaillère basse
    monServomoteur2.write(140);}
  


  if(myTransfer.available())
  {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    recSize = myTransfer.rxObj(testStruct, recSize);
    Serial.print("valx : ");
    Serial.println(testStruct.Joy1X);
  
  }
  
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;

  ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(testStruct, sendSize);

  ///////////////////////////////////////// Stuff buffer with array
  sendSize = myTransfer.txObj(arr, sendSize);

  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);

  
  delay(700);
}