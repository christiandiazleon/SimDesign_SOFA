#
#   Hello World server in Python
#   Binds REP socket to tcp://*:5555
#   Expects b"Hola Servidor Python" from client, replies with b"Aja cliente C++ como anda todo"
#

import time
import zmq
#import datetime


context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")
# socket.bind("tcp://*:5556")
#  socket.bind("tcp://*:5557")
print('ZMQ Server listening ... ')

while True:
    #  Wait for next request from client
    message = socket.recv()
    # instant = time()
   
    print("Received message from SOFA: {}".format(message))
    #print("Received message from SOFA: %s" % message)

    print("Splitting message")
    x = message.split()
    print (x)
    
    #  Do some 'work'
    time.sleep(1)

    #  Send reply back to client
    socket.send(b"Hola cliente, muy bien y tu ?")
    # print('Response sent')
