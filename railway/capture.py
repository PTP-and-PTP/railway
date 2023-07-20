#!/usr/bin/env python

import cv2
import os

number = 0
device = 0

def save_camera_frame(
        index,
        device = 0,
        dir_path = 'captures',
        basename = 'image',
        ext = 'jpg',
        ):
    global number
    camera = cv2.VideoCapture(device)
    if not camera.isOpened():
        print("No webcam is detected.")
        return

    os.makedirs(dir_path, exist_ok=True)
    base_path = os.path.join(dir_path, basename)

    # caputre
    image = '{}_{}.{}'.format(base_path, number, ext)
    ret, frame = camera.read()
    cv2.imwrite(image, frame)
    number = number + 1
    
    camera.release()
    print(image)
    return image
