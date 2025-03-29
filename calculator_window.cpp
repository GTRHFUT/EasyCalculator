#include "calculator_window.h"
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QRegularExpression>
CalculatorWindow::CalculatorWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("计算器");
    setWindowIcon(QIcon(":/Image/calculator.ico"));
    setFixedSize(400, 600);

    // 显示区域设置
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");
    display->setStyleSheet(R"(
        QLineEdit {
            background-color: #f8ece0;
            border: 2px solid #e0d6c9;
            border-radius: 10px;
            font-size: 28px;
            padding: 20px;
            text-align: right;
            box-shadow: inset 0 2px 5px rgba(0,0,0,0.1);
        }
    )");

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);
    mainLayout->addWidget(display, 0, 0, 1, 4);
    createButtonLayout(mainLayout);
}

void CalculatorWindow::createButtonLayout(QGridLayout *mainLayout) {
    QStringList buttons = {
        "%", "C", "⌫", ".",
        "1/x", "x²", "√x", "/",
        "7", "8", "9", "×",
        "4", "5", "6", "-",
        "1", "2", "3", "+",
        "0", "(", ")", "="
    };

    QGridLayout *buttonLayout = new QGridLayout;
    int row = 0, col = 0;
    for (const QString &text : buttons) {
        QPushButton *btn = new QPushButton(text);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 基础样式
        QString style = R"(
            QPushButton {
                background-color: #f5f5f5;
                border: 2px solid #e0e0e0;
                border-radius: 15px;
                font-size: 20px;
                padding: 15px;
                margin: 2px;
            }
            QPushButton:hover { background-color: #e0e0e0; }
            QPushButton:pressed { background-color: #d0d0d0; }
        )";

        // 运算符特殊样式
        if (text == "+" || text == "-" || text == "×" || text == "/"
            || text == "%" || text == "√x" || text == "1/x" || text == "x²") {
            style = R"(
                QPushButton {
                    background-color: #e3f2fd;
                    border: 2px solid #bbdefb;
                    border-radius: 15px;
                    font-size: 20px;
                    padding: 15px;
                    margin: 2px;
                }
                QPushButton:hover { background-color: #bbdefb; }
                QPushButton:pressed { background-color: #90caf9; }
            )";
        }

        // 等号特殊样式
        if (text == "=") {
            style = R"(
                QPushButton {
                    background-color: #00acc1;
                    border: 2px solid #00838f;
                    color: white;
                    border-radius: 15px;
                    font-size: 20px;
                    padding: 15px;
                    margin: 2px;
                }
                QPushButton:hover { background-color: #00838f; }
                QPushButton:pressed { background-color: #006064; }
            )";
        }

        btn->setStyleSheet(style);
        connect(btn, &QPushButton::clicked, this, &CalculatorWindow::onButtonClicked);
        buttonLayout->addWidget(btn, row, col);
        col = (col + 1) % 4;
        if (col == 0) row++;
    }
    mainLayout->addLayout(buttonLayout, 1, 0, 1, 4);
}

void CalculatorWindow::onButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString text = btn->text();

    // 符号转换
    if (text == "×") text = "*";
    if (text == "√x") text = QString(QChar(0x221A));  // √
    if (text == "x²") text = QString(QChar(0x00B2));  // x²
    if (text == "1/x") text = "r";

    // 单目运算符处理
    if (text == QString(QChar(0x221A)) || text == QString(QChar(0x00B2)) || text == "r") {
        QString current = display->text();
        if (current == "0" && text == "r") {
            QMessageBox::warning(this, "Error", "Cannot perform this operation on 0");
            return;
        }
        bool ok;
        double num = current.toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error", "invalid number");
            return;
        }

        if (text == QString(QChar(0x221A))) {
            if (num < 0) {
                QMessageBox::warning(this, "Error", "The square root cannot be negative");
                return;
            }
            display->setText(QString::number(std::sqrt(num)));
        } else if (text == QString(QChar(0x00B2))) {
            display->setText(QString::number(num * num));
        } else if (text == "r") {
            if (num == 0) {
                QMessageBox::warning(this, "Error", "divide by zero");
                return;
            }
            display->setText(QString::number(1.0 / num));
        }
        return;
    }

    // 其他按钮处理
    if (text == "=") {
        QString expr = display->text() + '#';
        double result;
        QString error = logic.calculate(expr, result);

        if (error.isEmpty()) {
            display->setText(QString::number(result, 'g', 12));
        } else {
            QMessageBox::warning(this, "Error", error);
            display->setText("0");
        }
    } else if (text == "C") {
        display->setText("0");
    } else if (text == "⌫") {
        QString current = display->text();
        if (current == "0") return;
        current.chop(1);
        display->setText(current.isEmpty() ? "0" : current);
    } else {
        if (display->text() == "0" &&!text.contains(QRegularExpression("[+\\-*/%]"))) {
            display->setText(text);
        } else {
            display->setText(display->text() + text);
        }
    }
}
