#include "term.h"
char buffer[10240] = {0};
int state = 0;

int main()
{
    socketClass mySocket;
    handleCommands myCommands;
    
    mySocket.socket_init(8080);
    mySocket.socket_binding();
    mySocket.socket_listening();
    mySocket.socket_AcceptCall();
    while (true)
    {
        myCommands.handleBuffer(&mySocket);
        state = myCommands.command();
        mySocket.socket_send();
        if (state == 0)
        {
            break;
        }
    }
    mySocket.socket_closing();
    return 0;
}