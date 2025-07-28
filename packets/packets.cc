#include <iostream>
#include "window.h"
#include "packets.h"

#ifdef _WIN32
#ifndef SIO_RCVALL
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////
// PacketHandler
///////////////////////////////////////////////////////////////////////////////////////

PacketHandler::PacketHandler() : running(false), sock(-1), notifier(nullptr)
{
#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {

  }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////
// ~PacketHandler
///////////////////////////////////////////////////////////////////////////////////////

PacketHandler::~PacketHandler()
{
  stop();
#ifdef _WIN32
  WSACleanup();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////
// start
///////////////////////////////////////////////////////////////////////////////////////

int PacketHandler::start()
{
  if (running)
  {
    return 0;
  }

#ifdef _WIN32
  sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
  if (sock == INVALID_SOCKET)
  {
    std::cerr << "Socket creation failed, error: " << WSAGetLastError() << std::endl;
    return -1;
  }

  char host_name[256];
  if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
  {
    std::cerr << "Failed to get hostname, error: " << WSAGetLastError() << std::endl;
    closesocket(sock);
    return -1;
  }

  struct hostent* host_info = gethostbyname(host_name);
  if (host_info == nullptr)
  {
    std::cerr << "Failed to get host info, error: " << WSAGetLastError() << std::endl;
    closesocket(sock);
    return -1;
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = *(u_long*)host_info->h_addr_list[0];
  addr.sin_port = 0;

  if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
  {
    std::cerr << "Bind failed, error: " << WSAGetLastError() << std::endl;
    closesocket(sock);
    return -1;
  }

  DWORD optval = 1;
  if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
  {
    std::cerr << "Failed to set IP_HDRINCL, error: " << WSAGetLastError() << std::endl;
    closesocket(sock);
    return -1;
  }

  if (ioctlsocket(sock, SIO_RCVALL, &optval) == SOCKET_ERROR)
  {
    std::cerr << "Failed to set promiscuous mode, error: " << WSAGetLastError() << std::endl;
    closesocket(sock);
    return -1;
  }
#else
  sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock < 0) {
    return -1;
  }
#endif

  notifier = new QSocketNotifier(sock, QSocketNotifier::Read, this);
  connect(notifier, &QSocketNotifier::activated, this, &PacketHandler::handle);
  notifier->setEnabled(true);
  running = true;
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// stop
///////////////////////////////////////////////////////////////////////////////////////

int PacketHandler::stop()
{
  if (!running)
  {
    return 0;
  }

  running = 0;
  if (notifier)
  {
    notifier->setEnabled(false);
    delete notifier;
    notifier = nullptr;
  }

  if (sock != -1)
  {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    sock = -1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// handle
///////////////////////////////////////////////////////////////////////////////////////

void PacketHandler::handle()
{
  if (!running)
  {
    return;
  }

  char buffer[65536];
  int bytes_received = recv(sock, buffer, sizeof(buffer), 0);

  if (bytes_received > 0)
  {
    parse(buffer, bytes_received);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// parse
///////////////////////////////////////////////////////////////////////////////////////

void PacketHandler::parse(char* buffer, int size)
{
  packet_info_t packet;
  packet.length = size;
  packet.timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toStdString();

#ifdef _WIN32
  struct iphdr
  {
    unsigned char ihl : 4;
    unsigned char version : 4;
    unsigned char tos;
    unsigned short tot_len;
    unsigned short id;
    unsigned short frag_off;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short check;
    unsigned int saddr;
    unsigned int daddr;
  };

  iphdr* ip_header = (iphdr*)buffer;
  struct in_addr addr;
  addr.s_addr = ip_header->saddr;
  packet.source_ip = inet_ntoa(addr);
  addr.s_addr = ip_header->daddr;
  packet.dest_ip = inet_ntoa(addr);

  switch (ip_header->protocol)
  {
  case IPPROTO_TCP: packet.protocol = "TCP"; break;
  case IPPROTO_UDP: packet.protocol = "UDP"; break;
  case IPPROTO_ICMP: packet.protocol = "ICMP"; break;
  default: packet.protocol = "Other"; break;
  }
#else
  struct ethhdr* eth_header = (ethhdr*)buffer;

  if (ntohs(eth_header->h_proto) == ETH_P_IP)
  {
    struct iphdr* ip_header = (struct iphdr*)(buffer + sizeof(ethhdr));
    struct in_addr addr;
    addr.s_addr = ip_header->saddr;
    packet.source_ip = inet_ntoa(addr);
    addr.s_addr = ip_header->daddr;
    packet.dest_ip = inet_ntoa(addr);

    switch (ip_header->protocol)
    {
    case IPPROTO_TCP: packet.protocol = "TCP"; break;
    case IPPROTO_UDP: packet.protocol = "UDP"; break;
    case IPPROTO_ICMP: packet.protocol = "ICMP"; break;
    default: packet.protocol = "Other"; break;
    }
  }
  else
  {
    packet.source_ip = "N/A";
    packet.dest_ip = "N/A";
    packet.protocol = "Ethernet";
  }
#endif

  packets.push_back(packet);
  emit received();
}

