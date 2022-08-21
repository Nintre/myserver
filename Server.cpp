#include"Server.h"
#include"Singleton.h"

using namespace myserver::utility;
using namespace myserver::server;

Server::Server() : m_ip(""), m_port(0), m_threads(1024), m_connects(1024), m_wait_time(10){
}

Server::~Server(){
}