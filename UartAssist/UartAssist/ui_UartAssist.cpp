#include "ui_UartAssist.h"

UI_UartAssist::UI_UartAssist(QWidget *parent) : QMainWindow(parent) {
	centralWidget = new QWidget(parent);

	layout = new QHBoxLayout(centralWidget);
	leftLayout = new QVBoxLayout(centralWidget);
	rightLayout = new QVBoxLayout(centralWidget);
	settings = new QGroupBox(tr("Settings"), centralWidget);
	settingsLayout = new QFormLayout(settings);
	receiverOptions = new QGroupBox(tr("Receiver Options"), centralWidget);
	receiverOptionsLayout = new QVBoxLayout(receiverOptions);
	receiverFormatLayout = new QHBoxLayout(receiverOptions);
	transmitterOptions = new QGroupBox(tr("Transmitter Options"), centralWidget);
	transmitterOptionsLayout = new QVBoxLayout(transmitterOptions);
	transmitterFormatLayout = new QHBoxLayout(transmitterOptions);
	transmitterAutoLayout = new QHBoxLayout(transmitterOptions);
	countBytes = new QGroupBox(tr("Count"), centralWidget);
	countBytesLayout = new QVBoxLayout(countBytes);
	countReceivedLayout = new QHBoxLayout(countBytes);
	countTransmittedLayout = new QHBoxLayout(countBytes);

	receiverArea = new QPlainTextEdit(centralWidget);
	transmitterArea = new QTabWidget(centralWidget);
	transmitOne = new QPlainTextEdit(transmitterArea);
	transmitMore = new QTableWidget(5, 6, transmitterArea);

	settingCOM = new QComboBox(settings);
	settingBaudRate = new QComboBox(settings);
	settingBaudRateValidator = new QRegExpValidator(QRegExp("\\d{1,10}"), settings);
	settingFlowCtrl = new QComboBox(settings);
	settingDataBits = new QComboBox(settings);
	settingStopBits = new QComboBox(settings);
	settingParity = new QComboBox(settings);
	uartRefresh = new QPushButton(tr("Refresh"), settings);
	uartOnOff = new QPushButton(tr("Turn On"), settings);

	receiverAscii = new QRadioButton(tr("ASCII", "receiver"), receiverOptions);
	receiverHex = new QRadioButton(tr("Hex", "receiver"), receiverOptions);
	receiverShowTime = new QCheckBox(tr("Show Time"), receiverOptions);
	receiverClear = new QPushButton(tr("Clear"), receiverOptions);
	receiverSave = new QPushButton(tr("Save"), receiverOptions);

	transmitterAscii = new QRadioButton(tr("ASCII", "transmitter"), transmitterOptions);
	transmitterHex = new QRadioButton(tr("Hex", "transmitter"), transmitterOptions);
	transmitterNewLine = new QCheckBox(tr("Append \"\\r\\n\""), transmitterOptions);
	transmitterAuto = new QCheckBox(tr("Auto"), transmitterOptions);
	transmitterEvery = new QLabel(tr("every"), transmitterOptions);
	transmitterPeriod = new QLineEdit(transmitterOptions);
	transmitterPeriodValidator = new QRegExpValidator(QRegExp("\\d{1,4}"), transmitterOptions);
	transmitterMs = new QLabel(tr("ms"), transmitterOptions);
	transmitterTransmit = new QPushButton(tr("Transmit"), transmitterOptions);

	receivedBytes = new QLabel(tr("Received: ") + "0" + tr("B"), countBytes);
	resetReceived = new QPushButton(tr("Reset"), countBytes);
	transmittedBytes = new QLabel(tr("Transmitted: ") + "0" + tr("B"), countBytes);
	resetTransmitted = new QPushButton(tr("Reset"), countBytes);
	resetBoth = new QPushButton(tr("Reset Both"), countBytes);
}

