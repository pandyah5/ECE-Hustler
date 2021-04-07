import binascii
import cv2
import numpy as np
import io

filename = "D:\\Hetav Documents\\UofT\Academic Planning\\ECE 243\\Final Project\Sprites\\b1_walk_1.jpg"

# C++ code to convert hex-24 bits to hex-16 bits
# http://www.barth-dev.de/online/rgb565-color-picker/
# RGB565 = (((RGB888&0xf80000)>>8) + ((RGB888&0xfc00)>>5) + ((RGB888&0xf8)>>3));

def pixels (im):
    i = 0
    j = 0
    hex_codes = str(im.shape[1]) + ',' + str(im.shape[0]) + ','
    hex_codes += '{'
    print("Dimensions: ", im.shape[1], 'x' , im.shape[0])
    while j < im.shape[1]:
        while(i < im.shape[0]):
            red = im[i][j][0]
            green = im[i][j][1]
            blue = im[i][j][2]
            hex_codes += ('0x%02x%02x%02x' % (red, green, blue))
            hex_codes += ', '
            i += 1
        j += 1
        i = 0

    return hex_codes

im = cv2.imread(filename)
string = pixels (im)
print(len(string))
print(string)

with io.open('b1_walk1.txt', "w", encoding="utf-8") as f:
    f.write(string)
f.close()
