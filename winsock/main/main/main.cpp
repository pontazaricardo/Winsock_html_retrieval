// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

//This project has two main sections:
//1. Connects to a given website (via sockets) and downloads its html.
//2. Find all the links (<a href> tags) and posts them at the end
//3. For temproal or permanent forwareded sites, use the forwarded site address instead. 

int main()
{
	ofstream fout;
	fout.open("obtainedHTML.txt"); //Will save the website's html there

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
		system("pause");
		return 1;
	}

	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Initialization of sockets

	string website;
	system("CLS");
	cout << "************ LINK COUNTER IN WEBSITE **************\n";
	cout << "\n- This program looks for the links of the form <a href=' '> in a website.\n";
	cout << "\n- Please type the website to retrieve\n";
	cout << "  (DON'T USE 'HTTP://' NOR 'HTTPS://', JUST 'www.foo.bar')\n";
	cout << "\n- Examples: www.ntu.edu.tw, www.nthu.edu.tw or arcadianhome.com\n";
	cout << "\n- For temporal or permanent forwarded sites, we cannot obtain the html.\n";
	cout << "  Please use the forwareded address instead.\n";
	cout << "\nWebsite to visit: ";
	cin >> website;

	unsigned positionSlash = website.find("/");
	
	string hostString = website.substr(0, positionSlash);
	string getString = website.substr(positionSlash + 1);

	if (positionSlash>500) {
		getString = "";
	}

	char hostStringChar[1024];
	strncpy_s(hostStringChar, hostString.c_str(), sizeof(hostStringChar));
	hostStringChar[sizeof(hostStringChar) - 1] = 0;

	struct hostent *host;
	host = gethostbyname(hostStringChar);

	if (host == NULL) {
		cout << "\nERROR: COULD NOT CONNECT TO WEBSITE.\n\n";
		fout.close();
		system("pause");
		return 1;
	}

	string direccion = "GET /";
	direccion.append(getString);
	direccion.append(" HTTP/1.1\r\nHost: ");
	direccion.append(hostString);
	direccion.append("\r\nConnection: close\r\n\r\n");

	char direccionChar[2048];
	strncpy_s(direccionChar, direccion.c_str(), sizeof(direccionChar));
	direccionChar[sizeof(direccionChar) - 1] = 0;

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;

	cout << "Connecting...\n";
	try {
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	}
	catch (exception& e) {
		cout << "\nERROR: COULD NOT CONNECT TO WEBSITE.\n\n";
		fout.close();
		system("pause");
		return 1;
	}


	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		cout << "Could not connect";
		fout.close();
		system("pause");
		return 1;
	}
	
	cout << "Connected.\n";
	send(Socket, direccionChar, strlen(direccionChar), 0);
	char buffer[500] = "";
	string htmlStringObtenido = "";
	while (recv(Socket, buffer, 1, 0) > 0) {
		
		string str(buffer);
		htmlStringObtenido += str;

	}
	fout << "===========================FOUND HTML===========================\n\n\n" << htmlStringObtenido; //htmlStringObtenido has the site's html in a string format (it used to be char).
	cout << "\nWEBSITE: " << website << "\n";
	std::size_t posicion1, posicion2;
	posicion1 = htmlStringObtenido.find("<a href=");
	
	fout << "\n**************************FOUND LINKS***************************\n";
	cout << "\n**************************FOUND LINKS***************************\n";
	if (posicion1 != string::npos) {

		string linkObtenido;
		string tokenObtenido = htmlStringObtenido;
		int counter = 1;
		while (string::npos != posicion1) {
			
			tokenObtenido = tokenObtenido.substr(posicion1);
			int posicionComillas = tokenObtenido.find_first_of(">");
			linkObtenido = tokenObtenido.substr(9, posicionComillas - 10);
			fout << "\n LINK NO. " << counter << ": " << linkObtenido << "\n";
			cout << "Link no." << counter << ": " << linkObtenido << "\n";
			counter++;
			posicion1 = tokenObtenido.find("<a href=", posicionComillas);

		}
		counter--;
		cout << "TOTAL NO. OF LINKS: " << counter << "\n";
	}
	else {
		cout << "\nNo links of the form '<a href="" ""> were found.\n";
		fout << "\nNo links of the form '<a href=>' were found.\n";
	}

	cout << "\n\nPLEASE CHECK THE FILE 'obtainedHTML.txt' FOR MORE INFORMATION.\n\n";

	closesocket(Socket);
	WSACleanup();
	fout.close();

	system("pause");

	return 0;
}

