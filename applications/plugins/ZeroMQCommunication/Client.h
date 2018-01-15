#include <sys/time.h>
#include <iostream>

using namespace std;

class Client
{
  public:
    Client();
    void setupConnection();
    void GetResponseFromServer();

  private:
    struct timeval t_before, t_after;
    string replyMessage;
};