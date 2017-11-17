#include "player.h"
#include "dlgcdg.h"
#include "libCDG/include/libCDG.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  DlgCdg w;
  w.show();

  return a.exec();
}
