#include <SPI.h>
#include <SoftwareSerial.h>

#define LOAD          10   /* Entrée LOAD du MAX72219 */

#define DISPLAY_TEST  0x0F /* Registre Display-Test */
#define SHUTDOWN      0x0C /* Registre Shutdown */
#define DECODE_MODE   0x09 /* Registre Decode-mode */
#define SCAN_LIMIT    0x0B /* Registre Scan-limit */
#define INTENSITY     0x0A /* Registre Intensity */


char c;
int puissance;
String mot;
enum color_t {NOCOLOR, RED, GREEN, YELLOW};
void bargraphDraw(color_t const *mybargraph);
void levelGauge(unsigned int percentage);

byte lum = 0x00;
const byte nb_digit = 3;   /* nombre de digits du bargraph */
const byte nb_segment = 7; /* nombre de segments par digit */

void setup() {
  pinMode(LOAD, OUTPUT);
  digitalWrite(LOAD, HIGH);
  Serial.begin(9600);
  SPI.begin();
  
  //SPI.setBitOrder(MSBFIRST); /* bits de poids fort en premier */

  writeRegister(DISPLAY_TEST, 0x00); /* Normal operation */
  writeRegister(DECODE_MODE, 0x00); /* No decode */
  writeRegister(SCAN_LIMIT, nb_digit ); /* Display digits 0, 1, 2 */
  writeRegister(INTENSITY, 15);   /* LED brightness, max=0x0F */
  writeRegister(SHUTDOWN, 0x01);    /* Normal operation */
  writeRegister(1, 0x00);
  writeRegister(2, 0x00);

//  demo();  /* Lancer la démo */

  //writeRegister(SHUTDOWN, 0x00);  /* Shutdown mode */
}

void writeRegister(byte thisRegister, byte thisValue) // Écrire dans un registre du MAX7219
{
  // Mettre l'entrée LOAD à l'état bas
  digitalWrite(LOAD, LOW);

  SPI.transfer(thisRegister); // Adresse du registre
  SPI.transfer(thisValue); // Contenu du registre

  // Basculer l'entrée LOAD à l'état haut pour verrouiller et transférer les données
  digitalWrite(LOAD, HIGH);
}

void loop()
{
 /* 
  while(Serial.available()){
    delay(2);
    c = Serial.read();
    mot += c;
    
  }
  

  if(mot.length()>0){
    puissance = mot[0] - '0';
    Serial.println(puissance);
  }
  */
  
    
      byte barGraph;
      barGraph = 0;
      for (byte d = 0 ; d < nb_digit ; d++){
        for (byte s = 0 ; s < nb_segment; s++){
          barGraph |=(1<<s);  
          delay(100);     
          writeRegister(d+1, barGraph);
        } 
      }
      
      //lum |= 0x;

      //writeRegister(1, lum);


  //mot ="";

  
}
