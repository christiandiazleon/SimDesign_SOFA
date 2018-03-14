#include <zmq.hpp>
#include "zhelpers.hpp"
using namespace std;

int main(int argc, char *argv[])
{

    zmq::context_t context(1);
    zmq::socket_t client(context, ZMQ_DEALER);

    const string endpoint = "tcp://localhost:5559";

    client.setsockopt(ZMQ_IDENTITY, "PEER1", 5);
    cout << "Connecting to ZMQ Network Manager " << endpoint << "..." << endl;
    client.connect(endpoint);
    for (int request = 0; request < 10; request++)
    {

        s_sendmore(client, "");
        s_send(client, "Testing sending some data");

        std::string string = s_recv(client);

        std::cout << "Received reply " << request
                  << " [" << string << "]" << std::endl;
    }
}