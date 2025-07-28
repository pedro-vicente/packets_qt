#ifndef PACKETS_H
#define PACKETS_H

#include <QMainWindow>
#include <QDateTime>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QNetworkInterface>

#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////
// packet_info_t
///////////////////////////////////////////////////////////////////////////////////////

struct packet_info_t
{
  std::string timestamp;
  std::string source_ip;
  std::string dest_ip;
  std::string protocol;
  int length;
};

///////////////////////////////////////////////////////////////////////////////////////
// PacketHandler
///////////////////////////////////////////////////////////////////////////////////////

class PacketHandler : public QObject
{
  Q_OBJECT

public:
  PacketHandler();
  ~PacketHandler();

  int running;
  int sock;
  int start();
  int stop();
  void parse(char* buffer, int size);

  std::vector<packet_info_t> packets;
  QSocketNotifier* notifier;

signals:
  void received();

private slots:
  void handle();
};

#endif