# habil & anas 
# anas.salamani@outlook.fr
# habil1996@hotmail.fr

import cv2
import serial,time
import pickle


face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Camera interne : 0, si ajout de webcam externe, mettre 1
numera_camera = 0
cam=cv2.VideoCapture(numera_camera)


# Charge le trainner du script "entrainement.py" 
#   dans la fonction createLBPHFaceRecognizer()
#recognise = cv2.face.createLBPHFaceRecognizer()
#recognise.load("trainner.yml")
labels = {} # dictionary


# Ouvre le fichier "labels.pickle" généré par entrainement.py 
#   et crée un dictionaire contenant le label ID
#with open("labels.pickle", 'rb') as f:##
#    og_label = pickle.load(f)##
#    labels = {v:k for k,v in og_label.items()}##
#    print(labels)

# On instancie un port serie "arduino" et on l'ouvre
# Si sur pc :
# serial.Serial("COM3", 9600, timeout=1)
with serial.Serial("COM3", 9600, timeout=1) as arduino:
    time.sleep(0.1) # Delay nécessaire pour la synchronisation
    if arduino.isOpen():
        while True:
            
            ret, img= cam.read()
            img=cv2.flip(img,1)
            gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
            #detection du visage
            faces = face_cascade.detectMultiScale(gray,
                                           scaleFactor=1.3,
                                           minSize=(80,80))
            
            for x,y,w,h in faces:
                # envoi les coordonnées à Arduino sous la forme de 
                #   chaîne de caractère
                string='X{0:d}Y{1:d}'.format((x+w//2),(y+h//2))
                print(x)
                print(isinstance(x,int))
                
                # Retourne la prediction de l'indentification
                face_save = gray[y:y+h, x:x+w]
                #ID, conf = recognise.predict(face_save)
                
                # Intervalle de confiance, jouer avec cette valeur pour tester la fiabilité
               #if conf >= 50 and conf <= 100:  
                    #print(ID) # Affiche sur le terminal l'ID du candidat (0;1;2 ... etc)
                    #print(labels[ID]) # Affiche le nom du candidat
                    
                    # Affiche un carré vert avec le nom de la personne reconnu
                cv2.putText(img,"bg",(x-10,y-10),cv2.FONT_HERSHEY_COMPLEX ,1,(0,255,0), 2, cv2.LINE_AA )
                img = cv2.rectangle(img, (x,y), (x+w,y+h),(0,255,0),4)
                   # Si la sortie du modele n'est pas dans l'intervalle 
                   # Si la personne n'est pas reconnu 
                #else :
                    
                    # Affiche un carré rouge avec "inconnu"
                cv2.putText(img,"inconnu",(x-10,y-10),cv2.FONT_HERSHEY_COMPLEX ,1,(0,0,255), 2, cv2.LINE_AA )
                img = cv2.rectangle(img, (x,y), (x+w,y+h),(0,0,255),4)
                    
                
                # On envoi à la carte arduino UNO la valeur des positions.
                arduino.write(string.encode('utf-8'))

            # Dessin d'un rectangle blanc qui va "viser" le visage
            # Servir pour le deplacement de la caméra
            cv2.rectangle(img,(640//2-70,480//2-70),
                         (640//2+70,480//2+70),
                          (255,255,255),3)

            cv2.imshow('img',img)


            # Pour cesser le flux video appuyer sur 'q' : 
            if cv2.waitKey(10)&0xFF== ord('q'):
                break
