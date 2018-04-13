import random
import time

import zmq

 
def main():
    ctx = zmq.Context()
    subscriber = ctx.socket(zmq.SUB)
    subscriber.setsockopt_string(zmq.SUBSCRIBE, '')
    subscriber.connect("tcp://localhost:5557")

    publisher = ctx.socket(zmq.PUSH)
    publisher.connect("tcp://localhost:5558")

    random.seed(time.time())
    while True:
        """
        Wait 100 ms for an event in the suscriber socket
        Listen to PUB server socket around of 100 ms
        """
        if subscriber.poll(100) & zmq.POLLIN:
            message = subscriber.recv()
            print("I: received message %s" % message)
        
        message = "Soy el cliente python 1"
        publisher.send_string("%s" % message)

        """
        else:
            """

        """
        rand = random.randint(1,100) 
        if rand < 10:
            publisher.send_string("%d" % rand)
            print ("I: sending message %d" % rand)
        """


if __name__ == '__main__':
    main()
