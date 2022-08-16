#include <SPI.h>
#include <SoftwareSerial.h>

#define LOAD          10   /* Entrée LOAD du MAX72219 */
#define pot           A6
#define mot1           6
#define mot2           5
#define rxPin          3
#define txPin          4
#define bp_manu        A3
#define bp_bluetooth   A2
#define bp_auto        A1
#define led_manu       7
#define led_bluetooth  8
#define led_auto       9

#define time_delay 500

// MAX7219 Configuration
#define DISPLAY_TEST  0x0F /* Registre Display-Test */
#define SHUTDOWN      0x0C /* Registre Shutdown     */
#define DECODE_MODE   0x09 /* Registre Decode-mode  */
#define SCAN_LIMIT    0x0B /* Registre Scan-limit   */
#define INTENSITY     0x0A /* Registre Intensity    */

const int nb_led = 7;           /* Number of LED         */

//int power_lut[nb_led+1] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F};
int Chenillard_lut[nb_led+1] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40};
int one_led_lut[nb_led] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};
int Demo_start_lut[3][7] = {
  {0x08,0x1C,0x3E,0x7F,0x7F,0x7F,0x7F},
  {0x40,0x60,0x70,0x78,0x7C,0x7E,0x7F},
  {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F}
};

int power_lut[3][8] = {
  {0x00,0x08,0x1C,0x3E,0x7F,0x7F,0x7F},
  {0x40,0x60,0x70,0x78,0x7C,0x7E,0x7F},
  {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F}
};


unsigned long delay_ms;

char c;
int puissance;
int bargraph;



const byte nb_digits = 3;   /* nombre de digits du bargraph */
const byte nb_segments_digit = 4; /* nombre de segments par digit */



int demo = 0;
boolean sw_mode;

int bp_auto_default_state = HIGH;
int bp_manu_default_state = HIGH;
int bp_bluetooth_default_state = HIGH ;

int bp_auto_state = 0;
int bp_manu_state = 0;
int bp_bluetooth_state = 0;


int state_led_manu = LOW;
int state_led_auto = LOW;
int state_led_bluetooth = LOW;


int Start = 1;

SoftwareSerial Bluetooth (rxPin, txPin);
  
void setup() {

  Serial.begin(9600);
  Bluetooth.begin(9600);

// INPUT OUTPUTx
  setPwmFrequency(5, 8);
  setPwmFrequency(5, 8);
   //TCCR0B = TCCR0B & B11111000 | B00000010; // for D5;D6 7.8kHz, NOTE : it affects delay and millis() ;
  //TCCR0B = TCCR0B & B11111000 | B00000011;  // for D5;D6 976Hz
  // Motor
  pinMode(mot1, OUTPUT); 
  pinMode(mot2, OUTPUT); 
  // Bluetooth 
  pinMode(rxPin,INPUT);   
  pinMode(txPin,OUTPUT);
  // Max7219
  pinMode(LOAD, OUTPUT); // MOSI Pin
  // Variator
  pinMode(pot, INPUT);   // Speed variator
  // Mode 
  pinMode(bp_manu,INPUT_PULLUP);
  pinMode(bp_auto,INPUT);
  pinMode(bp_bluetooth, INPUT);
  
  pinMode(led_manu, OUTPUT);
//  digitalWrite(led_manu,LOW);
  pinMode(led_bluetooth, OUTPUT);
//  digitalWrite(led_bluetooth,LOW);
  pinMode(led_auto, OUTPUT);
//  digitalWrite(led_auto, LOW);
  //  pinMode(sw, INPUT); // switching mode


  
  digitalWrite(LOAD, HIGH);
  
  SPI.begin();
  //SPI.setBitOrder(MSBFIRST); /* bits de poids fort en premier */

  writeRegister(DISPLAY_TEST, 0x00); /* Normal operation */
  writeRegister(DECODE_MODE, 0x00); /* No decode */
  writeRegister(SCAN_LIMIT, nb_digits + 1 ); /* Display digits 0, 1, 2 */
  writeRegister(INTENSITY, 0xF);   /* LED brightness, max=0x0F */
  writeRegister(SHUTDOWN, 0x01);    /* Normal operation */
  writeRegister(1, 0x00);
  writeRegister(2, 0x00);


  switch_off();
  //writeRegister(SHUTDOWN, 0x00);  /* Shutdown mode */
}

void loop()
{

  if (demo == 0){
    start(Demo_start_lut);
    demo= 1; 
  }

  select_mode();
//  sw_mode = digitalRead(sw);
  /* 
  if (sw_mode == manual_mode){
    Speed_Control_Manual_Acquisition();  
  }
  else if (sw_mode == bluetooth_mode){
    Speed_Control_Bluetooth_Acquisition();
  }
  */
  
  //runningLed(one_led_lut);

}

