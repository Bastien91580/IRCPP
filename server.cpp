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

int main(int argc, char ** argv)
{
    int nombreUser = atoi(argv[1]);
    int connexionClient, connexionServer;
    int client[nombreUser], server[nombreUser];
    int connected = nombreUser;
    int i, j, k;
    int portNum = 1500;
    int nextPort = 1600;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char message[277]; //Pseudo 21 + Message 256
    int clientCount = 1;
    char name[nombreUser][21];
    char verifName[21];

    struct sockaddr_in server_addr_connexion, server_addr[2];
    socklen_t size;

    // Socket de Connexion
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

    // Socket de discution
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

        // Verification du Pseudo
        int nameExist = 0;
        do{
            nameExist = 0;
            recv(server[j], verifName, 21, 0);
            for(i = 0; i < j; i++){
                if(strcmp(verifName, name[i]) == 0){
                    buffer[0] = 'K';
                    buffer[1] = 'O';
                    buffer[2] = '\0';
                    send(server[j], buffer, bufsize, 0);
                    nameExist++;
                }
            }
        }while(nameExist > 0);

        buffer[0] = 'O';
        buffer[0] = 'K';
        buffer[0] = '\0';
        send(server[j], buffer, bufsize, 0);
        
        strcpy(name[j], verifName);
    }

    // Conversation prête
    if(j == nombreUser){
        buffer[0] = 'O';
        buffer[0] = 'K';
        buffer[0] = '\0';
        for(i = 0; i < nombreUser; i++)
            send(server[i], buffer, bufsize, 0);
        cout << "Tout les Utilisateur sont connectés !" << endl;
    }

    int res;

    while (nombreUser > 0) 
    {
        for(j = 0; j < nombreUser; j ++){

            // Reception du message
            do {
                res = recv(server[j], buffer, bufsize, MSG_DONTWAIT);
                message[0] = '\0';
                strcat(message, buffer);
                for(i = 0; i < 255; i++){
                    if(message[i] == '\n'){
                        message[i] = '\0';
                        break;
                    }
                }
                
                // Commande pour mettre fin a la connexion
                if(strcmp(message,"/exit") == 0){
                    cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr[j].sin_addr);
                    nombreUser --;
                    close(server[j]);
                }
            } while (*buffer == '\n');
            
            // Revois du message aux autres utilisateurs
            if(res > 0){
                cout << message << endl;
                for(k = 0; k < nombreUser; k++)
                    if(k != j && server[k] > 0)
                            send(server[k], message, bufsize, 0);
            }
        }
    }

    
    
    return 0;
}