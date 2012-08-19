#!/usr/bin/python
# coding=utf-8

import bottle
from bottle import route, post, run, request
import os,random
import time
import smtplib


bottle.debug(True)
randomMessages = ["space1.txt","space2.txt","space3.txt","ufo.txt", "satellites.txt", "constants.txt", "weather.txt", "metro.txt", "ailove1.txt", "ailove2.txt"]

def sendEmail(subject, body):

    SMTP_SERVER = 'smtp.gmail.com'
    SMTP_PORT = 587
     
    sender    = "ailove.box@gmail.com"
    password  = "bynfq9vz"
    recipient = "trigger@ifttt.com"
     
    body = "" + body + ""
     
    headers = ["From: "    + sender, "To: "      + recipient,
               "Subject: " + subject,

               "MIME-Version: 1.0",
               "Content-Type: text/html"]
    headers = "\r\n".join(headers)
     
    session = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
     
    session.ehlo()
    session.starttls()
    session.ehlo
    session.login(sender, password)
     
    session.sendmail(sender, recipient, headers + "\r\n\r\n" + body)
    session.quit()

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

def sendSMS(phone, message):
    print ("Send SMS to: "+phone+"\n["+message+"]")
    f = open("../skypekit/bin/sms/"+str(time.time())+".txt", "w+")
    f.write(phone+"\n"+message)
    f.close()

@route('/')
def home():
    try:
        message=cutRandomLineFromFile("congratulations.txt")
    except Exception, e:
        message=getRandomLineFromFile(random.choice(randomMessages))
    #print message;
    #sendEmail(message,"")
    sendSMS("+79253200641",message);
    return "OK"
run(host='10.0.0.9', port=8515, reloader=True)
