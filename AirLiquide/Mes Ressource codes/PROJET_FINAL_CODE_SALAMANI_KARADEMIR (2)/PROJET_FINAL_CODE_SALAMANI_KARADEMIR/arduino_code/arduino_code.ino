#include<Servo.h>
Servo x, y; // On instancie 2 servo moteur


int position_x = 90, position_y = 0;           // les positions initial du moteur au démarrage du programme

int largeur = 640, hauteur = 480;             // taille/resolution  de la video

void setup() {

  Serial.begin(9600); // initialise le port serie avec la vitesse de transmission
  
  // On declare les 2 servos moteur en x et y broche 9;10
  x.attach(9);  
  y.attach(10);

  // On initialise leur position initiale
  x.write(position_x);
  y.write(position_y);
}
const int angl_increment = 1;   // degree d'increment

void loop() {
  if (Serial.available() > 0) // Si port serie activé
  {
    int position_x, position_y;
    
    if (Serial.read() == 'X') // Si on recoit 'X' on recupere la valeur qui suit
    {
      position_x = Serial.parseInt();  // on recupere l'entier qui suit le 'X'

      
      if (Serial.read() == 'Y'){  //Si on recoit 'Y' on recupere la valeur qui suit
        position_y = Serial.parseInt(); // on recupere l'entier qui suit le 'Y'
      }
        
    }


    // Definition d'un rectangle imaginaire dans lequel l'utilisateur devra être
    
    if (position_x > largeur / 2 + 70){
      position_x -= angl_increment;
    } 
    if (position_x < largeur / 2 - 70){
      position_x += angl_increment;
    }  
    if (position_y < hauteur / 2 + 70){
      position_y -= angl_increment;
    }
    if (position_y > hauteur / 2 - 70){
      position_y += angl_increment;
    }
        
    // Réajuste si POSITION recu supérieur à 180 ou inferieur à 0
    //      pour eviter des mouvements incohérent du servo moteur
    
    if (position_x >= 180){
      position_x = 180;
    }
    else if (position_x <= 0){
      position_x = 0;
    }    
    if (position_y >= 180){
      position_y = 180;
    }
    else if (position_y <= 0){
      position_y = 0;
    }


    // Ensuite, on applique ces deux valeur d'angles aux 2 servos
      
    x.write(position_x);
    y.write(position_y);

  }
}
