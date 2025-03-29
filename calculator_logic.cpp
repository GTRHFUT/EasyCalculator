#include "calculator_logic.h"
#include <QMessageBox>
#include <cmath>

QString CalculatorLogic::calculate(const QString& str, double& result) {
    QString expr = str;
    //存放运算符号的栈
    Stack<QChar> s1;
    //存放数字的栈
    Stack<double> s2;
    //存放左括号的栈，用于检查括号是否匹配
    Stack<QChar> bracket;
    //字符串下标索引，数字元素
    int i = 0;
    s1.push('#');
    while (i < expr.length()) {
        //判断是否是负数
        if (expr[i] == '-' && (i == 0 || expr[i - 1] == '(')) {
            double n = 0;
            //记录小数位的权重，为1时代表是整数部分
            double decimal = 1;
            //是否是小数
            bool isDecimal = false;
            i++; //跳过负号
            while (expr[i].isDigit() || expr[i] == '.') {
                if (expr[i] == '.') {
                    //如果小数点前面不是数字
                    if (i == 0 || !expr[i-1].isDigit()) {
                        qDebug() << "Error: Illegal decimal point at position " << i + 1 << '\n';
                        return "Error: Illegal decimal point";
                    }
                    //如果一个数字的小数点个数大于1
                    if (isDecimal) {
                        qDebug() << "Error: Multiple decimal points in a number at position " << i + 1 << '\n';
                        return "Error: Multiple decimal points";
                    }
                    isDecimal = true;
                }
                else {
                    if (isDecimal) {
                        decimal *= 0.1;
                        n += (expr[i].digitValue()) * decimal;
                    }
                    else {
                        n = n * 10 + (expr[i].digitValue());
                    }
                }
                i++;
            }
            s2.push(-n);
        }
        //判断是否是数字或小数点
        else if (expr[i].isDigit() || expr[i] == '.') {
            double n = 0;
            double decimal = 1;
            bool isDecimal = false;
            while (expr[i].isDigit() || expr[i] == '.') {
                if (expr[i] == '.') {
                    if (i == 0 || !expr[i - 1].isDigit()) {
                        qDebug() << "Error: Illegal decimal point at position " << i + 1 << '\n';
                        return "Error: Illegal decimal point";
                    }
                    if (isDecimal) {
                        qDebug() << "Error: Multiple decimal points in a number at position " << i + 1 << '\n';
                        return "Error: Multiple decimal points";
                    }
                    isDecimal = true;
                }
                else {
                    if (isDecimal) {
                        decimal *= 0.1;
                        n += (expr[i].digitValue()) * decimal;
                    }
                    else {
                        n = n * 10 + (expr[i].digitValue());
                    }
                }
                i++;
            }
            s2.push(n);
        }
        //判断是否是运算符
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '(' ||
                 expr[i] == ')'|| expr[i] == '#'|| expr[i] == '%'
                 || expr[i] == QChar(0x221A)  // √
                 || expr[i] == 'r'            // 1/x
                 || expr[i] == QChar(0x00B2)){  // x²
            QChar ch;
            s1.get_top(ch);
            if (expr[i] == '(' || expr[i] == '[' || expr[i] == '{')
                bracket.push(str[i]);
            QChar brackettop = ' ';
            if (!bracket.empty())
                bracket.get_top(brackettop);
            if (expr[i] == ')') {
                if (bracket.empty() || brackettop != '(') {
                    qDebug() << "Error: Mismatched parentheses at position " << i + 1 << '\n';
                    return "Error: Mismatched parentheses";
                }
                //将一对对应的括号从括号栈中移除
                if (ch == '(')
                    bracket.pop();
            }

            //如果栈顶元素的优先级小于待入栈运算符，则将该运算符入栈
            if (SymbolPriorityComparison(ch, str[i]) == 1) {
                s1.push(str[i]);
                i++;
            }
            //如果栈顶元素的优先级大于待入栈运算符，则进行栈顶运算符和数字栈的上面元素进行运算
            else if (SymbolPriorityComparison(ch, str[i]) == -1) {
                s1.pop();
                double num1, num2;
                // 增加栈空检查
                if (s2.empty() && ch !='(') {
                    qDebug() << "Error: Insufficient operands in the number stack." << '\n';
                    return "Error: Insufficient operands";
                }
                s2.get_top(num1);
                s2.pop();
                if (s2.empty() && ch !='(') {
                    qDebug() << "Error: Insufficient operands in the number stack." << '\n';
                    return "Error: Insufficient operands";
                }
                s2.get_top(num2);
                s2.pop();
                //双目运算符
                if (ch == '+') {
                    s2.push(num2 + num1);
                }
                else if (ch == '-') {
                    s2.push(num2 - num1);
                }
                else if (ch == '*') {
                    s2.push(num2 * num1);
                }
                else if (ch == '/') {
                    if (num1 == 0) {
                        qDebug() << "Error: divide by zero" << '\n';
                        return "Error: divide by zero";
                    }
                    s2.push(num2 / num1);
                }
                else if(ch == '%'){
                    if (num1 == 0) {
                        qDebug() << "Error: divide by zero" << '\n';
                        return "Error: divide by zero";
                    }
                    s2.push(static_cast<int>(num2) % static_cast<int>(num1));
                }
                else if(ch == '('){
                    qDebug() << "Error: Unmatched left brackets in the expression" << '\n';
                    return "Error: Unmatched left brackets";
                }
                //单目运算符
                //在calculator_window.cpp中运算
            }
            //如果运算符优先级相同
            else if (SymbolPriorityComparison(ch, str[i]) == 0) {
                s1.pop();
                i++;
            }
        }
        else {
            qDebug() << "Error: Illegal character '" << str[i] << "' at position " << i + 1 << '\n';
            return "Error: Illegal character";
        }
    }
    //检查括号栈是否为空，如果不为空说明有括号未匹配
    if (!bracket.empty()) {
        qDebug() << "Error: Unmatched left brackets in the expression" << '\n';
        return "Error: Unmatched left brackets";
    }
    //如果符号栈为空，则运算结束
    if (s1.empty()) {
        s2.get_top(result);
        qDebug() << result << '\n';
        return "";
    }
    //如果不为空
    else {
        qDebug() << "Error: Unprocessed operators in the expression" << '\n';
        return "Error: Unprocessed operators";
    }
}


