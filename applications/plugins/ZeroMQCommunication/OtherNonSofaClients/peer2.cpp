#include <zmq.hpp>
#include "zhelpers.hpp"
using namespace std;

int main (int argc, char *argv[])
{

    zmq::context_t context(1);
    zmq::socket_t peer2(context, ZMQ_DEALER);

    const string endpoint = "tcp://localhost:5559";

    peer2.setsockopt(ZMQ_IDENTITY, "PEER2", 5);
    cout << "Connecting to ZMQ Network Manager " << endpoint << "..." << endl;
    peer2.connect(endpoint);
    //s_sendmore(peer2, "");
    //s_send(peer2, "Probando");

    //std::string string = s_recv(peer2);

    //std::cout << "Received reply " << " [" << string << "]" << std::endl;

        
    for (int request = 0; request < 10; request++)
    {

        //s_sendmore(peer2, "");
        s_send(peer2, "Probando");
        
        std::string string = s_recv(peer2);

        std::cout << "Received reply " << request
                  << " [" << string << "]" << std::endl;
    }
    
}
