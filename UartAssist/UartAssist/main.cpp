#include "qapplication.h"
#include "qtranslator.h"
#include "UartAssist.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
	QTranslator translator;
	translator.load(":/uartassist_zh.qm");
	app.installTranslator(&translator);
    UartAssist mainWindow;
    return app.exec();
}
