#ifndef TINY_MUDUO_BUFFER_H_
#define TINY_MUDUO_BUFFER_H_
#include<vector>
#include<string>

static const int kPrePendIndex=8;
class buffer{
public:
    buffer():buff_(1024),readIndex(kPrePendIndex),writeIndex(kPrePendIndex){}
    int Read(int fd);

    char *Begin(){return &*buff_.begin();}
    char *Beginread(){return Begin()+readIndex;}
    char *Beginwrite(){return Begin()+writeIndex;}

    int readableBytex(){return writeIndex-readIndex;}//此处返回可读取的字节数
    int writeableBytex(){return buff_.size()-writeIndex;}//此处返回可写入的字节数
    int prependableBytex(){return readIndex;}

    void ensureEnoughLen(int len)//此处当读取的数据长度大于可写入的字节数（writeindex之后的部分）,该函数来根据要写入的数据大小来选择是否扩容
    {
        if(writeableBytex()>=len)
        {
            return;
        }
        else if(prependableBytex()-kPrePendIndex+writeableBytex()>=len)
        {
            std::copy(Beginread(),Beginwrite(),Begin()+kPrePendIndex);
            readIndex=8;
            writeIndex=readIndex+readableBytex();
        }
        else
        {
            buff_.resize(buff_.size()*2);
        }
    }
    void Append(const char *data,int len)//该函数实现直接将数据粘贴到writeindex处，前提要保证位置足够
    {
        ensureEnoughLen(len);
        std::copy(data,data+len,Beginwrite());
        writeIndex+=len;
    }
    void Append(std::string data)
    {
        Append(data.data(),data.size());
    }

    std::string getSomeString(int len)
    {
        std::string ret=std::string(Beginread(),Beginread()+len);
        readIndex+=len;
        return ret;
        
    }
    std::string getAllString()
    {
        std::string ret =std::string(Beginread(),Beginread()+readableBytex());
        readIndex+=readableBytex();
        return ret;
        
    }
private:
    std::vector<char>buff_;
    int readIndex;
    int writeIndex;
};
#endif