#!/usr/bin/python
# coding=utf-8

import smtplib
 
SMTP_SERVER = 'smtp.gmail.com'
SMTP_PORT = 587
 
sender = 'ailove.box@gmail.com'
password = "bynfq9vz"
recipient = 'trigger@ifttt.com'
subject = u"Проверка @gasubasu #hashtags".encode("utf-8")
body = u"тело письма".encode("utf-8")

 
body = "" + body + ""
 
headers = ["From: " + sender,
           "Subject: " + subject,
           "To: " + recipient,
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
