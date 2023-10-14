#include <functional>
#include <iostream>
#include <string.h>
#include <string> //for xc
// To use Socket
#include <sys/types.h>
#include <sys/socket.h>
// To use the function getnameinfo()
// getnameinfo  - address-to-name translation in protocol-independent manner
#include <netdb.h>
#include <unistd.h>
//
#include <arpa/inet.h>

using namespace std;

int main()
{
    char ip_[] = "0.0.0.0";
    struct sockaddr_in hint; /*sockaddr_in : Structure describing an Internet socket address.*/
    socklen_t size;

    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0)
    {
        cout << "Can;t Create a socket";
        return -1;
    }
    cout << "\n=> Socket server has been created..." << endl;

    // Bind Socket to ip port

    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = htons(INADDR_ANY);
    hint.sin_port = htons(1500); // Host to network short "htons" its reverse fun is ntoh
    // any time we deal with a port number we should pass this number by htons

    /* Convert from presentation format of an Internet number in buffer
       starting     at CP to the binary network format and store result for
       interface type AF in buffer starting at BUF.  */
    // inet_pton(AF_INET, ip_, &hint.sin_addr); // 127.0.0.1

    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) < 0)
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
         return -2;
    }

    // Mark the socket to listening in

    if (listen(listening, SOMAXCONN) == -1)
    {
        cout << "Cant listen";
        return -3;
    }
    //  Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (struct sockaddr *)&client, &clientSize);
    if (clientSocket < 0)
    {
        cout << "Problem with client listening";
        //    return -4;
    }

    // Close the listening socket
    // close(listening);

    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
    // Close the listening socket
    close(listening);

    // While recieb=ving display the messsage
    char buf[4096];
    while (true)
    {
        // clear buffer
        memset(buf, 0, 4096);
        // wait for message
        int byteRec = recv(clientSocket, buf, 4096, 0);
        if (byteRec == -1)
        {
            cout << " There was connection issue " << endl;
            break;
        }
        if (byteRec == 0)
        {
            cout << " The client disconnected " << endl;
            break;
        }
        // Display message
        cout << " Recieved " << string(buf, 0, byteRec) << endl;
        // resend message
        send(clientSocket, buf, 4096, 0);
    }
    cout << "\n\n=> Connection terminated with IP " << inet_ntoa(hint.sin_addr);

    // close the socket
    close(clientSocket); // close socket
    return 0;
}