import cv2
from HandTrackingModule import HandDetector

class Main:
    def __init__(self):
        self.camera = cv2.VideoCapture(0,cv2.CAP_DSHOW) # Incoming as video stream
        self.camera.set(3, 1280) # Set resolution
        self.camera.set(4, 720)
        
    def Gesture_recognition(self):
        while True:
            self.detector = HandDetector()
            frame, img = self.camera.read()
            img = self.detector.findHands(img) # Find your hand
            lmList, bbox = self.detector.findPosition(img) # Get the orientation of your hand
             
            cv2.imshow("camera", img)
            if cv2.getWindowProperty('camera', cv2.WND_PROP_VISIBLE) < 1:
                break
            # Exit the program by pressing the close button
            cv2.waitKey(1)   
            # if cv2.waitKey(1) & 0xFF == ord("q"):
            #     break # Press q to exit
