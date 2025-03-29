#ifndef CALCULATOR_WINDOW_H
#define CALCULATOR_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include "calculator_logic.h"

class CalculatorWindow : public QWidget {
    Q_OBJECT
public:
    CalculatorWindow(QWidget *parent = nullptr);
private slots:
    void onButtonClicked(); // 仅声明
private:
    QLineEdit *display;
    CalculatorLogic logic;
    void createButtonLayout(QGridLayout *mainLayout);
};

#endif // CALCULATOR_WINDOW_H
