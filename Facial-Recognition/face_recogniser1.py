import cv2
import pickle
import serial
import time

# Setting serial port to COM2 at bard rate of 9600
port = serial.Serial('COM2',9600)

video = cv2.VideoCapture(0)
cascade = cv2.CascadeClassifier("D:/four_one/esd_lab/Facial-Recognition/haarcascade_frontalface_default.xml")

recognise = cv2.face.LBPHFaceRecognizer_create()        #Local Binary Pattern Histogram
print(dir (cv2.face))
recognise.read("D:/four_one/esd_lab/Facial-Recognition/trainner.yml")

labels = {}
with open("D:/four_one/esd_lab/Facial-Recognition/labels.pickle", 'rb') as f:
    og_label = pickle.load(f)
    labels = {v:k for k,v in og_label.items()}
    print(labels)


count_if = 0
count_else = 0

while True:
    check,frame = video.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    face = cascade.detectMultiScale(gray, scaleFactor = 1.1, minNeighbors = 5)
    #print(face)

    for x,y,w,h in face:
        face_save = gray[y:y+h, x:x+w]

        ID, conf = recognise.predict(face_save)
        #print(ID,conf)
        if conf >= 20 and conf <= 50:
            #print(ID)
            print(labels[ID])
            cv2.putText(frame,labels[ID],(x-10,y-10),cv2.FONT_HERSHEY_COMPLEX ,1, (18,5,255), 2, cv2.LINE_AA )

            # Checking the ID
            # Replace the "ID == 2" with your ID so that LED chaser can start
            # working
            if(ID == 0 or ID == 1):
                count_if = count_if + 1
                print("COUNT IF: ", count_if)
                count_else = 0
                if count_if >= 10:
                    port.write(str.encode('1'))
                    print("sent 1")
                    count_if = 0


            # These are the ID other than your face.
            
        else:
            count_else = count_else + 1
            print("COUNT ELSE:", count_else)
            if count_else >= 10:
                port.write(str.encode('0'))
                print("Sent 0")
        frame = cv2.rectangle(frame, (x,y), (x+w,y+h),(0,255,255),4)

    cv2.imshow("Video",frame)
    key = cv2.waitKey(1)
    if(key == ord('q')):
        break

video.release()
cv2.destroyAllWindows()
