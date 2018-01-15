#
#   Hello World client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "Hola servidor C++" to server, expects "Hola cliente python" back
#

import zmq

context = zmq.Context()

#  Socket to talk to server
print("Connecting to ZMQ server…")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

#  Do 10 requests, waiting each time for a response
#for request in range(10):
#print("Sending request %s …" % request)
socket.send(b" Hello ZMQ Server, What is your version")

#  Get the reply.
message = socket.recv()
print(message)
print("Received reply %s" % message)
