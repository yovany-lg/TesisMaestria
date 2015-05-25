import numpy as np
import cv2



for x in xrange(1,10):
	pass


def claheFiltering(fileName,i):
	img = cv2.imread('0855.bmp', 0)

	# create a CLAHE object (Arguments are optional).
	clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
	cl1 = clahe.apply(img)

	cv2.imwrite("frame"+i+".bmp", cl1)
	# pass