#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include "window.h"

///////////////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  MainWindow window;
  QSize screen = QGuiApplication::primaryScreen()->size();
  window.resize(screen.width(), screen.height());
  window.showMaximized();
  return app.exec();
}
