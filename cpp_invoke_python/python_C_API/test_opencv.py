import cv2 as cv

"""
In [1]: import test_opencv

In [2]: a = test_opencv.read_img('py.png')

In [3]: a.shape
Out[3]: (16, 16, 3)

In [4]: a[:10, 0, 0]
Out[4]: array([  0,   0,   0,   0, 198, 195, 190, 185, 178, 174], dtype=uint8)

In [5]: a = test_opencv.scale_up(a)

In [6]: a.shape
Out[6]: (32, 32, 3)

In [7]: a[:10, 0, 0]
Out[7]: array([  0,   0,   0,   0,   0,   0,   0,  50, 149, 197], dtype=uint8)
"""

def read_img(img_path):
    return cv.imread(img_path)

def scale_up(img, scale=2):
    height, width = img.shape[:2]
    dsize = (width*scale, height*scale)
    big_img = cv.resize(img, dsize)
    print("big_img[0] in python:", big_img[0])
    return big_img