#include <iostream>
#include "window.h"
#include "packets.h"

///////////////////////////////////////////////////////////////////////////////////////
// MainWindow
///////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
  setWindowTitle("Network Packet Handler");
  setMinimumSize(800, 600);

  QWidget* container = new QWidget;
  setCentralWidget(container);

  ///////////////////////////////////////////////////////////////////////////////////////
  // add layout
  ///////////////////////////////////////////////////////////////////////////////////////

  QVBoxLayout* layout = new QVBoxLayout(container);
  QHBoxLayout* btn_layout = new QHBoxLayout;
  btn_start = new QPushButton("Start");
  btn_stop = new QPushButton("Stop");
  btn_clear = new QPushButton("Clear");
  btn_stop->setEnabled(false);
  btn_layout->addWidget(btn_start);
  btn_layout->addWidget(btn_stop);
  btn_layout->addWidget(btn_clear);
  btn_layout->addStretch();
  layout->addLayout(btn_layout);

  ///////////////////////////////////////////////////////////////////////////////////////
  // add table
  ///////////////////////////////////////////////////////////////////////////////////////

  table = new QTableWidget;
  table->setColumnCount(5);
  QStringList headers = { "Time", "Source IP", "Destination IP", "Protocol", "Length" };
  table->setHorizontalHeaderLabels(headers);
  table->horizontalHeader()->setStretchLastSection(true);
  table->setAlternatingRowColors(false);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  layout->addWidget(table);

  ///////////////////////////////////////////////////////////////////////////////////////
  // handler
  ///////////////////////////////////////////////////////////////////////////////////////

  handler = new PacketHandler;
  connect(handler, &PacketHandler::received, this, &MainWindow::update);

  ///////////////////////////////////////////////////////////////////////////////////////
  // timer
  ///////////////////////////////////////////////////////////////////////////////////////

  timer = new QTimer;
  connect(timer, &QTimer::timeout, this, &MainWindow::update);

  ///////////////////////////////////////////////////////////////////////////////////////
  // signals and slots
  ///////////////////////////////////////////////////////////////////////////////////////

  connect(btn_start, &QPushButton::clicked, this, &MainWindow::start);
  connect(btn_stop, &QPushButton::clicked, this, &MainWindow::stop);
  connect(btn_clear, &QPushButton::clicked, this, &MainWindow::clear);
}

///////////////////////////////////////////////////////////////////////////////////////
// ~MainWindow
///////////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
  if (handler)
  {
    handler->stop();
    delete handler;
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// start
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::start()
{
  btn_start->setEnabled(false);
  btn_stop->setEnabled(true);
  handler->start();
  timer->start(1000);
}

///////////////////////////////////////////////////////////////////////////////////////
// stop
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::stop()
{
  btn_start->setEnabled(true);
  btn_stop->setEnabled(false);
  handler->stop();
  timer->stop();
}

///////////////////////////////////////////////////////////////////////////////////////
// clear
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::clear()
{
  table->setRowCount(0);
  handler->packets.clear();
}

///////////////////////////////////////////////////////////////////////////////////////
// update
///////////////////////////////////////////////////////////////////////////////////////

void MainWindow::update()
{
  std::vector<packet_info_t>& packets = handler->packets;

  if (packets.size() > table->rowCount())
  {
    int rows = table->rowCount();
    table->setRowCount(packets.size());

    for (size_t idx = rows; idx < packets.size(); ++idx)
    {
      const packet_info_t& packet = packets[idx];
      table->setItem(idx, 0, new QTableWidgetItem(QString::fromStdString(packet.timestamp)));
      table->setItem(idx, 1, new QTableWidgetItem(QString::fromStdString(packet.source_ip)));
      table->setItem(idx, 2, new QTableWidgetItem(QString::fromStdString(packet.dest_ip)));
      table->setItem(idx, 3, new QTableWidgetItem(QString::fromStdString(packet.protocol)));
      table->setItem(idx, 4, new QTableWidgetItem(QString::number(packet.length)));
    }

    table->scrollToBottom();
  }
}
