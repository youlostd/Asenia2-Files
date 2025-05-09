# -*- coding: utf-8 -*-
import os,shutil

def hasNumbers(inputString):
	return (char.isdigit() for char in inputString)

crcList	= { }

for line in open('../root/property.xml'):
	if "crc " in line:
		x1 = line.split("crc = \"")
		x2 = x1[1].split("\"")
		y1 = line.split("filename = \"")
		y2 = y1[1].split("\"")
		crcList.update({x2[0]:[0, y2[0]]})
		
for root, dirs, files in os.walk('../'):
	for file in files:
		if str(file) == "AreaData.txt":
			for line in open(root + "\\AreaData.txt"):
				line = line.replace("    ","").replace("\n","")
				if line.isdigit():
					bla = crcList[line]
					print bla
					crcList.update({line:[bla[0]+1,bla[1]]})

for arr in crcList:
	if arr[0] == 0:
		print arr[1]
		print shutil.copy("unused_obj/%i" % arr[0],arr[1])
	