UI_UartAssist::~UI_UartAssist(void) {
	centralWidget->deleteLater();

	layout->deleteLater();
	leftLayout->deleteLater();
	rightLayout->deleteLater();
	settings->deleteLater();
	settingsLayout->deleteLater();
	receiverOptions->deleteLater();
	receiverOptionsLayout->deleteLater();
	receiverFormatLayout->deleteLater();
	transmitterOptions->deleteLater();
	transmitterOptionsLayout->deleteLater();
	transmitterFormatLayout->deleteLater();
	transmitterAutoLayout->deleteLater();
	countBytes->deleteLater();
	countBytesLayout->deleteLater();
	countReceivedLayout->deleteLater();
	countTransmittedLayout->deleteLater();

	receiverArea->deleteLater();
	transmitterArea->deleteLater();
	transmitOne->deleteLater();
	for (int row = 0; row < transmitMore->rowCount(); ++row)
		for (int col = 0; col < transmitMore->columnCount(); ++col)
			transmitMore->cellWidget(row, col)->deleteLater();
	transmitMore->deleteLater();

	settingCOM->deleteLater();
	settingBaudRate->deleteLater();
	settingBaudRateValidator->deleteLater();
	settingFlowCtrl->deleteLater();
	settingDataBits->deleteLater();
	settingStopBits->deleteLater();
	settingParity->deleteLater();
	uartRefresh->deleteLater();
	uartOnOff->deleteLater();

	receiverAscii->deleteLater();
	receiverHex->deleteLater();
	receiverShowTime->deleteLater();
	receiverClear->deleteLater();
	receiverSave->deleteLater();

	transmitterAscii->deleteLater();
	transmitterHex->deleteLater();
	transmitterNewLine->deleteLater();
	transmitterAuto->deleteLater();
	transmitterEvery->deleteLater();
	transmitterPeriod->deleteLater();
	transmitterPeriodValidator->deleteLater();
	transmitterMs->deleteLater();
	transmitterTransmit->deleteLater();

	receivedBytes->deleteLater();
	resetReceived->deleteLater();
	transmittedBytes->deleteLater();
	resetTransmitted->deleteLater();
	resetBoth->deleteLater();
}

void UI_UartAssist::InitUI(QMainWindow *mainWindow) {
	InitLayout(mainWindow); // 初始化布局
	InitStyle(mainWindow); // 初始化样式
}