void writeRegister(byte thisRegister, byte thisValue){ // Écrire dans un registre du MAX7219
  // Mettre l'entrée LOAD à l'état bas
  digitalWrite(LOAD, LOW);

  SPI.transfer(thisRegister); // Adresse du registre
  SPI.transfer(thisValue); // Contenu du registre

  // Basculer l'entrée LOAD à l'état haut pour verrouiller et transférer les données
  digitalWrite(LOAD, HIGH);
}

void select_mode(){
  
  bp_auto_state      = digitalRead(bp_auto);
  bp_manu_state      = digitalRead(bp_manu);
  bp_bluetooth_state = digitalRead(bp_bluetooth);

  if((bp_manu_state != bp_manu_default_state)) {
    bp_manu_default_state = bp_manu_state;
    if(bp_manu_state == LOW){
      state_led_manu =! state_led_manu;
      digitalWrite(led_manu, state_led_manu);
    }    
  }


  
}

void runningLed(int one_led_lut[7]){
  switch_off();
  for (int digit = 1 ; digit <=3 ; digit++){
    for(int segment = 0 ; segment <7; segment ++){
      writeRegister(digit,one_led_lut[segment]);
      delay_ms = millis(); while(millis()<(delay_ms + time_delay)){}  
    }
    writeRegister(digit,0);
  }
  for (int digit = 3 ; digit >=1 ; digit--){
    for(int segment = nb_led-1 ; segment >=0; segment --){
      writeRegister(digit,one_led_lut[segment]);
      delay_ms = millis(); while(millis()<(delay_ms + time_delay)){}  
    }
    writeRegister(digit,0);
  }
  
}

void start(int Demo_start_lut[3][7]){

  for (int i = 0 ; i < 2 ; i++){

    for(int j = 0; j<4; j++){
      writeRegister(2,Demo_start_lut[0][j]);  
      delay_ms = millis(); while(millis()<(delay_ms + time_delay)){}  
      }
  
    for(int k = 0; k<7; k++){ 
      writeRegister(1,Demo_start_lut[1][k]);
      writeRegister(3,Demo_start_lut[2][k]);   
      delay_ms = millis(); while(millis()<(delay_ms + time_delay)){}
      }
     switch_off();
  }
    switch_off();  
}

void switch_off(){
    writeRegister(1,0);
    writeRegister(2,0);
    writeRegister(3,0);
}

void Speed_Control(int power){
  power = map(power,0,1023,0,255);
  analogWrite(mot1, power);
  analogWrite(mot2, power);
}

void Speed_Control_Manual_Acquisition(){



  int power = analogRead(pot);
  
   
  //Serial.println(pot); //Debug purpose
  
  Speed_Control(power);
  Speed_Control_Display(power);   
}

void Speed_Control_Display (int power){
  
  int Speed = map(power,0,1020,13,0);
  int led = Speed;
  if(Speed<5){
    writeRegister(2,power_lut[0][led]);
    writeRegister(1,0);
    writeRegister(3,0); 
  }
  if(Speed>=5 &&Speed <=11){
    led = led-5;
    writeRegister(2,0x7F);
    writeRegister(1,power_lut[1][led]);
    writeRegister(3,power_lut[2][led]); 
  }
  
}

void Speed_Control_Bluetooth_Acquisition(){
  char c ;
  String mot;
  while(Bluetooth.available()){
    delay(2);
    c = Bluetooth.read();
    mot += c;
  }

  if (mot.length()>0){
    //Serial.print("taille mot :");
    //Serial.println(strlen(mot));
    //Serial.println(mot);
    
    //int power = atoi(mot);
    //Serial.println(mot);
    String value = mot.substring(0,mot.length());
    int power = value.toInt();
    Serial.println(power);
    Speed_Control(power);
    Speed_Control_Display(power);  
    
    
  }
  mot="";
  
}

void setPwmFrequency(int pin, int divisor) {
   byte mode;
   if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
      switch(divisor) {
         case 1: mode = 0x01; break;
         case 8: mode = 0x02; break;
         case 64: mode = 0x03; break;
         case 256: mode = 0x04; break;
         case 1024: mode = 0x05; break;
         default: return;
      }
      if(pin == 5 || pin == 6) {
         TCCR0B = TCCR0B & 0b11111000 | mode;
      } else {
         TCCR1B = TCCR1B & 0b11111000 | mode;
      }
   } else if(pin == 3 || pin == 11) {
      switch(divisor) {
         case 1: mode = 0x01; break;
         case 8: mode = 0x02; break;
         case 32: mode = 0x03; break;
         case 64: mode = 0x04; break;
         case 128: mode = 0x05; break;
         case 256: mode = 0x06; break;
         case 1024: mode = 0x7; break;
         default: return;
      }
      TCCR2B = TCCR2B & 0b11111000 | mode;
   }
}
