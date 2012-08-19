#!/usr/bin/python
# coding=utf-8                                                  
import math

def sin_a(res):
	sin="{"
	for i in range(0,res):
		sin+=str(int(round(8+8*math.cos(i/float(res)*3.14159265*2))))+", "
		if(i%16==0): sin+="\n"
	sin+="}"
	return sin

print sin_a(255)

                   
