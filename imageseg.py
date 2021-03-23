import cv2
import numpy as np   

img = cv2.imread('Dataset\\Train\\353.jpg')
img = cv2.resize(img ,((int)(img.shape[1]/5),(int)(img.shape[0]/5)))
original = img.copy()
neworiginal = img.copy() 
cv2.imwrite('original.jpg',img)


#Calculating number of pixels with shade of white(p) to check if exclusion of these pixels is required or not (if more than a fixed %) in order to differentiate the white background or white patches in image caused by flash, if present.
p = 0 
for i in range(img.shape[0]):
	for j in range(img.shape[1]):
		B = img[i][j][0]
		G = img[i][j][1]
		R = img[i][j][2]
		if (B > 110 and G > 110 and R > 110):
			p += 1
			
#finding the % of pixels in shade of white
totalpixels = img.shape[0]*img.shape[1]
per_white = 100 * p/totalpixels
'''
print 'percantage of white: ' + str(per_white) + '\n'
print 'total: ' + str(totalpixels) + '\n'
print 'white: ' + str(p) + '\n'
'''

#excluding all the pixels with colour close to white if they are more than 10% in the image
if per_white > 10:
	img[i][j] = [200,200,200]
	cv2.imwrite('color change.jpg', img)


#Guassian blur
blur1 = cv2.GaussianBlur(img,(3,3),1)


#mean-shift algo
newimg = np.zeros((img.shape[0], img.shape[1],3),np.uint8)
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER , 10 ,1.0)

img = cv2.pyrMeanShiftFiltering(blur1, 20, 30, newimg, 0, criteria)
cv2.imwrite('means shift image.jpg',img)


#Guassian blur
blur = cv2.GaussianBlur(img,(11,11),1)

#Canny-edge detection
canny = cv2.Canny(blur, 160, 290)

canny = cv2.cvtColor(canny,cv2.COLOR_GRAY2BGR)
#creating border around image to close any open curve cut by the image border 
#bordered = cv2.copyMakeBorder(canny,10,10,10,10, cv2.BORDER_CONSTANT, (255,255,255))		#function not working(not making white coloured border) 
#bordered = cv2.rectangle(canny,(-2,-2),(275,183),(255,255,255),3)
#cv2.imwriteow('Canny on meanshift bordered image',bordered)


#contour to find leafs
bordered = cv2.cvtColor(canny,cv2.COLOR_BGR2GRAY)
contours,hierarchy = cv2.findContours(bordered, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

maxC = 0
for x in range(len(contours)):													#if take max or one less than max then will not work in
	if len(contours[x]) > maxC:													# pictures with zoomed leaf images
		maxC = len(contours[x])
		maxid = x

perimeter = cv2.arcLength(contours[maxid],True)
#print perimeter
Tarea = cv2.contourArea(contours[maxid])
cv2.drawContours(neworiginal,contours[maxid],-1,(0,0,255))
cv2.imwrite('Contour.jpg',neworiginal)
#cv2.imwrite('Contour complete leaf.jpg',neworiginal)



#Creating rectangular roi around contour
height, width, _ = canny.shape
min_x, min_y = width, height
max_x = max_y = 0
frame = canny.copy()

# computes the bounding box for the contour, and draws it on the frame,
for contour, hier in zip(contours, hierarchy):
	(x,y,w,h) = cv2.boundingRect(contours[maxid])
	min_x, max_x = min(x, min_x), max(x+w, max_x)
	min_y, max_y = min(y, min_y), max(y+h, max_y)
	if w > 80 and h > 80:
		#cv2.rectangle(frame, (x,y), (x+w,y+h), (255, 0, 0), 2)   #we do not draw the rectangle as it interferes with contour later on
		roi = img[y:y+h , x:x+w]
		originalroi = original[y:y+h , x:x+w]
		
if (max_x - min_x > 0 and max_y - min_y > 0):
	roi = img[min_y:max_y , min_x:max_x]	
	originalroi = original[min_y:max_y , min_x:max_x]
	#cv2.rectangle(frame, (min_x, min_y), (max_x, max_y), (255, 0, 0), 2)   #we do not draw the rectangle as it interferes with contour

cv2.imwrite('ROI.jpg', frame)
cv2.imwrite('rectangle ROI.jpg', roi)
img = roi


#Changing colour-space
#imghsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
imghls = cv2.cvtColor(roi, cv2.COLOR_BGR2HLS)
cv2.imwrite('HLS.jpg', imghls)
imghls[np.where((imghls==[30,200,2]).all(axis=2))] = [0,200,0]
cv2.imwrite('new HLS.jpg', imghls)

#Only hue channel
huehls = imghls[:,:,0]
cv2.imwrite('img_hue hls.jpg',huehls)
#ret, huehls = cv2.threshold(huehls,2,255,cv2.THRESH_BINARY)

huehls[np.where(huehls==[0])] = [35]
cv2.imwrite('img_hue with my mask.jpg',huehls)


#Thresholding on hue image
ret, thresh = cv2.threshold(huehls,28,255,cv2.THRESH_BINARY_INV)
cv2.imwrite('thresh.jpg', thresh)


#Masking thresholded image from original image
mask = cv2.bitwise_and(originalroi,originalroi,mask = thresh)
cv2.imwrite('masked out img.jpg',mask)


#Finding contours for all infected regions
contours,heirarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
Infarea = 0
for x in range(len(contours)):
	cv2.drawContours(originalroi,contours[x],-1,(0,0,255))
	cv2.imwrite('Contour masked.jpg',originalroi)
	
	#Calculating area of infected region
	Infarea += cv2.contourArea(contours[x])

if Infarea > Tarea:
	Tarea = img.shape[0]*img.shape[1]

print(Infarea)
try:
	per = 100 * Infarea/Tarea
except ZeroDivisionError:
	per = 0

print(per)