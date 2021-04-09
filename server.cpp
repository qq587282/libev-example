#include "server.h"

void NetServer::start()
{
   sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
   if(sockfd < 0)
   {
      perror("create socket error");
      exit(0); 
   }
   struct sockaddr_in sddr, cddr;
   sddr.sin_family = AF_INET;
   sddr.sin_addr.s_addr = INADDR_ANY;
   sddr.sin_port = htons(8888);
   
   if(bind(sockfd, (struct sockaddr*)&sddr, sizeof(sddr)) < 0)
   {
	   perror("bind error");
	   exit(0);
   }
   if(listen(sockfd, 20)<0)
   {
	   perror("listen error");
	   exit(0);
   }
   ev::io serv_io;
   serv_io.set<NetServer, &NetServer::accept_cb>(this);
   serv_io.start(sockfd, ev::READ);
   std::cout << "server start now...\n";
 
   loop.run(0);
}
 
void NetServer::accept_cb(ev::io &watcher, int revents)
{
	struct sockaddr_in cddr;
	socklen_t cli_len = sizeof(cddr);
	int clifd = -1;
	if(EV_ERROR & revents)
	{
		perror("get valid events");
		return;
	}
	clifd = accept4(watcher.fd, (struct sockaddr*)&cddr, &cli_len, SOCK_NONBLOCK|SOCK_CLOEXEC);
    if(clifd < 0)
		return;
	int one = 1;
	setsockopt(clifd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one));
	read_io.set<NetServer, &NetServer::read_cb>(this);
	write_io.set<NetServer, &NetServer::write_cb>(this);
	read_io.start(clifd, ev::READ);
	write_io.start(clifd, ev::WRITE);
}
 
void NetServer::read_cb(ev::io &watcher, int revents)
{
	readbuf.set_write_buf_size(1);
	int readlen = recv(watcher.fd, readbuf.buf+readbuf.writeIdx, readbuf.get_writeable_size(), 0);
	
	if(readlen < 0)
	{
		if(errno == EAGAIN || errno== EWOULDBLOCK)
			return;
		Close();
		return;
	}
	if(readlen == 0)
	{
		Close();
		return;
	}
	if(readlen > 0)
	{
		//if(readlen == len)
		printf("recv buf:%s\n", readbuf.buf+readbuf.readIdx);
	}
	readbuf.readIdx += readlen;
	
	
	
	/*int fd=w->fd;
	ev_io_stop(loop,  w);
	ev_io_init(w,write_callback,fd,EV_WRITE);
	ev_io_start(loop,w);
	printf("socket fd : %d, turn read 2 write loop! ",fd);*/

}
 
void NetServer::write_cb(ev::io &watcher, int revents)
{	
	char buffer[1024]={0};
	//ev_io read_event;
	snprintf(buffer,1023,"this is a libev server!\n");
	write(watcher.fd,buffer,strlen(buffer)); 
	
	
	
	/*int fd=w->fd;
	ev_io_stop(loop,  w);
	ev_io_init(w,recv_callback,fd,EV_READ);
	ev_io_start(loop,w);*/

}
 
void NetServer::Close()
{
	::close(sockfd);
	sockfd = -1;
}