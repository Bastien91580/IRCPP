#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/time.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define STDIN 0  /* Descripteur de fichier pour entrée standard */

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
    int verif = 0;
    int res;
    char inputString[100];

    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    struct sockaddr_in server_addr;


    // Premiere Connexion Au Serveur Pour Recup Le Port De Discution
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


    cout << "### Recepetion Du Port Pour De Discution ..." << endl;
    recv(client, buffer, bufsize, 0);
    close(client);
    // Fin Premiere Connexion, Puis Connexion Au Port De Discution

    client = socket(AF_INET, SOCK_STREAM, 0);
    portNum = atoi(buffer);
    server_addr.sin_port = htons(portNum);
    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "### Connexion Au Serveur Port : " << portNum << endl;

    
    // Choix Du Pseudo Et Verification Avec Le Serveur De Son Existance
    do {
        
        cout << "Saisir Votre Pseudo : ";
        fgets(nameUser,20,stdin);
        for(i = 0; i < 21; i++)
            if(nameUser[i] == '\n')
                nameUser[i] = '\0';

        send(client, nameUser, 21, 0);
        cout << "Pseudo : " << nameUser << endl;

        recv(client, buffer, bufsize, 0);
        if(strcmp(buffer,"KO") == 0){ 
            cout << "Pseudo Deja Utilisé" << endl;
        } else
            verif ++;
    }while(verif == 0);

    

    // Attente Des Autre Utilisateur
    cout << "### En Attente De Connexion Des Autres Utilisateurs ..." << endl;
    recv(client, buffer, bufsize, 0);
    if(strcmp(buffer, "OK"))
        cout << "### Tout Les Utilisateurs Sont Connecté ..." << endl;
    else 
        cout << "### Erreur De La Connexion De Tout Les Utilisateurs ..." << endl;
    

    // Debut Cyles Lecture/Ecriture
    cout << "\n\n=> Pour Quitter Saisir /exit.\n" << endl;
    do {
        
        // Recepetion de tout les messages
        do {
            res = recv(client, buffer, bufsize, MSG_DONTWAIT);
            if(res > 0) cout << buffer << endl;
        } while(res > 0);
        


        // Attente de 0,5 sec Si touche clavier press pour saisie
        FD_ZERO(&readfds);
        FD_SET(STDIN, &readfds);
        select(STDIN+1, &readfds, NULL, NULL, &tv);

        if (FD_ISSET(STDIN, &readfds)) {
            fgets(message,255,stdin);
            if(strlen(message) > 0){
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
            }
        }

    } while (true);

    cout << "\n### Connexion Terminée.\nBye !\n";
    close(client);
    return 0;
}