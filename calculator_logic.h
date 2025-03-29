#ifndef CALCULATOR_LOGIC_H
#define CALCULATOR_LOGIC_H

#include <QString>
#include "stack_template.h"

class CalculatorLogic {
public:
    QString calculate(const QString& str, double& result);
private:
    int SymbolPriorityComparison(QChar ch1, QChar ch2);
};

#endif // CALCULATOR_LOGIC_H
