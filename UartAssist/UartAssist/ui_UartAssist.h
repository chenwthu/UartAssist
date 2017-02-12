#ifndef UI_UARTASSIST_H
#define UI_UARTASSIST_H

#include <qapplication.h>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdesktopwidget.h>
#include <qfile.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qheaderview.h>
#include <qicon.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmainwindow.h>
#include <qplaintextedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qstring.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qvalidator.h>

class UI_UartAssist : public QMainWindow { Q_OBJECT
public:
	UI_UartAssist(QWidget *);
	~UI_UartAssist(void);

	void InitUI(QMainWindow *);

private:
	void InitLayout(QMainWindow *);
	void InitStyle(QMainWindow *);

public:
	QDesktopWidget *desktop;
	QWidget *centralWidget;

	QHBoxLayout *layout;
	QVBoxLayout *leftLayout;
	QVBoxLayout *rightLayout;
	QGroupBox *settings;
	QFormLayout *settingsLayout;
	QGroupBox *receiverOptions;
	QVBoxLayout *receiverOptionsLayout;
	QHBoxLayout *receiverFormatLayout;
	QGroupBox *transmitterOptions;
	QVBoxLayout *transmitterOptionsLayout;
	QHBoxLayout *transmitterFormatLayout;
	QHBoxLayout *transmitterAutoLayout;
	QGroupBox *countBytes;
	QVBoxLayout *countBytesLayout;
	QHBoxLayout *countReceivedLayout;
	QHBoxLayout *countTransmittedLayout;

	QPlainTextEdit *receiverArea;
	QTabWidget *transmitterArea;
	QPlainTextEdit *transmitOne;
	QTableWidget *transmitMore;

	QComboBox *settingCOM;
	QComboBox *settingBaudRate;
	QRegExpValidator *settingBaudRateValidator;
	QComboBox *settingFlowCtrl;
	QComboBox *settingDataBits;
	QComboBox *settingStopBits;
	QComboBox *settingParity;
	QPushButton *uartRefresh;
	QPushButton *uartOnOff;

	QRadioButton *receiverAscii;
	QRadioButton *receiverHex;
	QCheckBox *receiverShowTime;
	QPushButton *receiverClear;
	QPushButton *receiverSave;

	QRadioButton *transmitterAscii;
	QRadioButton *transmitterHex;
	QCheckBox *transmitterNewLine;
	QCheckBox *transmitterAuto;
	QLabel *transmitterEvery;
	QLineEdit *transmitterPeriod;
	QRegExpValidator *transmitterPeriodValidator;
	QLabel *transmitterMs;
	QPushButton *transmitterTransmit;

	QLabel *receivedBytes;
	QPushButton *resetReceived;
	QLabel *transmittedBytes;
	QPushButton *resetTransmitted;
	QPushButton *resetBoth;
};

#endif
