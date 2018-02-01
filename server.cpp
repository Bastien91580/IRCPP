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
    int nombreUser = 2;
    int connexionClient, connexionServer;
    int client[nombreUser], server[nombreUser], i, j, k;
    int portNum = 1500;
    int nextPort = 1600;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char message[277]; //Pseudo 21 + Message 256
    int clientCount = 1;

    struct sockaddr_in server_addr_connexion, server_addr[2];


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
        cout << "### Erreur binding connexion, La connexion est deja etabli sur ce port (" << portNum << ") ..." << endl;
        return -1;
    }
    size = sizeof(server_addr_connexion);
    cout << "### Attente connexion User ..." << endl;

    for(j = 0; j < nombreUser; j ++){

        listen(connexionClient, 1);

        connexionServer = accept(connexionClient,(struct sockaddr *)&server_addr_connexion,&size);
        if (connexionServer < 0) 
            cout << "### Erreur validation connexion ..." << endl;

        buffer[0]='\0';
        sprintf(buffer,"%d", nextPort);
        send(connexionServer, buffer, bufsize, 0);

        

        client[j] = socket(AF_INET, SOCK_STREAM, 0);

        server_addr[j].sin_family = AF_INET;
        server_addr[j].sin_addr.s_addr = htons(INADDR_ANY);
        server_addr[j].sin_port = htons(nextPort);

        if ((bind(client[j], (struct sockaddr*)&server_addr[j],sizeof(server_addr[j]))) < 0) 
        {
            cout << "### Erreur binding connexion, La connexion est deja etabli sur ce port (" << nextPort << ") ..." << endl;
            return -1;
        }
        nextPort++;
        size = sizeof(server_addr[j]);
        cout << "### Attente connexion User ..." << endl;
        listen(client[j], 1);

        server[j] = accept(client[j],(struct sockaddr *)&server_addr[j],&size);
        if (server[j] < 0) 
            cout << "### Erreur validation connexion ..." << endl;
        else
            cout << "User #" << j << " connecté" << endl;


    }


    while (server[0] > 0) 
    {
        for(j = 0; j < nombreUser; j ++){
            do {
                recv(server[j], buffer, bufsize, 0);
                message[0] = '\0';
                strcat(message, buffer);
                for(i = 0; i < 255; i++){
                    if(message[i] == '\n'){
                        message[i] = '\0';
                        break;
                    }

                }
                
                if(strcmp(message,"/exit") == 0){
                    cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr[j].sin_addr);
                    close(server[j]);
                }
            } while (*buffer == '\n');
            cout << message << endl;
            for(k = 0; k < nombreUser; k++){
                if(k != j){
                    send(server[k], message, bufsize, 0);
                }
            }
        }

    }

    
    
    return 0;
}