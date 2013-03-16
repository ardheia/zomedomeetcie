#include <QtGui/QApplication>
#include "fenprincipale.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fenPrincipale w;
    w.show();
    //if(argv[1]!="")
    //    w.ouvrir(argv[1]);

    return a.exec();
}
