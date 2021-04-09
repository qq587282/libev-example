#include "server.h"

//g++ -o libev_server libev_server.cpp -I/home/laijia/workfile/libev/ -L/usr/local/lib -lev


int main()
{
	NetServer netServer;
	netServer.start();
	return 0;
}
