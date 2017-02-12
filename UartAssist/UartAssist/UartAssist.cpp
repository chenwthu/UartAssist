#include "UartAssist.h"

UartAssist::UartAssist(QWidget *parent) : QMainWindow(parent) {
	ui = new UI_UartAssist(this);
	ui->InitUI(this); // 初始化界面
	this->show();

	currentSerialPort = new QSerialPort(this);
	receiverTimer = new QTimer(this);
	transmitterTimer = new QTimer(this);

	InitSettings(); // 初始化参数设置
	InitSignalsAndSlots(); // 初始化信号和槽
	InitEventFilter(); // 初始化事件过滤
}

UartAssist::~UartAssist(void) {
	ui->deleteLater();

	currentSerialPort->deleteLater();
	receiverTimer->deleteLater();
	transmitterTimer->deleteLater();
}

bool UartAssist::eventFilter(QObject *obj, QEvent *e) {
	if (e->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(e);
		if ((obj == ui->transmitOne) && (ke->modifiers() == Qt::CTRL) && (ke->key() == Qt::Key_Return)) {
			ui->transmitterTransmit->click();
			return true;
		}
		else if ((obj == ui->transmitMore) && (ke->modifiers() == Qt::NoModifier)) {
			switch (ke->key()) {
			case Qt::Key_0: static_cast<QPushButton *>(ui->transmitMore->cellWidget(0, 2))->click(); break;
			case Qt::Key_1: static_cast<QPushButton *>(ui->transmitMore->cellWidget(0, 5))->click(); break;
			case Qt::Key_2: static_cast<QPushButton *>(ui->transmitMore->cellWidget(1, 2))->click(); break;
			case Qt::Key_3: static_cast<QPushButton *>(ui->transmitMore->cellWidget(1, 5))->click(); break;
			case Qt::Key_4: static_cast<QPushButton *>(ui->transmitMore->cellWidget(2, 2))->click(); break;
			case Qt::Key_5: static_cast<QPushButton *>(ui->transmitMore->cellWidget(2, 5))->click(); break;
			case Qt::Key_6: static_cast<QPushButton *>(ui->transmitMore->cellWidget(3, 2))->click(); break;
			case Qt::Key_7: static_cast<QPushButton *>(ui->transmitMore->cellWidget(3, 5))->click(); break;
			case Qt::Key_8: static_cast<QPushButton *>(ui->transmitMore->cellWidget(4, 2))->click(); break;
			case Qt::Key_9: static_cast<QPushButton *>(ui->transmitMore->cellWidget(4, 5))->click(); break;
			default: return false;
			}

			return true;
		}
	}

	return QMainWindow::eventFilter(obj, e);
}

void UartAssist::InitSettings(void) {
	currentUartState = UartState::OFF;

	/* 所有当前可用的串口 */
	ui->settingCOM->clear();
	foreach (auto const &info, QSerialPortInfo::availablePorts())
		ui->settingCOM->addItem(info.portName() + ": " + info.description());
	ui->settingCOM->setCurrentIndex(0);
	ui->settingCOM->setToolTip(ui->settingCOM->currentText());

	/* 波特率 */
	ui->settingBaudRate->clear();
	foreach (auto const &baudRate, QSerialPortInfo::standardBaudRates())
		ui->settingBaudRate->addItem(QString::number(baudRate));
	ui->settingBaudRate->setCurrentText("115200"); // 默认波特率115200

	/* 流控制 */
	ui->settingFlowCtrl->clear();
	ui->settingFlowCtrl->addItem(tr("No Flow Control"));
	ui->settingFlowCtrl->addItem(tr("Hardware Flow Control"));
	ui->settingFlowCtrl->addItem(tr("Software Flow Control"));
	ui->settingFlowCtrl->setCurrentText(tr("No Flow Control")); // 默认无流控制

	/* 数据位 */
	ui->settingDataBits->clear();
	ui->settingDataBits->addItem("5");
	ui->settingDataBits->addItem("6");
	ui->settingDataBits->addItem("7");
	ui->settingDataBits->addItem("8");
	ui->settingDataBits->setCurrentText("8"); // 默认8位数据位

	/* 停止位 */
	ui->settingStopBits->clear();
	ui->settingStopBits->addItem("1");
	ui->settingStopBits->addItem("1.5");
	ui->settingStopBits->addItem("2");
	ui->settingStopBits->setCurrentText("1"); // 默认1位停止位

	/* 奇偶校验 */
	ui->settingParity->clear();
	ui->settingParity->addItem(tr("No Parity"));
	ui->settingParity->addItem(tr("Even"));
	ui->settingParity->addItem(tr("Odd"));
	ui->settingParity->setCurrentText(tr("No Parity")); // 默认无奇偶校验

	receiverTimer->setInterval(0);
	if (!ui->transmitterPeriod->text().isEmpty())
		transmitterTimer->setInterval(ui->transmitterPeriod->text().toInt());
}

