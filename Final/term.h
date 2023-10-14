#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;


string exec(string command);
typedef enum process_state
{
    FAILED,
    SUCESS

} process_state_t;

class socketClass
{

public:
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_length;

    //socketClass();

    process_state_t socket_init(int porttNum);
    process_state_t socket_binding();
    process_state_t socket_listening();
    process_state_t socket_AcceptCall();
    void socket_send();

    process_state_t socket_closing();

    //~socketClass();
};

class handleCommands
{
public:
    process_state_t handleBuffer(socketClass *socketClass_t);
    int command();
    string exec(string command);
};
