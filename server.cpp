#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main()
{
    int connexionClient, connexionServer;
    int client[2], server[2], i;
    int portNum = 1500;
    int nextPort = 1600;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char message[277]; //Pseudo 21 + Message 256
    int clientCount = 1;

    struct sockaddr_in server_addr_connexion, server_addr0;


    socklen_t size;

    connexionClient = socket(AF_INET, SOCK_STREAM, 0);

    if (connexionClient < 0) 
    {
        cout << "\nErreur Création Socket ..." << endl;
        exit(1);
    }

    cout << "\n### Socket Client Créé ..." << endl;

    server_addr_connexion.sin_family = AF_INET;
    server_addr_connexion.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr_connexion.sin_port = htons(portNum);

    if ((bind(connexionClient, (struct sockaddr*)&server_addr_connexion,sizeof(server_addr_connexion))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    size = sizeof(server_addr_connexion);
    cout << "=> Looking for clients..." << endl;
    listen(connexionClient, 1);

    connexionServer = accept(connexionClient,(struct sockaddr *)&server_addr_connexion,&size);
    if (connexionServer < 0) 
        cout << "=> Error on accepting..." << endl;

    buffer[0]='\0';
    sprintf(buffer,"%d", nextPort);
    send(connexionServer, buffer, bufsize, 0);





    client[0] = socket(AF_INET, SOCK_STREAM, 0);

    server_addr0.sin_family = AF_INET;
    server_addr0.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr0.sin_port = htons(nextPort);

    if ((bind(client[0], (struct sockaddr*)&server_addr0,sizeof(server_addr0))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    size = sizeof(server_addr0);
    cout << "=> Looking for clients..." << endl;
    listen(client[0], 1);

    server[0] = accept(client[0],(struct sockaddr *)&server_addr0,&size);
    if (server[0] < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server[0] > 0) 
    {
        do {
            recv(server[0], buffer, bufsize, 0);
            message[0] = '\0';
            strcat(message, buffer);
            for(i = 0; i < 255; i++){
                if(message[i] == '\n'){
                    message[i] = '\0';
                    break;
                }

            }
            
            if(strcmp(message,"/exit") == 0){
                cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr0.sin_addr);
                close(server[0]);
            }
        } while (*buffer == '\n');
        cout << message << endl;

    }

    
    close(connexionClient);
    return 0;
}