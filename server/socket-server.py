import socket

HOST = '10.0.0.9'                 # Symbolic name meaning all available interfaces
PORT = 3333               # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print "SOCKET SERVER STARTED"
print str(HOST)+":"+str(PORT)
try:
	while 1:
		conn, addr = s.accept()
		print 'Connected by', addr
		data = conn.recv(1024)
		if not data: 
			continue
		print data
		conn.close()
except:
	conn.close()

# -- End of python script
