#include "term.h"
extern char buffer[1024];
process_state_t socketClass::socket_init(int portNum)
{
    process_state_t ret = FAILED;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNum);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (server_socket < 0)
    {
        cout << "Can;t Create a socket";
        ret = FAILED;
    }
    else
    {
        cout << "\n=> Socket server has been created..." << endl;
        ret = SUCESS;
    }
    return ret;
}
process_state_t socketClass::socket_binding()
{
    process_state_t ret = FAILED;
    int check = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (check < 0)
    {
        std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
        ret = FAILED;
    }
    else
    {
        ret = SUCESS;
    }
    return ret;
}
process_state_t socketClass::socket_listening()
{
    process_state_t ret = FAILED;
    int check = listen(server_socket, 5);
    if (check < 0)
    {
        std::cout << "Cant listen";
        ret = FAILED;
    }
    else
    {
        std::cout << "Server is listening on port 8080..." << std::endl;
        ret = SUCESS;
    }
    return ret;
}
process_state_t socketClass::socket_AcceptCall()
{
    process_state_t ret = FAILED;
    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
    if (client_socket < 0)
    {
        cout << "Problem with client listening";
        ret = FAILED;
    }
    else
    {
        ret = SUCESS;
    }
    return ret;
}

process_state_t socketClass::socket_closing()
{
    process_state_t ret = FAILED;

    close(client_socket);
    int check = close(server_socket);

    if (check < 0)
    {
        cout << "Problem with client listening";
        ret = FAILED;
    }
    else
    {
        ret = SUCESS;
    }
    return ret;
}
void socketClass::socket_send()
{
    send(client_socket, buffer, strlen(buffer), 0);
}

process_state_t handleCommands::handleBuffer(socketClass *socketClass_t)
{
    process_state_t ret = FAILED;
    memset(buffer, 0, 1024);
    recv(socketClass_t->client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Received: " << buffer << std::endl;

    return ret;
}
int handleCommands::command()
{

    if (memcmp(buffer, "bat", 3) == 0)
    {
        memset(buffer, 0, 1024);
        string bufferstring = exec("acpi");
        strcpy(buffer, bufferstring.c_str());
    }

    if (memcmp(buffer, "fire", 4) == 0)
    {
        memset(buffer, 0, 1024);
        string bufferstring = exec("firefox");
        strcpy(buffer, bufferstring.c_str());
    }
    if (memcmp(buffer, "end", 3) == 0)
    {
        return 0;
    }
}
string handleCommands::exec(string command)
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
