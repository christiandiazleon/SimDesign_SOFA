#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/Node.h>
#include <zmq.hpp>
#include <zmq.h>
#include "zhelpers.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <stdint.h>
#include <random>
#include "ZMQClientComponent.h"
//#include "SofaTypeMessages.h"
using namespace std;

namespace sofa
{

namespace component
{

namespace controller
{

// Create a context to use in all sockets creation
zmq::context_t context(1);

/** Socket to subscribe this SOFA client to Network Manager server.
 * Although in first instance this SOFA client will be have a sender behavior
 * and not will receive messages
 **/
zmq::socket_t subscriber(context, ZMQ_SUB);


/**
 * Socket to send message, which will be taked by Network Manager and delivered to
 * all peer clients connected
 **/
zmq::socket_t sender(context, ZMQ_PUSH);

msgPointsGroupV3 msg;
BodyPointsToBeSent bp;

ZMQClientComponent::ZMQClientComponent()
// : myparam(initData(&myparam, 0.42, "myparam", "ZeroMq version plugin. "))
// : myparam(initData(&myparam, (double)(0.42), "myparam", "ZeroMq version plugin. "))
// , d_address(initData(&d_address, (std::string)"127.0.0.1", "address", "Scale for object display. (default=localhost)"))
{

}

void ZMQClientComponent::setupConnection()
{
    /**publisherEndpoint is the endpoint through the clients will be connect to
     * Network Manager and will receive data driven by SOFA events
     */
    const string publisherEndpoint = "tcp://localhost:5557";
    //const string publisherEndpoint = "tcp://ec2-34-239-214-140.compute-1.amazonaws.com:5557";

    /**pushEndpoint is the enpoint through clients send SOFA data events to
     * Network Manager and this in turn redirects them to all connected clients
     * via publisherEndpoint socket
    */
    //const string pushEndpoint = "tcp://ec2-34-239-214-140.compute-1.amazonaws.com:5558";
    const string pushEndpoint = "tcp://localhost:5558";

    /**
     * Connecting to publisherEndpoint and pushEndpoint
    */
    cout << "Connecting to ZMQ Network Manager   " << publisherEndpoint << "..." << endl;
    // subscriber.connect("tcp://ec2-34-239-214-140.compute-1.amazonaws.com:5557");
    subscriber.connect("tcp://localhost:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    /**
     * Connecting to publisherEndpoint and pushEndpoint
    */
    // sender.connect("tcp://ec2-34-239-214-140.compute-1.amazonaws.com:5558");
    sender.connect("tcp://localhost:5558");

    // Set the identity http://api.zeromq.org/4-2:zmq-setsockopt
    //client.setsockopt(ZMQ_IDENTITY, "PEER1", 5);

}

void ZMQClientComponent::hapkitDataSend()
{
    
    // A traves del objeto SerialDriver, accedemos a la posicion del instrumento con getPositionInstrument
    // el cual me retorna la posicion del instrumento. La almaceno en a
    float a = objectsSerialDriver[0]->getPositionInstrument();

    std::cout << "datos del hapkit " << a << std::endl;
    hapkitValue = to_string(a);

    // strcat(myStringHapkitValue, " ");

    s_send(sender, hapkitValue);
    cout << "Datos enviados" << endl;
}


void ZMQClientComponent::instrumentDataSend(instrumentData a)
{
    std::cout << "enviando datos de instrument: " << endl;

    a.pos = sofa::defaulttype::Vec3d(2.0f, 1.0f, 1.0f);
    a.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    a.btnState = 45;
    a.openInst = 1.0f;
    a.blnDataReady = false;

    /* Strings to store instrumentData members */
    string posVector0, posVector1, posVector2, allPosVector,
        quatQuat0, quatQuat1, quatQuat2, quatQuat3, allQuatQuat,
        btnStateStr, openInstStr, blnDataReadyStr, allInstrumentData;

    // ******* Turn to strings Vec3d pos contents*********
    posVector0 = to_string(a.pos[0]);
    posVector1 = to_string(a.pos[1]);
    posVector2 = to_string(a.pos[2]);

    // ******* Turn to strings Quat quat contents*********
    quatQuat0 = to_string(a.quat[0]);
    quatQuat1 = to_string(a.quat[1]);
    quatQuat2 = to_string(a.quat[2]);
    quatQuat3 = to_string(a.quat[3]);

    /* Other instrumentData */
    btnStateStr = " btsState value : " + to_string(a.btnState);
    openInstStr = " openInst value : " + to_string(a.openInst);
    blnDataReadyStr = " blnDataReady value : " +  to_string(a.blnDataReady);

    /* Concatenating all posVector elements */
    allPosVector = " -> posVector3D elements: " + posVector0 + " " + posVector1 + " " + posVector2 + " | ";

    /* Concatenating all quatQuat elements */
    allQuatQuat = " -> quatQuat elements: " + quatQuat0 + " " + quatQuat1 + " " + quatQuat2 + " " + quatQuat3 + " | ";

    /* Grouping instrumentDataStr */
    allInstrumentData = allPosVector + " " + allQuatQuat + " " + btnStateStr + " " + openInstStr + " "
    + blnDataReadyStr;

    // sleep(1);
    s_send(sender, allInstrumentData);

    /**
     * Send message from ZMQ_DEALER
     *ZMQ_SNDMORE is multipart message. ---
    */
    //zmq::message_t message;
    //client.send(message);
    //s_sendmore(client, "");

    //  Wait for next request from client

    /*
    while(1){
        // std::string string = s_recv(subscriber);

        // std::cout << "Received request: " << string << std::endl;

        // Do some 'work'
        sleep(1);
        s_send(sender, allInstrumentData);
    }
    */

}


void ZMQClientComponent::attachingDataToSend(attachingData b)
{

    string vIdTrianglesStr0;

    std::cout << "enviando datos de attaching: " << endl;

    b.vIdTriangles = {888, 2, 3, 4};
    int total = 0;
    total = b.vIdTriangles[0] + b.vIdTriangles[1] + b.vIdTriangles[2] + b.vIdTriangles[3];
    // cout << "The first element of vIdTriangles is: " << b.vIdTriangles[0] << endl;
    // cout << "The total of elements is: " << total << endl;
    vIdTrianglesStr0 = to_string(b.vIdTriangles[0]);


    // Sending data with ZMQ

    //zmq::message_t request(vIdTrianglesStr0.size());

    // tener en cuenta esto http://zguide.zeromq.org/page:all#A-Minor-Note-on-Strings

    //std::copy_n(vIdTrianglesStr0.c_str(), vIdTrianglesStr0.size() + 1, reinterpret_cast<char *>(request.data()));
    //socket.send(request);

    s_send(sender, vIdTrianglesStr0);


}

BodyPointsToBeSent::BodyPointsToBeSent()
{

}

void BodyPointsToBeSent::ProcessPtsToBeSent()
{
    int sizeTempTotal = 50;
    int contador = 0;

    string deformationVectorK_X;

    /* data en el llenado del vector PointNet, va a iterar por el valor de sizeTempTotal
    en cada eje x,y,z por eso sizeTempTotal es 50 para que de 150  */
    float data[150];
    vector<PointNet> vPoints;
    vPoints.resize(sizeTempTotal);
    int NbPointsMsg = 20;
    // startPoint = (int)data[0];

    /* Llenamos el array de data */
    for (unsigned int i = 0; i < 150; i++)
    {
        data[i] = (float)i;
    }

    int cont = 0;
    
    /* Llenamos el vector del total de puntos del organo vPoints 
    basandonos en que no pasara de 50, por lo que ponemos sizeTempTotal 
    como variable de control. Se incrementa en tres porque se va a llenar
    el vector vPoints que es de tipo PointNet que tiene x,y,z en cada posicion  */
    for (unsigned int i = 0; i <= sizeTempTotal; i += 3)
    {
        // Llenando el vector que contiene todos los puntos del organo
        /* Creamos pTemp de tipo PointNet y a este le asignaremos los valores de data
        por tres veces sizeTemp (que tiene tamaño 50), asi que completara su tamaño 150 */
        PointNet pTemp(data[i], data[i + 1], data[i + 2]);
        // vNroPointC.push_back((int)data[i]);

        /* Asignamos pTemp a vPoints que es en donde estarán todos los puntos de deformacion*/
        vPoints[i] = pTemp;
        
        // std::cout << "Guardando " << i << "punto por " << i << "ésima ocasión" << std::endl;
        /* Guardamos cada elemento en la posicion cont de vPoints para cada x,y,z que posee en su estructura como
        vector tipo PointNet*/
        std::cout << vPoints[cont].getX() << " " << vPoints[cont].getY() << " " << vPoints[cont].getZ() << std::endl;
        std::cout << vPoints.size() << std::endl;
        // std::cout << "Vector de puntos tiene" << vPoints << " \n\n" << std::endl;
        cont++;
    }
    /*De esta manera nuestro vector vPoint que es el que contendra el numero tota de puntos de un
    organo, ya esta lleno
    Realmente, este vector vPoints lo recibire de SOFA, por lo que aca en este metodo
    ProcessPtsToBeSent deberia es tener un metodo set vPoints que pueda ser usado desde SOFA y
    mediante el se asigne ese vector que me pasan a vPoints y ahi ya empezar a construir el mensaje*/

    //msgPointsGroupV3 msgTemp;
    msg.setNroTotalPoints((int)vPoints.size());
    // std::cout << "Hola, mi vector de puntos tiene" << vPoints.size() << " \n\n"
    //           << endl;


    /* Definimos unas variables temporales */
    
    /* sizeTemp (20) para tenerla como control para el maximo d enumero de puntos por mensaje  */
    int sizeTemp;

    /* vPointsTemp que ira solo hasta 20 y en el almacenaremos el contenido de vPoints que
    tendra 50 elementos, aca se parte  */
    vector<PointNet> vPointsTemp; 

    /* Mientras contador sea menor que mi vector de puntos vPoints (50) */
    while(contador < vPoints.size())
    {
        if (contador == 0)
        {
            /* Definimos que inicia el mensaje para el envio de datos de deformación*/
            msg.setIDTypeMsgDef(1); // Start message
            
            /* Definimos un tamaño temporal del mensaje igual al numero de puntos por mensaje NbPointsMsg*/
            sizeTemp = NbPointsMsg;
            /*Inicializamos el vector vPointsTemp a 20 que es el numero maximo de puntos por mensaje.  */
            vPointsTemp.resize(sizeTemp);

            /*  START message - Generando el mensaje inicial de 20 con sizeTemp como variable de control */
            for (unsigned int k = 0; k < sizeTemp; k++)
            {
                /* vPointsTemp que ira solo hasta 20 y en el almacenaremos el contenido de vPoints que
                    tendra 50 elementos, aca se parte  */
                vPointsTemp[k] = vPoints[k];
            }
        }
        /**
         * FINISH 
         * Para el mensaje final de que se terminaron de llenar la totalidad de puntos en vPoints
         * Si contador +NbPointsMsg es igual o mayor que el total de puntos a enviar de mi vector vPoints 
        */
        else if((contador+NbPointsMsg) >= vPoints.size())
        {
            /* tamaño temporal sizeTemp del mensaje = tamaño del vector totalpuntos vPoints - contador  */
            sizeTemp = (vPoints.size() - contador);

            /* Se inicializa vPointsTemp al tamaño que da sizeTemp, que seria lo que resta de enviar de puntos
            que seria el mensaje final*/
            vPointsTemp.resize(sizeTemp);

            /**
             * Llenamos el vector con los ultimos puntos que faltan por ser enviados en el mensaje
             * Como son los ultimos puntos que faltan por enviar, k siempre va a ser menor que vPoints(50)
             * */
                for (unsigned int k = contador; k < vPoints.size(); k++)
            {
                vPointsTemp[k - contador] = vPoints[k];
                // std::cout << "Finalizando de empaquetar puntos" << endl;

                deformationVectorK_X = std::to_string(vPoints[k].getX());
                std::cout << "Valor de X en string" << deformationVectorK_X << endl;
                // s_send(sender, deformationVectorK_X);
            }
        }
        else
        /** Mensajes de abordamiento de puntos intermediarios - MIDDLE
         * cuando contador es mayor a cero pero no mayor que el tamaño de puntos totales a enviar
        */
        {
            /*Ponemos sizeTemp igual a maximo de puntos por mensaje NbPointsMsg */
            sizeTemp = NbPointsMsg;

            /*Inicializamos la variable temporal vPointsTemp al tamaño de maximo numero de puntos por mensaje */
            vPointsTemp.resize(sizeTemp);

            /*Llenamos el vector vPoints con los mensajes intermediarios */
            for (unsigned int k = contador; k < (contador + NbPointsMsg); k++)
            {
                vPointsTemp[k - contador] = vPoints[k];
            }
        }

        /*incrementamos lo que tiene contador mas NbPointsMsg */
        contador+=NbPointsMsg;
    }
}


void ZMQClientComponent::init()
{
    //fun();

    /* We get the rootContext */
    sofa::simulation::Node::SPtr rootContext = static_cast<simulation::Node *>(this->getContext()->getRootContext());
    cout << "rootContext: " << rootContext << endl;
 
    //este era. Buscamos un objeto Serial Driver.
    getContext()->get<SerialDriverType>(&objectsSerialDriver, core::objectmodel::BaseContext::SearchRoot);

    //rootContext->getTreeObjects<SerialDriverType>(&objectsSerialDriver);
    if(objectsSerialDriver.size() == 0) {
        std::cout << "Oe" << " \n\n"
                << endl;
    }
    else
    {
    
        std::cout << "Hola este es el tamaño de mi vector Serial Driver:" << objectsSerialDriver.size() << " \n\n"
                << endl;
        std::cout << "Este es el elemento 0 de mi vector Serial Driver: " << objectsSerialDriver[0]->name.getValue() << " \n\n"
                << endl;
        std::cout << "Este es la posicion del hapkit: " << objectsSerialDriver[0]->getPositionInstrument() << " \n\n"
                << endl;
        
        //SerialDriver* s = new SerialDriver();

        std::cout << "ZeroMQCommunication::init()" << std::endl;
        ZMQClientComponent z;
        //msgPointsGroupV3 msg;
    

        // Connecting to Nerwork Manager
        z.setupConnection();
        msg.test();
    
    }

    // Creating instrument data object
    // instrumentData itemp;

    // Creating attaching data object
    // attachingData n;



    /*
    ***** Envio de datos de instrumento y de attaching usando las estructuras***
    while (1)
    {
        // Sending data objects structure members
        z.instrumentDataSend(itemp);
        z.attachingDataToSend(n);
        // z.getResponseFromServer();
        // z.draw();
    }
    */
}

/*ZMQClientComponent::~ZMQClientComponent()
{
}*/

void ZMQClientComponent::draw(const core::visual::VisualParams *vparam)
{
    // std::cout << "Draw del ZMQ parameters" << std::endl;

    hapkitDataSend();
    msg.SetupReceive();
    // bp.ProcessPtsToBeSent();


    //float a = s->askDevice();
    //float a = s->getTraslValue();

    /*
    float a = objectsSerialDriver[0]->getPositionInstrument();

    std::cout << "datos del hapkit " << a << std::endl;
    hapkitValue = to_string(a);
    s_send(sender, hapkitValue);
    cout << "Datos enviados" << endl;
    */
}

void ZMQClientComponent::draw()
{
    std::cout
        << "Draw del ZMQ solo" << std::endl;

    /*
    instrumentData itemp;
    itemp.pos = sofa::defaulttype::Vec3d(1.0f, 1.0f, 1.0f);
    itemp.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    itemp.btnState = 5671;
    itemp.openInst = 1.0f;
    itemp.blnDataReady = true;

    cout << itemp.btnState;
    */
    // SofaTypeMessages a;
    // a.instrumentDataSend(itemp);

}



// int ZeroMqComponentClass = sofa::core::RegisterObject("This component does nothing.").add<ZeroMqComponent>();
SOFA_DECL_CLASS(ZMQClientComponent)

int ZMQClientComponentClass = sofa::core::RegisterObject("This component connect to ZMQ Socket.").add< ZMQClientComponent >();
} // namespace controller

} // namespace component

} // namespace sofa
