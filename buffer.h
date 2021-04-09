/*************************************************************************
    > File Name: buffer.h
    > Author: 
    > Mail: @163.com 
    > Created Time: Sun 25 Sep 2016 08:34:19 AM PDT
 ************************************************************************/
 
#include<iostream>
#include<stdio.h>
#include<string.h>
 
using namespace std;
#define default_buf_size 256
class Buffer
{
	public:
		Buffer()
		{
			buf = new char[default_buf_size];
			buflen = default_buf_size;
		}
		~Buffer()
		{
			delete[] buf;
		}
		void set_write_buf_size(int needsize)
		{
			int buf_size = writeIdx + needsize;
			if(buf_size <= buflen)
				return;				
			if(needsize < readIdx)
			{
				memmove(buf, buf+readIdx, writeIdx-readIdx);
				writeIdx -= readIdx;
				readIdx = 0;
				return;
			}
			while(buflen < buf_size)
				buflen *= 2;
 
			char *oldbuf = buf;
			buf = new char[buflen];
			memcpy(buf, oldbuf+readIdx, writeIdx-readIdx);
			delete[] oldbuf;
			writeIdx -= readIdx;
			readIdx = 0;
			
		}
        int get_readable_size()
		{
			return writeIdx-readIdx;
		}
		int get_writeable_size()
		{
			return buflen-writeIdx;
		}
 
		void writebuf(void *data, int datalen)
		{
			set_write_buf_size(datalen);
			memcpy(buf+writeIdx, data, datalen);
			writeIdx += datalen;
		}
 
		int writeIdx;
		int readIdx;
		int buflen;
		char *buf;
};
