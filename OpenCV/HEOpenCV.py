import cv2
import numpy as np

img = cv2.imread('0787.bmp',0)
equ = cv2.equalizeHist(img)
res = np.hstack((img,equ)) #stacking images side-by-side
cv2.imwrite('result.bmp',res)