void UartAssist::InitSignalsAndSlots(void) {
	connect(currentSerialPort, &QSerialPort::errorOccurred, [this](auto error) {
		if (error == QSerialPort::ResourceError) {
			QMessageBox::critical(this, tr("Error"), tr("The device is unexpectedly removed!"));
			ui->settingCOM->removeItem(ui->settingCOM->currentIndex());
			if (currentUartState == UartState::ON)
				ui->uartOnOff->click();
		}
	});

	connect(ui->settingCOM, &QComboBox::currentTextChanged, [this] {
		ui->settingCOM->setToolTip(ui->settingCOM->currentText());
	});
	connect(ui->settingCOM, static_cast<void (QComboBox:: *)(const QString &)>(&QComboBox::highlighted), [](const QString &text) {
		QToolTip::showText(QCursor::pos(), text);
	});
	connect(ui->uartOnOff, &QPushButton::clicked, [this] {
		if (currentUartState == UartState::ON) {
			currentSerialPort->close();
			currentUartState = UartState::OFF;
			receiverTimer->stop();
			transmitterTimer->stop();
			ui->uartOnOff->setText(tr("Turn On"));
		}
		else {
			currentSerialPort->setPortName(ui->settingCOM->currentText().split(':').at(0));
			currentSerialPort->setBaudRate(ui->settingBaudRate->currentText().toInt());
			if (ui->settingFlowCtrl->currentText() == tr("No Flow Control"))
				currentSerialPort->setFlowControl(QSerialPort::NoFlowControl);
			else if (ui->settingFlowCtrl->currentText() == tr("Hardware Flow Control"))
				currentSerialPort->setFlowControl(QSerialPort::HardwareControl);
			else
				currentSerialPort->setFlowControl(QSerialPort::SoftwareControl);
			currentSerialPort->setDataBits(static_cast<QSerialPort::DataBits>(ui->settingDataBits->currentText().toInt()));
			if (ui->settingStopBits->currentText() == "1")
				currentSerialPort->setStopBits(QSerialPort::OneStop);
			else if (ui->settingStopBits->currentText() == "1.5")
				currentSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
			else
				currentSerialPort->setStopBits(QSerialPort::TwoStop);
			if (ui->settingParity->currentText() == tr("No Parity"))
				currentSerialPort->setParity(QSerialPort::NoParity);
			else if (ui->settingParity->currentText() == tr("Even"))
				currentSerialPort->setParity(QSerialPort::EvenParity);
			else
				currentSerialPort->setParity(QSerialPort::OddParity);

			if (currentSerialPort->open(QSerialPort::ReadWrite)) {
				currentUartState = UartState::ON;
				receiverTimer->start();
				if (ui->transmitterAuto->isChecked())
					transmitterTimer->start();
				ui->uartOnOff->setText(tr("Turn Off"));
			}
			else {
				QMessageBox::critical(this, tr("Error"), tr("Fail to turn on this device!"));
			}
		}
	});
	connect(ui->uartRefresh, &QPushButton::clicked, [this] {
		if (currentUartState == UartState::ON)
			QToolTip::showText(QCursor::pos(), tr("Please turn off the current device first."));
		else
			InitSettings();
	});

	connect(ui->receiverAscii, &QRadioButton::toggled, [this](bool checked) {
		if (checked) {
			QByteArray hexText = ui->receiverArea->toPlainText().replace(QRegExp("\\[\\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d\\]\\s"), "").toUtf8();
			ui->receiverArea->setPlainText(QString::fromUtf8(QByteArray::fromHex(hexText)));
		}
		ui->receiverArea->moveCursor(QTextCursor::End);
	});
	connect(ui->receiverHex, &QRadioButton::toggled, [this](bool checked) {
		if (checked) {
			QByteArray asciiText = ui->receiverArea->toPlainText().replace(QRegExp("\\[\\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d\\]\\s"), "").toUtf8();
			QString hexText = QString::fromUtf8(asciiText.toHex().toUpper());
			int len = hexText.length();
			ui->receiverArea->clear();
			for (int i = 0; i < len; i += 2)
				ui->receiverArea->insertPlainText(hexText.mid(i, 2) + ' ');
		}
	});
	connect(ui->receiverClear, &QPushButton::clicked, [this] {
		ui->receiverArea->clear();
		ui->resetReceived->click();
	});
	connect(ui->receiverSave, &QPushButton::clicked, [this] {
		QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), "/", "*.txt;;*.log");
		if (!filename.isEmpty()) {
			QFile file(filename);
			if (file.open(QFile::WriteOnly | QFile::Text)) {
				QTextStream os(&file);
				os << ui->receiverArea->toPlainText();
			}
		}
	});
	connect(receiverTimer, &QTimer::timeout, [this] {
		if (currentSerialPort->isReadable()) {
			QByteArray data = currentSerialPort->readAll();
			if (data.isEmpty()) return;

			ui->receiverArea->moveCursor(QTextCursor::End);
			if (ui->receiverShowTime->isChecked())
				ui->receiverArea->insertPlainText(QTime::currentTime().toString("[HH:mm:ss.zzz] "));

			if (ui->receiverAscii->isChecked()) {
				ui->receiverArea->insertPlainText(QString::fromUtf8(data));
			}
			else {
				QByteArray hexText = data.toHex().toUpper();
				int len = hexText.length();
				for (int i = 0; i < len; i += 2)
					ui->receiverArea->insertPlainText(QString::fromUtf8(hexText.mid(i, 2)) + ' ');
			}

			QRegExp rx("\\d+");
			rx.indexIn(ui->receivedBytes->text());
			ui->receivedBytes->setText(ui->receivedBytes->text().replace(rx, QString::number(rx.cap().toInt() + data.size())));
		}
	});

	connect(ui->transmitterAuto, &QCheckBox::toggled, [this](bool checked) {
		if (checked)
			transmitterTimer->start();
		else
			transmitterTimer->stop();
	});
	connect(ui->transmitterPeriod, &QLineEdit::editingFinished, [this] {
		transmitterTimer->setInterval(ui->transmitterPeriod->text().toInt());
	});
	connect(ui->transmitterTransmit, &QPushButton::clicked, [this] {
		if (currentSerialPort->isWritable()) {
			if (ui->transmitterArea->currentWidget() == ui->transmitOne) {
				QByteArray data = ui->transmitOne->toPlainText().replace(QRegExp("\\r\\n|\\r|\\n"), "\r\n").toUtf8();
				if (ui->transmitterAscii->isChecked()) {
					if (ui->transmitterNewLine->isChecked()) data += "\r\n";
				}
				else {
					if (ui->transmitterNewLine->isChecked()) data += "0D0A";
					data = QByteArray::fromHex(data);
				}
				currentSerialPort->write(data);

				QRegExp rx("\\d+");
				rx.indexIn(ui->transmittedBytes->text());
				ui->transmittedBytes->setText(ui->transmittedBytes->text().replace(rx, QString::number(rx.cap().toInt() + data.size())));
			}
			else {
				for (int row = 0; row < ui->transmitMore->rowCount(); ++row)
					for (int col = 0; col < ui->transmitMore->columnCount(); col += 3)
						if (static_cast<QCheckBox *>(ui->transmitMore->cellWidget(row, col))->isChecked())
							static_cast<QPushButton *>(ui->transmitMore->cellWidget(row, col + 2))->click();
			}
		}
	});
	connect(transmitterTimer, &QTimer::timeout, ui->transmitterTransmit, &QPushButton::click);

	connect(ui->resetReceived, &QPushButton::clicked, [this] {
		ui->receivedBytes->setText(ui->receivedBytes->text().replace(QRegExp("\\d+"), "0"));
	});
	connect(ui->resetTransmitted, &QPushButton::clicked, [this] {
		ui->transmittedBytes->setText(ui->transmittedBytes->text().replace(QRegExp("\\d+"), "0"));
	});
	connect(ui->resetBoth, &QPushButton::clicked, ui->resetReceived, &QPushButton::click);
	connect(ui->resetBoth, &QPushButton::clicked, ui->resetTransmitted, &QPushButton::click);

	for (int row = 0; row < ui->transmitMore->rowCount(); ++row)
		for (int col = 0; col < ui->transmitMore->columnCount(); col += 3)
			connect(static_cast<QPushButton *>(ui->transmitMore->cellWidget(row, col + 2)), &QPushButton::clicked, [this, row, col] {
				if (currentSerialPort->isWritable()) {
					QByteArray data = static_cast<QLineEdit *>(ui->transmitMore->cellWidget(row, col + 1))->text().toUtf8();
					if (ui->transmitterAscii->isChecked()) {
						if (ui->transmitterNewLine->isChecked()) data += "\r\n";
					}
					else {
						if (ui->transmitterNewLine->isChecked()) data += "0D0A";
						data = QByteArray::fromHex(data);
					}
					currentSerialPort->write(data);

					QRegExp rx("\\d+");
					rx.indexIn(ui->transmittedBytes->text());
					ui->transmittedBytes->setText(ui->transmittedBytes->text().replace(rx, QString::number(rx.cap().toInt() + data.size())));
				}
			});
}

void UartAssist::InitEventFilter(void) {
	ui->transmitOne->installEventFilter(this);
	ui->transmitMore->installEventFilter(this);
}
