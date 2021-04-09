#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include "ev++.h"
#include "ev.h"
#include <stdio.h>
#include <linux/tcp.h>
#include "buffer.h"


class NetServer{

public:
   NetServer(){ sockfd = -1;}
   ~NetServer(){}
   void start();
   void accept_cb(ev::io &watcher, int revents);
   void read_cb(ev::io &watcher, int revents);
   void write_cb(ev::io &wather, int revents);
   void timer_cb(ev::timer &watcher, int revents);
   void Close();
private:
   int sockfd;
   ev::default_loop loop;
   ev::io read_io;
   ev::io write_io;
   Buffer readbuf;
   Buffer writebuf;
};