int CalculatorLogic::SymbolPriorityComparison(QChar ch1, QChar ch2) {
    if (ch1 == '+' || ch1 == '-') {
        if (ch2 == '+' || ch2 == '-' || ch2 == '#' || ch2 == ')')
            return -1;
        else if (ch2 == '*' || ch2 == '/' || ch2 == '%' || ch2 == '(' || ch2 == QChar(0x221A)
                 || ch2 == 'r' || ch2 == QChar(0x00B2))
            return 1;
    }
    else if (ch1 == '*' || ch1 == '/'|| ch1 == '%' || ch2 == QChar(0x221A)
             || ch2 == 'r' || ch2 == QChar(0x00B2)) {
        if (ch2 == '+' || ch2 == '-' || ch2 == '*' || ch2 == '/' || ch2 == '%'|| ch2 == QChar(0x221A)
            || ch2 == 'r' || ch2 == QChar(0x00B2)|| ch2 == '#' || ch2 == ')')
            return -1;
        else if (ch2 == '(' )
            return 1;
    }
    else if (ch1 == '#') {
        if (ch2 == '#')
            return 0;
        return 1;
    }
    else if (ch1 == '(') {
        if (ch2 == ')')
            return 0;
        else if (ch2 == '#')
            return -1;
        else if (ch2 == '+' || ch2 == '-' || ch2 == '*' || ch2 == '/' || ch2 == '%'
                 || ch2 == QChar(0x221A) || ch2 == 'r' || ch2 == QChar(0x00B2)|| ch2 == '(')
            return 1;
    }
    else if (ch1 == ')') {
        if (ch2 == '#' || ch2 == '+' || ch2 == '-' || ch2 == '*' || ch2 == '/'|| ch2 == '%'
            || ch2 == QChar(0x221A)|| ch2 == 'r' || ch2 == QChar(0x00B2))
            return -1;
    }

    return 0;
}
