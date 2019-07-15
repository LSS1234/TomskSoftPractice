#include <QDate>
#include "main_window.h"
#include "about.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	_currencyWired.push_back(new Currency("RUB"));
	_currencyWired.push_back(new Currency("USD"));
	_currencyWired.push_back(new Currency("EUR"));
	_currencyWired.push_back(new Currency("AUD"));
	_currencyWired.push_back(new Currency("JPY"));
	_currencyWired.push_back(new Currency("CAD"));

	_parserRequest = new RequestAPI();

	createBaseCurrencyBox();
	createResultConvertBox();

	_about = new QMenuBar(this);
	_about->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_menu = new QMenu("&Help");
	_action = _menu->addAction("&About");
	
	_about->addMenu(_menu);
	_about->show();

	_mainLayout = new QVBoxLayout(this);
	_mainLayout->addWidget(_about);
	_mainLayout->addWidget(_formGroupBox);
	_mainLayout->addWidget(_gridGroupBox);

	setLayout(_mainLayout);
	
	connect(_parserRequest, &RequestAPI::replyAccepted, this, &MainWindow::onReplyAccept);
	connect(_action, &QAction::triggered, this, &MainWindow::createAboutWindow);
}


void MainWindow::createResultConvertBox()
{
	_gridGroupBox = new QGroupBox(tr("Result convert"),this);
	_gridLayout = new QGridLayout(this);
	const int  countCollums = 5;

	for (int i = 0; i < countCollums; ++i)
	{
		_currencyCollums.push_back(new QLabel(tr("Currency %1:").arg(i + 1)));
		_gridLayout->addWidget(_currencyCollums[i], i + 1, 1);
	}

	for (int i = 0; i < countCollums; ++i)
	{
		_valueCollums.push_back(new QLabel(tr("Value :").arg(i + 1)));
		_gridLayout->addWidget(_valueCollums[i], i + 1, 2);
	}

	_gridLayout->setHorizontalSpacing(50);
	_gridLayout->setColumnStretch(1, 10);
	_gridLayout->setColumnStretch(2, 20);
	_gridGroupBox->setLayout(_gridLayout);
}


void MainWindow::createBaseCurrencyBox()
{
	_formGroupBox = new QGroupBox(this);
	_formGroupBox->setFixedHeight(100);
	_formLayout = new QFormLayout(this);

	_submitButton = new QPushButton("Submit",this);
	_submitButton->setDisabled(true);

	_dateToDay = _dateToDay.currentDate();
	QDate minimumDate(_dateToDay.year() - 1, _dateToDay.month(),
		_dateToDay.day());
	_calendar = new QDateEdit(this);
	_calendar->setDateRange(minimumDate,_dateToDay);
	_calendar->setCalendarPopup(true);
	_calendar->setDate(_dateToDay);

	_valueLabel = new QLabel(tr("Value:"),this);
	_currencyLabel = new QLabel(tr("Currency:"),this);

	_valueEdit = new QLineEdit();

	/*������������ ������� ����� � lineEdit(������ �����, �������� 6,
	� �������� 3 ������� ����� �������*/
	_valueEdit->setValidator(new QRegExpValidator
	(QRegExp("[0-9]{0,6}\[.]{0,1}\[0-9]{0,3}$")));
	

	_currencyBox = new QComboBox(this);
	for (int i = 0; i < _currencyWired.size(); i++)
	{
		_currencyBox->addItem(_currencyWired[i]->getTypeCurrency());
	}

	_formLayout->addRow(_valueLabel, _valueEdit);
	_formLayout->addRow(_currencyLabel, _currencyBox);
	_formLayout->addRow(_calendar, _submitButton);
	_formGroupBox->setLayout(_formLayout);

	connect(_valueEdit, &QLineEdit::textEdited, this, &MainWindow::setEnableButton);
	connect(_submitButton, &QPushButton::clicked, this, &MainWindow::doConvert);
	connect(_calendar, &QDateEdit::dateChanged, this, &MainWindow::selectDate);
}


void MainWindow::setEnableButton()
{
	if (!_valueEdit->text().isEmpty())
	{
		_submitButton->setEnabled(true);
	}
	if (_valueEdit->text().isEmpty())
	{
		_submitButton->setEnabled(false);
	}
}


void MainWindow::doConvert()
{
	int baseCurrencyIndex = 0;

	for (int i = 0; i < _currencyWired.size(); i++)
	{
		if (_currencyWired[i]->getTypeCurrency() == _currencyBox->currentText())
		{
			baseCurrencyIndex = i;
		}
	}
	_currencyWired.move(baseCurrencyIndex, 0);

	_parserRequest->getRequest(_currencyWired, _dateToDay);
	_submitButton->setEnabled(false);
}



void MainWindow::onReplyAccept()
{
	_currencyWired = _parserRequest->getResultParse();
	int counterCollums = 0;
	//���������� ���������� � �������� ����.
	for (int i = 1; i < _currencyWired.size(); ++i)
	{
			_currencyCollums[counterCollums]->setText(_currencyWired[i]->getTypeCurrency() + ": "
				+ QString::number(_currencyWired[i]->getRatioCurrency()));

			_valueCollums[counterCollums]->setText(_currencyWired[i]->getTypeCurrency() + ": "
				+ QString::number(_currencyWired[i]->getValue((_valueEdit->text().toDouble()))));
			counterCollums++;
	}
	_submitButton->setEnabled(true);
}


void MainWindow::selectDate(const QDate &date)
{
	_dateToDay = date;
}


void MainWindow::createAboutWindow()
{
	About about;
	about.exec();
}


MainWindow::~MainWindow() {}
