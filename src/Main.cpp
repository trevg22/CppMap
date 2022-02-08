#include "View.h"

int main(int argc, char **argv) {
  QApplication app(argc,argv);
  View *view = new View(&app);
  return app.exec();
}
