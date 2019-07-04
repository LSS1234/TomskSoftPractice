#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QComboBox;

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget();
	~Widget();

private slots:
	void createWindow();
	//void sendData();

private:
	void createGridGroupBox();
	void createFormGroupBox();

	enum { NumGridRows = 5};

	QPushButton *aboutButton;
	QPushButton *submitButton;

	QGroupBox *gridGroupBox;
	QGroupBox *formGroupBox;

	QLabel *firstCollums[NumGridRows];
	QLabel *secondCollums[NumGridRows];
	QLabel *valueLabel;
	QLabel *currencyLabel;

	QLineEdit *valueEdit;
	QComboBox *currencyBox;


};

#endif