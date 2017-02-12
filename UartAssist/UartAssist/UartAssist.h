#ifndef UARTASSIST_H
#define UARTASSIST_H

#include <qbytearray.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <qtooltip.h>
#include "ui_UartAssist.h"

class UartAssist : public QMainWindow { Q_OBJECT
public:
    UartAssist(QWidget *parent = Q_NULLPTR);
	~UartAssist(void);

protected:
	virtual bool eventFilter(QObject *, QEvent *);

private:
	void InitSettings(void);
	void InitSignalsAndSlots(void);
	void InitEventFilter(void);

    UI_UartAssist *ui;

	enum class UartState { ON, OFF } currentUartState;
	QSerialPort *currentSerialPort;
	QTimer *receiverTimer;
	QTimer *transmitterTimer;
};

#endif
