# Communication Test between C++ and Python process

**zeroMQ**

[zeromq](http://zguide.zeromq.org/page:all) act like a asyncronous messaging framework
to distributed and concurrent applications. Work with queing messages


Tipically the applications messages communication need of a middleware entity which translate
a message from a formal messaging protocol which handle the sender, to another protocol that will be handle
by the  receiver.

Is of this way, the kind of manage the communication to formal exchange of defined messages


>A zeroMQ system can running without that middleware entity or broker message



**zeromq** have sockets, each of which represents  one connection many to many between endpoints.

# zeroMQ Messaging patterns

zeromq work with the following  [messaging patterns](http://zguide.zeromq.org/page:all#Messaging-Patterns)


+ **Request - Reply**
 
 Connects a set of clients to a set of services
 
 + **Publish - Subscribe**
 
 Connects a set of publishers to a set of subscribers
 
 + **Push - Pull** (Pipeline)
 
 Connects nodes in a fan-out/fan-in pattern that can have multiple steps and loops. This is a parallel task distribution and collection pattern.
 
 + **Exclusive pair**
 
 Connects two sockets exclusively. This is a pattern for connecting two threads in a process, not to be confused with "normal" pairs of sockets.
 
 
 
 In zeroMQ each pattern define a particular network topology. The messages can be carried via TCP, Multicast
 IPC, support multiple languages and platforms and allowing any architecture (centralized, distributed, small, big.)
 
 Is created to treat scalability problems allowing multiple connections between sockets with the messaging patterns
 previously mentioned
 

 # Setup the C++ client
 
 + **Download the packages** (Ubuntu)
 
 `sudo apt-get install libtool pkg-config build-essential autoconf automake`
 
 + **Setup `libsodium`** library 
 
 ```
 wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz
 
 tar -zxvf LATEST.tar.gz
 
 cd libsodium-stable
 
 sudo ./configure
 
 sudo make
 
 sudo make install
 
 sudo ldconfig
  ```
  
 + **Setup the zeroMQ package**
 
 ```
 wget https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.tar.gz
 
 tar -zxvf zeromq-4.2.2.tar.gz 
 
 cd zeromq-4.2.2/
 
 sudo ./configure
 
 sudo make
 
 sudo make install
 
 sudo ldconfig 
 ```
 
 + **Using zeroMQ binding to C++ implementation**
 
  ```
  cd /usr/include/
  
  sudo wget https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp
  ```
  
  Of this way, we using the `#include <zmq.hpp>` similar to headers in the C++ client
  
  
  + **Checking the right installation of together packages**
  
   ```
   cd /usr/local/lib 
   
   ls
   ```
   
   We should look `libsodium.so` y `libzmq.so` files in this directory
   
   
   
In addition, is necessary have installed the following packages:

* gcc
* Cmake
* Pyzmq --- For the pyhon server ---  `pip install pyzmq`


# Running Client and Server scripts

+ Running the Python Server

```
cd zeroMQ
cd Python 
python server.py
```

+ Running the C++ client

Is necessary build and after execute the binary file.

```
g++ Client.cpp main.cpp -o client.out -lzmq

./client.out
```