void UI_UartAssist::InitLayout(QMainWindow *mainWindow) {
	/* 主窗口 */
	mainWindow->setWindowIcon(QIcon(":/UartAssist.ico")); // 图标
	mainWindow->setWindowTitle(tr("Uart Assist")); // 标题

	/* 布局 */
	mainWindow->setCentralWidget(centralWidget);

	centralWidget->setLayout(layout); // 整体布局
	layout->addLayout(leftLayout); // 左半部分布局
	layout->addLayout(rightLayout); // 右半部分布局
	layout->setStretchFactor(leftLayout, 4); // 左半部分占80%
	layout->setStretchFactor(rightLayout, 1); // 右半部分占20%

	rightLayout->addWidget(settings); // 参数设置
	settings->setLayout(settingsLayout);

	rightLayout->addWidget(receiverOptions); // 接收选项
	receiverOptions->setLayout(receiverOptionsLayout);

	rightLayout->addWidget(transmitterOptions); // 发送选项
	transmitterOptions->setLayout(transmitterOptionsLayout);

	rightLayout->addWidget(countBytes); // 计数区
	countBytes->setLayout(countBytesLayout);

	/* 接收区 */
	leftLayout->addWidget(receiverArea);
	leftLayout->setStretchFactor(receiverArea, 7); // 接收区占70%
	receiverArea->setReadOnly(true); // 设置为只读
	receiverArea->setCenterOnScroll(true); // 自动滚动使光标显示在中央

	/* 发送区 */
	leftLayout->addWidget(transmitterArea);
	leftLayout->setStretchFactor(transmitterArea, 3); // 发送区占30%
	transmitterArea->addTab(transmitOne, tr("Transmit One")); // 单条发送
	transmitterArea->addTab(transmitMore, tr("Transmit More")); // 多条发送
	transmitterArea->setTabPosition(QTabWidget::West); // 标签栏在左侧
	transmitterArea->setUsesScrollButtons(false); // 不使用滚动按钮

	/* 多条发送 */
	transmitMore->horizontalHeader()->setVisible(false); // 隐藏列标题
	transmitMore->verticalHeader()->setVisible(false); // 隐藏行标题
	transmitMore->setFrameShape(QFrame::NoFrame); // 隐藏外边框
	transmitMore->setShowGrid(false); // 隐藏单元格边框
	transmitMore->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自适应列宽
	transmitMore->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // 拉伸第1列
	transmitMore->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch); // 拉伸第4列
	transmitMore->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 拉伸行高
	transmitMore->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
	transmitMore->setSelectionMode(QAbstractItemView::NoSelection); // 不允许选中
	for (int row = 0; row < transmitMore->rowCount(); ++row)
		for (int col = 0; col < transmitMore->columnCount(); ++col)
			switch (col % 3) {
			case 0: transmitMore->setCellWidget(row, col, new QCheckBox(QString::number(row * 2 + col / 3), transmitMore)); break;
			case 1: transmitMore->setCellWidget(row, col, new QLineEdit("", transmitMore)); break;
			case 2: transmitMore->setCellWidget(row, col, new QPushButton(tr("Transmit"), transmitMore)); break;
			}

	/* 参数设置 */
	settingsLayout->addRow(tr("COM"), settingCOM); // 串口选择
	settingsLayout->addRow(tr("Baud Rate"), settingBaudRate); // 波特率
	settingBaudRate->setEditable(true); // 波特率可编辑
	settingBaudRate->setValidator(settingBaudRateValidator); // 只能输入1至10位数字
	settingsLayout->addRow(tr("Flow Control"), settingFlowCtrl); // 流控制
	settingsLayout->addRow(tr("Data Bits"), settingDataBits); // 数据位
	settingsLayout->addRow(tr("Stop Bits"), settingStopBits); // 停止位
	settingsLayout->addRow(tr("Parity"), settingParity); // 奇偶校验
	settingsLayout->addRow(uartOnOff); // 打开或关闭串口
	settingsLayout->addRow(uartRefresh); // 刷新串口

	/* 接收选项 */
	receiverOptionsLayout->addLayout(receiverFormatLayout);
	receiverFormatLayout->addWidget(receiverAscii); // ASCII显示
	receiverAscii->setChecked(true); // 默认选中ASCII
	receiverFormatLayout->addWidget(receiverHex); // 16进制显示
	receiverOptionsLayout->addWidget(receiverShowTime); // 显示时间
	receiverOptionsLayout->addWidget(receiverClear); // 清屏
	receiverOptionsLayout->addWidget(receiverSave); // 保存

	/* 发送选项 */
	transmitterOptionsLayout->addLayout(transmitterFormatLayout);
	transmitterFormatLayout->addWidget(transmitterAscii); // ASCII发送
	transmitterAscii->setChecked(true); // 默认选中ASCII
	transmitterFormatLayout->addWidget(transmitterHex); // 16进制发送
	transmitterOptionsLayout->addWidget(transmitterNewLine); // 自动换行
	transmitterNewLine->setChecked(true); // 默认自动换行
	transmitterOptionsLayout->addLayout(transmitterAutoLayout);
	transmitterAutoLayout->addWidget(transmitterAuto); // 定时发送
	transmitterAutoLayout->addStretch(); // 增加间隔
	transmitterAutoLayout->addWidget(transmitterEvery); // 每隔
	transmitterAutoLayout->addWidget(transmitterPeriod); // 周期
	transmitterPeriod->setText("1000"); // 默认周期1000毫秒
	transmitterPeriod->setAlignment(Qt::AlignCenter); // 居中显示
	transmitterPeriod->setValidator(transmitterPeriodValidator); // 只能输入1至4位数字
	transmitterAutoLayout->addWidget(transmitterMs); // 毫秒
	transmitterOptionsLayout->addWidget(transmitterTransmit); // 发送

	/* 计数区 */
	countBytesLayout->addLayout(countReceivedLayout);
	countReceivedLayout->addWidget(receivedBytes); // 收到字节数
	countReceivedLayout->addWidget(resetReceived); // 收到字节数复位
	countBytesLayout->addLayout(countTransmittedLayout);
	countTransmittedLayout->addWidget(transmittedBytes); // 发出字节数
	countTransmittedLayout->addWidget(resetTransmitted); // 发出字节数复位
	countBytesLayout->addWidget(resetBoth); // 全部复位
}

void UI_UartAssist::InitStyle(QMainWindow *mainWindow) {
	desktop = QApplication::desktop();
	mainWindow->setMinimumHeight(desktop->availableGeometry().height() * 0.9); // 高度
	mainWindow->setMinimumWidth(desktop->availableGeometry().width() * 0.7); // 宽度

	receiverArea->setObjectName("receiverArea");
	transmitterArea->setObjectName("transmitterArea");
	transmitOne->setObjectName("transmitOne");
	transmitMore->setObjectName("transmitMore");
	settings->setObjectName("settings");
	receiverOptions->setObjectName("receiverOptions");
	transmitterOptions->setObjectName("transmitterOptions");
	countBytes->setObjectName("countBytes");

	QFile qssFile(":/UartAssist.qss");
	if (qssFile.open(QFile::ReadOnly)) {
		QString qss = QString::fromLatin1(qssFile.readAll());
		mainWindow->setStyleSheet(qss);
		qssFile.close();
	}
}
