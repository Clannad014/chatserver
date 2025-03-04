#include"buffer.h"
#include <sys/uio.h>
#include<stdio.h>
int buffer::Read(int fd)//从fd读取数据写入到首地址为writeindex的地方，writeindex右移，可读取的字节数增加
{
    char extrabuff[65536];
    struct iovec io[2];
    io[0].iov_base=Beginwrite();
    io[0].iov_len=writeableBytex();
    io[1].iov_base=extrabuff;
    io[1].iov_len=65536;
    int iovcnt=writeableBytex()<static_cast<int>(sizeof(extrabuff))? 2:1;
    int readsize=readv(fd,io,iovcnt);
    int writeable=writeableBytex();

    if(readsize<0)
    {
        printf("readv error\n");
    }
    else if(readsize<=writeableBytex())
    {
        writeIndex+=readsize;
    }
    else
    {
        writeIndex=buff_.size();
        Append(extrabuff,readsize-writeable);
    }
    return readsize;
}