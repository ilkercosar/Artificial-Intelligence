from sys import exit
import cv2
from time import time 
import speechManager

if __name__ == "__main__":
    startTime = time()
    sM = speechManager.speechManager()
    sM.startSpeechManager()
    stopTime = time()
    
    print(f"İşlemlerin başlatılma süresi {stopTime - startTime}")

    while True:

        if sM.checkStopFlag() == False:
            break

    exit(0)