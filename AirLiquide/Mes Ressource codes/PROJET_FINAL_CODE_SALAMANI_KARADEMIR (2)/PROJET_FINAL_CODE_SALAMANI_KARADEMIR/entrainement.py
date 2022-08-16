# habil & anas 
# anas.salamani@outlook.fr
# habil1996@hotmail.fr

import cv2
import os
import numpy as np
from PIL import Image
import pickle


cascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

recognise = cv2.face.createLBPHFaceRecognizer()

# Fonction qui va parcourir le repertoire courant et qui va chercher toute les sous 
#       dossier en recupérant les photos présentes et en les étiquetant

def getdata():

    current_id = 0  # nombre de "candidat"
    label_id = {}   # dictionaire
    face_train = [] # liste qui recupere les images 
    face_label = [] # list qui recupere l'etiquette de l'image
    
    # Trouve le repertoire dans lequel on se trouve
    repertoire_courant = os.path.dirname(os.path.abspath(__file__))
    
    # "image_data" est le fichier qui contient les données (photos)
    # On intégre son path au path principale
    my_face_dir = os.path.join(repertoire_courant,'image_data')
    # Parcours tout les fichier et document
    #  voir tuto ici : https://www.geeksforgeeks.org/os-walk-python/
 
    for root, dirs, files in os.walk(my_face_dir):
        for file in files:
            
            # Verifie si l'extension est ".png" ou ".jpg"
            if file.endswith("png") or file.endswith("jpg"):
                # Intègre le path de l'image 
                path = os.path.join(root, file)
                # Prend le nom du fichier comme label exemple : "label = habil"
                label = os.path.basename(root).lower()
                # fournit le label ID
                if not label in label_id:
                    label_id[label] = current_id
                    current_id += 1
                ID = label_id[label]
                
                # convertie l'image en niveau de gris
                # Peut aussi utiliser la methode de cv2 "gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)"
                pil_image = Image.open(path).convert("L")
                # convertie l'image en numpy array
                image_array = np.array(pil_image, "uint8")
        
                face = cascade.detectMultiScale(image_array)

                # Trouve la region d'interet "roi" et ajoute les données
                for x,y,w,h in face:
                    img = image_array[y:y+h, x:x+w]
                    cv2.imshow("Test",img)
                    cv2.waitKey(1)
                    face_train.append(img)
                    face_label.append(ID)

    # On chaîne les données d'étiquettes dans un fichier
    with open("labels.pickle", 'wb') as f:
        pickle.dump(label_id, f)
   

    return face_train,face_label

# Creaton d'un fichier d'entraienemnt .yml
face,ids = getdata()
recognise.train(face, np.array(ids))
recognise.save("trainner.yml")
