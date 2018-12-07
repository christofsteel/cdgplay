#include "dlgcdg.h"
#include "arguments.h"
#include <QApplication>
#include <QFileInfo>
#include <QCommandLineParser>
#include "settings.h"
#include <QTextStream>

Arguments *arguments;


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setApplicationName("cdgplay");
  QApplication::setApplicationVersion("1.0");

  arguments = new Arguments;

  QCommandLineParser parser;
  parser.setApplicationDescription("simply plays cdg files");
  parser.addHelpOption();
  parser.addVersionOption();  
  QCommandLineOption monitorOption({{"m", "monitor"}, "Set Monitor output", "MONITOR"});
  parser.addOption(monitorOption);
  parser.addPositionalArgument("cdg_file", "CDG_File to play");
  parser.process(a);

  if(parser.isSet(monitorOption)) {
      std::cout << (parser.value(monitorOption).toStdString()) << std::endl;
      arguments->monitor = parser.value(monitorOption).toInt();
  }

  if(parser.positionalArguments().length() < 1) {
      //qFatal("CDG file is missing");
      exit(-1);
  }
  arguments->cdg_file = parser.positionalArguments().at(0);

  DlgCdg w;

  w.show();

  return a.exec();
}
