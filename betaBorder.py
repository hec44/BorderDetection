"""
Vertix detection on multiple squares
by: hec44
"""
import numpy as np
import cv2
from matplotlib import pyplot as plt
from mpl_toolkits.axes_grid1 import ImageGrid
import math

img = cv2.imread('f.jpg')
#img = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
val_magic=300
r = (val_magic*1.0) / img.shape[1]
dim = (val_magic, int(img.shape[0] * r))

# perform the actual resizing of the image and show it
img = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
plt.imshow(img)
cv2.waitKey(0)
# Prepocess
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
blur = cv2.GaussianBlur(gray,(1,1),1000)
#blur = cv2.medianBlur(gray,100)
flag, thresh = cv2.threshold(blur, 120, 255, cv2.THRESH_BINARY)
#sobelx = cv2.Sobel(blur,cv2.CV_64F,1,0,ksize=5)
edges = cv2.Canny(thresh,100,200)
_, cnts, _ = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = sorted(cnts, key = cv2.contourArea, reverse = True)
screenCnt = None
# loop over our contours
for c in cnts:
    print(cv2.contourArea(c))
for c in cnts:
    # approximate the contour
    peri = cv2.arcLength(c, True)
    approx = cv2.approxPolyDP(c, 0.05 * peri, True)

    # if our approximated contour has four points, then
    # we can assume that we have found our card
    if len(approx) >= 4:

        screenCnt = approx
        if len(approx)==4:
            #skip kmeans if len == 4
            break
        #Find best 4 corners by kmean
        # convert to np.float32
        Z = np.float32(screenCnt)
        # define criteria and apply kmeans()
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
        _,_,screenCnt=cv2.kmeans(Z,4,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)
        screenCnt = np.int32(screenCnt)
        break
print(screenCnt.squeeze())
for point in screenCnt.squeeze():
    #print(point[0][1])
    cv2.circle(img, (point[0],point[1]), 10, (255, 0, 0), thickness=1, lineType=8, shift=0)

cv2.imwrite('imageWB.jpg',img)
