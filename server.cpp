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
    int client[2], server[2], i;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char message[277]; //Pseudo 21 + Message 256

    struct sockaddr_in server_addr1;
    struct sockaddr_in server_addr2;
    socklen_t size;

    client[0] = socket(AF_INET, SOCK_STREAM, 0);
    cout << client[0] << " ici ";

    client[1] = socket(AF_INET, SOCK_STREAM, 0);
    cout << client[1] << " ici ";

    if (client[0] < 0 || client[1] < 0) 
    {
        cout << "\nErreur Création Socket ..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;

    server_addr1.sin_family = AF_INET;
    server_addr1.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr1.sin_port = htons(portNum);

    server_addr2.sin_family = AF_INET;
    server_addr2.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr2.sin_port = htons(1600);

    if ((bind(client[0], (struct sockaddr*)&server_addr1,sizeof(server_addr1))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    if ((bind(client[1], (struct sockaddr*)&server_addr2,sizeof(server_addr2))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    cout << client[0] << " la ";

    size = sizeof(server_addr1);
    cout << "=> Looking for clients..." << endl;


    listen(client[0], 1);
    listen(client[1], 1);

    int clientCount = 1;
    server[0] = accept(client[0],(struct sockaddr *)&server_addr1,&size);
    cout << client[0] << " ici ";

    clientCount = 2;
    server[1] = accept(client[1],(struct sockaddr *)&server_addr2,&size);
    cout << client[1] << " ici ";

    // first check if it is valid or not
    if (server[0] < 0 || server[1] < 0) 
        cout << "=> Error on accepting..." << endl;

    int passe = 0;

    while (server[0] > 0) 
    {
        if(passe == 0){
            strcpy(buffer, "=> Server connected...\n");
            send(server[0], buffer, bufsize, 0);
            cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;

            send(server[1], buffer, bufsize, 0);
            cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;

            passe++;
        }
        
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
                cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr1.sin_addr);
                close(server[0]);
            }
        } while (*buffer == '\n');
        cout << message << endl;

        do {
            recv(server[1], buffer, bufsize, 0);
            message[0] = '\0';
            strcat(message, buffer);
            for(i = 0; i < 255; i++){
                if(message[i] == '\n'){
                    message[i] = '\0';
                    break;
                }

            }
            
            if(strcmp(message,"/exit") == 0){
                cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr1.sin_addr);
                close(server[1]);
            }
        } while (*buffer == '\n');
        cout << message << endl;

    }

    
    close(client[0]);
    return 0;
}