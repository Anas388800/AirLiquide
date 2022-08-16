int WRIST = 6;    // WRIST  MOTOR SPEED CONTROL
int potentiometerPin = A6; 
int PWM;

String mot;

#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial mySerial (rxPin, txPin);

void setup(){ 
  pinMode(WRIST, OUTPUT);
  pinMode(potentiometerPin, INPUT);
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);

} 
 
void loop() {
   
  
  while (mySerial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = mySerial.read();
    mot += c;
  }


  
  if (mot.length() >0) {
    
    mySerial.println(mot);
    Serial.println(mot);
    
    if(mot[0]=='0'){
    
      //Serial.println("1 envoyée "); 
      PWM =  0; 
     }
    
    if(mot[0]=='1'){
    
      //Serial.println("1 envoyée "); 
      PWM =  80; 
     }
    
    
    if(mot[0]=='2'){
      
      //Serial.println("3 envoyée ");
      PWM =  120;  
      } 

    if(mot[0]=='3'){
      
      //Serial.println("3 envoyée ");
      PWM =  150;  
      }

    if(mot[0]=='4'){
      
      //Serial.println("2 envoyée ");
      PWM =  180;  
      }
    if(mot[0]=='6'){
      
      //Serial.println("2 envoyée ");
      PWM =  200;  
      }

          if(mot[0]=='7'){
      
      //Serial.println("2 envoyée ");
      PWM =  230;  
      }

          if(mot[0]=='8'){
      
      //Serial.println("2 envoyée ");
      PWM =  255;  
      }
         
    }

  
//  for (int i=0;i<PWM;i++){
//    analogWrite(WRIST, i);
//    delay(50);
//  }
  analogWrite(WRIST, PWM);
  
   
  Serial.println(PWM);
  mot="";   // vide la variable stockant le code 

      
} 





  
//    while(mySerial.available())
//    Serial.println((char)mySerial.read());
