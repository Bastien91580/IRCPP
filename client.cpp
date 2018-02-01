#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    int client;
    int connexionPort = 1500;
    int portNum;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char nameUser[21];
    char message[256];
    int i = 0;

    struct sockaddr_in server_addr;

    ///////////////////////////////////// USER NAME
    cout << "Saisir Votre Pseudo : ";
    fgets(nameUser,20,stdin);
    for(i = 0; i < 21; i++)
        if(nameUser[i] == '\n')
            nameUser[i] = '\0';
    cout << "Pseudo : " << nameUser;


    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        cout << "\nErreur Création Socket ..." << endl;
        exit(1);
    }

    cout << "\n### Socket Client Créé ..." << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(connexionPort);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "### Connexion Au Serveur Port : " << connexionPort << endl;


    cout << "### En Attente De Confirmation Du Serveur ..." << endl;
    recv(client, buffer, bufsize, 0);
    cout << "### Connexion Confirmée." << endl;

    close(client);

    client = socket(AF_INET, SOCK_STREAM, 0);

    portNum = atoi(buffer);
    server_addr.sin_port = htons(portNum);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "### Connexion Au Serveur Port : " << portNum << endl;


    cout << "### En Attente De Confirmation Du Serveur ..." << endl;
    cout << "\n\n=> Pour Quitter Saisir /exit.\n" << endl;


    do {
        cout << ">: ";
        ///////////////////////////////////// MESSAGE
        fgets(message,255,stdin);
        for(i = 0; i < 255; i++)
            if(message[i] == '\n'){
                message[i] = '\0';
                break;
            }
        buffer[0] = '\0';
        strcat(buffer,nameUser);
        strcat(buffer," : ");
        strcat(buffer, message);

        send(client, buffer, bufsize, 0);

        if(strcmp(message,"/exit") == 0)
            break;
            //recv(client, buffer, bufsize, 0);


    } while (true);

    cout << "\n### Connexion Terminée.\nBye !\n";
    close(client);
    return 0;
}