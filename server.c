/*
 * server.c
 *
 *  Created on: 17 ott 2016
 *      Author: Federico De Laurentiis
 */


#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>




void ErrorHandler(char[]);

void ClearWinSock() ;


int main() {

#if defined WIN32
	// Initialize Winsock
	WSADATA wsaData;
	int iResult =WSAStartup(MAKEWORD(2,2),&wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
		}
	#endif
// INIZIO CODICE SOCKET


//DEFINIZIONE STRUCT PER MODELLARE INDIRIZZO IP E PORTA
	struct sockaddr_in {
		short sin_family;
		u_short sin_port;// u_short , presente in Winsock
		struct in_addr sin_addr;// in_addr, presente in Winsock
		char sin_zerp[8];

	};

	struct sockaddr_in cad;
	int clientSocket; //id della socket_client
	int clientLen;	  //dimensione indirizzo socket client
	int MySocket;// id socket
	int qlen=6; // massimo numero di socket accettabili  dalla socket


// ***CREAZIONE SOCKET***
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		printf("socket creation failed.\n");
		return 0;
	}


	struct sockaddr_in sad;
	sad.sin_family = AF_INET;// valore costante Winsock"2"
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );//localHost
	sad.sin_port = htons( 27015 );//conversione per la rete

//***ASSEGNAZIONE INDIRIZZO ALLA SOCKET ***

    //bind( id_socket , puntatore a sockaddr_in , byte indirizzo );
	if ( bind(MySocket, (struct sockaddr*) &sad, sizeof(sad)) <0) {
	ErrorHandler("bind() failed.\n");
	closesocket(MySocket);
	return 0;
	}


//***SETTAGGIO SOCKET IN ASCOLTO ***

	if (listen (MySocket, qlen) < 0) {
	ErrorHandler("listen() failed.\n");
	closesocket(MySocket);
	return 0;

	}


	printf(" serverSocket in ascolto ....");


//***CICLO PER ACCETTARE LE SOCKET DEI CLIENT***
	while (1) {

	clientLen = sizeof(cad); // set the size of the client address
	if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad,&clientLen)) < 0)
	{
		ErrorHandler("accept() failed .\n");
		// CHIUSURA DELLA CONNESSIONE
		closesocket(MySocket);
		ClearWinSock();
		return 0;

	}

	printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

	}




// CHIUSURA SOCKET
	closesocket(MySocket);
	#if defined WIN32
	WSACleanup();
	#endif
	return 0;
} // main end


void ErrorHandler(char *errorMessage) {

   printf(errorMessage);

}

void ClearWinSock() {

  #if defined WIN32
  WSACleanup();
  #endif

}






