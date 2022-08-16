# habil & anas 
# anas.salamani@outlook.fr
# habil1996@hotmail.fr

import cv2
import os

cam = cv2.VideoCapture(0)

# Regler la résolution 
cam.set(3, 640)
cam.set(4, 480)

face_detector = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")


face_id = input('\n entrer id de utilisateur ==>  ')

print("\n Fixer la caméra..")

cnt = 10

while(True):

    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_detector.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
        
        cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)     
        cnt += 1

        # Sauvegarder l'image 
        cv2.imwrite("User." + str(face_id) + '.' + str(cnt) + ".jpg", 
                    gray[y:y+h,x:x+w])

        cv2.imshow('image', img)

    # Pour cesser le flux video appuyer sur 'q' : 
    if cv2.waitKey(10)&0xFF== ord('q'):
                break
            
    
    elif cnt >= 100: # Prendre 100 photo __ valeur à modifier si besoin
         break

# Fermer la cam
cam.release()
cv2.destroyAllWindows()
