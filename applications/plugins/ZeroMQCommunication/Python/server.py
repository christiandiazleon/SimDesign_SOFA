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
print('ZMQ Server listening ... ')

while True:
    #  Wait for next request from client
    message = socket.recv()
    # instant = time()
   
    print("Received message from Sofa: {}".format(message))

    print("Splitting message")
    x = message.split()
    print (x)
    # print("Received message from c++ %s" % str(message))
    # print("Received message from C++: %s" % message,) 

    #print("Mensaje recibido desde C++: %s" % message, "in: %s" %
    #      datetime.datetime.now().microsecond)

    # print(hex(id(message)))
    # print(ctypes.addressof(message))
    #print('hi')

    #  Do some 'work'
    time.sleep(1)

    #  Send reply back to client
    socket.send(b"Hola cliente, muy bien y tu ?")
    # print('Response sent')
