import zmq
context = zmq.Context()
frontend = context.socket(zmq.ROUTER)
frontend.bind("tcp://*:5559")

# Initialize a poll set
poller = zmq.Poller()
poller.register(frontend, zmq.POLLIN)

print("Creating Server Network Manager Router")

while True:
    socks = dict(poller.poll())

    if socks.get(frontend) == zmq.POLLIN:
        message = frontend.recv_multipart()
        print(message)
        frontend.send_multipart(message)

    
        
# Aqui debo usar el ROUTER-DEALER para ver como envio cosas al cliente
# http://zguide.zeromq.org/py:rtdealer
# 
# 
#  

