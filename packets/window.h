#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "packets.h"

///////////////////////////////////////////////////////////////////////////////////////
// MainWindow
///////////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  QTableWidget* table;
  QPushButton* btn_start;
  QPushButton* btn_stop;
  QPushButton* btn_clear;
  PacketHandler* handler;
  QTimer* timer;

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void start();
  void stop();
  void clear();
  void update();
};


#endif
