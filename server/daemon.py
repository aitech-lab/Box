#!/usr/bin/python
# coding=utf-8

import bottle
from bottle import route, post, run, request
import os,random

bottle.debug(True)

randomMessages = ["space1.txt","space2.txt","space3.txt","ufo.txt"]

def cutRandomLineFromFile(filename):

    
    lines = open(filename,'r').readlines()
    i =  random.randint(0, len(lines)-1)
    line = lines[i]
    del lines[i]
    open(filename, 'w').writelines(lines)
    return line

def getRandomLineFromFile(filename):
    file = open(filename,'r')
    #Get the total file size
    file_size = os.stat(filename)[6]
    file.seek((file.tell()+random.randint(0,file_size-1))%file_size)

    #dont use the first readline since it may fall in the middle of a line
    file.readline()
    #this will return the next (complete) line from the file
    line = file.readline()

    #here is your random line in the file
    return line
    
@route('/')
def home():
    try:
        return cutRandomLineFromFile("congratulations.txt")
    except Exception, e:
        return getRandomLineFromFile(random.choice(randomMessages))

run(host='10.0.0.9', port=8515, reloader=True)
