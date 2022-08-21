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


bool Socket::close()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
    return true;
}

