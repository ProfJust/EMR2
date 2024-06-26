#!/usr/bin/env python3
# ################################################################################
# edited WHS, OJ , 7.1.2022 #
# sudo apt-get install python3-opencv

import cv2
print(cv2.__version__)

# lese Bild von Festplatte
image = cv2.imread('/home/oju/ur3_ws/src/emr2/scripts/test.png')

# zeige Bild in Fenster an
cv2.imshow("Bild", image)

# warte auf Tastendruck (wichtig, sonst sieht man das Fenster nicht)
# Fenster vorher mit Maus aktivieren!!
cv2.waitKey(0)


# Position y, x
y = 100
x = 50

# waehle eine Region of Interest an Punkt:
# (y, x) mit Dimension 50x50 Pixel
region_of_interest = image[y:y+50, x:x+50]

# zeige Region of Interest an
cv2.imshow("ROI", region_of_interest)

# warte auf Tastendruck (wichtig, sonst sieht man das Fenster nicht)
cv2.waitKey(0)

# setze ROI auf Gruen
region_of_interest[:, :] = (0, 255, 0)

# die ROI ist ein "Zeiger" auf das urspruenglich geladene Image.
# Es enthaelt nun eine gruene Box!
cv2.imshow("Bild modifiziert", image)

# warte auf Tastendruck
cv2.waitKey(0)