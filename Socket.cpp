#include "Socket.h"
#include "Logger.h"

using namespace myserver::utility;
using namespace myserver::socket;

#include <cerrno>
#include <cstring>
using namespace std;


Socket::Socket() : m_sockfd(0)
{
}

Socket::Socket(const string &ip, int port) : m_ip(ip), m_port(port), m_sockfd(0)
{
}

Socket::~Socket()
{
    close();
}

bool Socket::bind(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    if (ip != "")
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    else
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    sockaddr.sin_port = htons(port);

    if (::bind(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        Log("error" ,"socket bind error");
        return false;
    }
    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd, backlog) < 0)
    {
        Log("error" ,"socket listen error");
        return false;
    }
    return true;
}

bool Socket::connect(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        Log("error" ,"socket connect error");
        return false;
    }
    return true;
}

bool Socket::close()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
    return true;
}

int Socket::accept()
{
	int sockfd = ::accept(m_sockfd, NULL, NULL);
	if (sockfd < 0)
	{
        Log("error" ,"accept call error");
        sockfd = -1;
	}
	return sockfd;
}

int Socket::recv(char * buf, int len)
{
    return ::recv(m_sockfd, buf, len, 0);
}

int Socket::send(const char * buf, int len)
{
    return ::send(m_sockfd, buf, len, 0);
}

bool Socket::set_non_blocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        Log("error" , "Socket::set_non_blocking(F_GETFL, O_NONBLOCK)");
        return false;
    }
    flags |= O_NONBLOCK;
    if (fcntl(m_sockfd, F_SETFL, flags) < 0)
    {
        Log("error" ,"Socket::set_non_blocking(F_SETFL, O_NONBLOCK)");
        return false;
    }
    return true;
}

bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        Log("error" ,"socket set send buffer error");
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        Log("error" ,"socket set recv buffer error");
        return false;
    }
    return true;
}

//是否在close之后优雅的退出，即发送完未发送的数据
//true，为优雅
bool Socket::set_linger(bool active, int seconds)
{
    struct linger l;
    memset(&l, 0, sizeof(l));

    if (active)
        l.l_onoff = 1;
    else
        l.l_onoff = 0;
    l.l_linger = seconds;

    if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0)
    {
        Log("error" ,"socket set sock linger error");
        return false;
    }
    return true;
}

bool Socket::set_keep_alive()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0)
    {
        Log("error" ,"socket set sock keep alive error");
        return false;
    }
    return true;
}

// 一般来说，一个端口释放后会等待两分钟之后才能再被使用
// SO_REUSEADDR是让端口释放后立即就可以被再次使用。
bool Socket::set_reuse_addr()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
    {
        Log("error" ,"socket set sock reuser addr error");
        return false;
    }
    return true;
}

bool Socket::set_reuse_port()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) < 0)
    {
        Log("error" ,"socket set sock reuser port error");
        return false;
    }
    return true;
}
