#include "EchoServer.h"

class Socekt;
class InetAddress;
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return -1;
    }

    EchoServer echoserv(argv[1], atoi(argv[2]));
    echoserv.Start();
    return 0;
}