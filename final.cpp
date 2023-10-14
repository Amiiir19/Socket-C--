#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

string exec(string command)
{
    char buffer[128];
    string result = "";
    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe))
    {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

int main()
{
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_length;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
    }

    listen(server_socket, 5);

    std::cout << "Server is listening on port 8080..." << std::endl;

    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
    /****************************************************************************/

    /******************************************************************************/

    char buffer[1024] = {0};
    while (true)
    {
        memset(buffer, 0, 1024);

        recv(client_socket, buffer, sizeof(buffer), 0);
        std::cout << "Received: " << buffer << std::endl;
        if (memcmp(buffer, "fire", 4) == 0)
        {

            system("firefox");

            // system("acpi");
            memset(buffer, 0, 1024);
            string bufferstring = exec("acpi");
             strcpy(buffer, bufferstring.c_str());

            // std::cout << "openinnng" << std::endl;
        }
        if (memcmp(buffer, "end", 3) == 0)
        {
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }
    close(client_socket);
    close(server_socket);

    return 0;
}