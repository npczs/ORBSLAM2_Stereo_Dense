# -*- coding: UTF-8 -*-
# 1. 导入需要的包和模块
import cv2
import numpy as np
import os
import time
 
 
# 2. 读取视频，获取视频的帧率、宽度和高度三个参数
cap = cv2.VideoCapture('left.avi')
 
# isOpened() -> retval
# .   @brief Returns true if video capturing has been initialized already.
isOpened = cap.isOpened()
 
fps = cap.get(cv2.CAP_PROP_FPS)  # 帧率<每秒中展示多少张图片>
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)) # 获取宽度
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))  # 获取高度
print(fps, width, height)
# 10.0 996 608
 
 
# 3. 定义转换函数，当视频打开时进行分解操作并重命名文件保存
def changeToPics():
    i = 0
    while(isOpened):
         
        # read([, image]) -> retval, image
        # .   @brief Grabs, decodes and returns the next video frame.

	#f=open("./time.txt","a+")#打印时间戳
	#time1=time.time()
	#f.write(str(time1))
	#f.write("\n")
        (flag, frame) = cap.read()  # 读取每一张 flag<读取是否成功> frame<内容>
        filename = 'image' + str(i) + '.jpg'
        if flag == True:  #读取成功的话
            cv2.imwrite(r'./results' + os.sep + filename,frame,[cv2.IMWRITE_JPEG_QUALITY,100])
            #写入文件，1 文件名 2 文件内容 3 质量设置
        else:
            break
	i = i + 1
    print("convert successfully!!!")
 
 
# 4. 调用函数
changeToPics()
