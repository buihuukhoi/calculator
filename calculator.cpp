#include <QRegularExpression>
#include "calculator.h"
#include "./ui_calculator.h"

double calc_val = 0.0;
double mem_val = 0.0;
bool div_trigger = false;
bool mult_trigger = false;
bool add_trigger = false;
bool sub_trigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calc_val));
    QPushButton *num_buttons[10];
    for (int i = 0; i < 10; i++) {
        QString but_name = "Button" + QString::number(i);
        num_buttons[i] = Calculator::findChild<QPushButton *>(but_name);
        connect(num_buttons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButoonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButoonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButoonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButoonPressed()));

    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualPressed()));

    connect(ui->Sign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));

    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearPressed()));
    connect(ui->ClearEntry, SIGNAL(released()), this, SLOT(ClearEntryPressed()));

    connect(ui->Dot, SIGNAL(released()), this, SLOT(DotPressed()));

    connect(ui->PowerTwo, SIGNAL(released()), this, SLOT(PowerTwoPressed()));

    connect(ui->MemAdd, SIGNAL(released()), this, SLOT(MemPressed()));
    connect(ui->MemSub, SIGNAL(released()), this, SLOT(MemPressed()));
    connect(ui->MemClear, SIGNAL(released()), this, SLOT(MemPressed()));
    connect(ui->MemRecall, SIGNAL(released()), this, SLOT(MemPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString but_val = button->text();
    QString display_val = ui->Display->text();

    if (display_val.toDouble() == 0 && !display_val.contains(".", Qt::CaseInsensitive)) {
        ui->Display->setText(but_val);
    } else {
        QString new_val = display_val + but_val;
        ui->Display->setText(new_val);
    }
}

void Calculator::MathButoonPressed() {
    div_trigger = false;
    mult_trigger = false;
    add_trigger = false;
    sub_trigger = false;

    QString display_val = ui->Display->text();
    if (QString::compare(display_val, "", Qt::CaseInsensitive) != 0) {
        calc_val = display_val.toDouble();
    }

    QPushButton *button = (QPushButton *)sender();

    QString but_val = button->text();
    if (QString::compare(but_val, "/", Qt::CaseInsensitive) == 0) {
        div_trigger = true;
    } else if (QString::compare(but_val, "*", Qt::CaseInsensitive) == 0) {
        mult_trigger = true;
    } else if (QString::compare(but_val, "+", Qt::CaseInsensitive) == 0) {
        add_trigger = true;
    } else {
        sub_trigger = true;
    }
    ui->Display->setText(QString::number(0.0));
}

void Calculator::EqualPressed() {
    double res = 0.0;
    QString display_val = ui->Display->text();
    double dbl_display_val = display_val.toDouble();
    if (add_trigger || sub_trigger || mult_trigger || div_trigger) {
        if (add_trigger) {
            res = calc_val + dbl_display_val;
        } else if (sub_trigger) {
            res = calc_val - dbl_display_val;
        } else if (mult_trigger) {
            res = calc_val * dbl_display_val;
        } else if (div_trigger) {
            res = calc_val / dbl_display_val;
        }
    }
    ui->Display->setText(QString::number(res, 'g', 16));
}

void Calculator::ChangeNumberSign() {
    QString display_val = ui->Display->text();
    QRegularExpression re("[-]?[0-9.]*");
    QRegularExpressionMatch match = re.match(display_val);

    if (match.hasMatch()) {
        double dbl_display_val = display_val.toDouble();
        double dbl_display_val_sign = -1 * dbl_display_val;
        ui->Display->setText(QString::number(dbl_display_val_sign));
    }
}

void Calculator::ClearPressed() {
    calc_val = 0.0;
    div_trigger = false;
    mult_trigger = false;
    add_trigger = false;
    sub_trigger = false;

    ui->Display->setText(QString::number(calc_val));
}

void Calculator::ClearEntryPressed() {
    ui->Display->setText(QString::number(0.0));
}

void Calculator::DotPressed() {
    QString display_val = ui->Display->text();
    QString dot = ".";

    if (!display_val.contains(dot, Qt::CaseInsensitive)) {
        QString new_display_val = display_val + dot;
        ui->Display->setText(new_display_val);
    }
}

void Calculator::PowerTwoPressed() {
    double res = 0.0;
    QString display_val = ui->Display->text();
    double dbl_display_val = display_val.toDouble();
    res = dbl_display_val * dbl_display_val;
    ui->Display->setText(QString::number(res, 'g', 16));
}

void Calculator::MemPressed() {
    QString display_val = ui->Display->text();

    QPushButton *button = (QPushButton *)sender();
    QString but_val = button->text();

    if (QString::compare(but_val, "M+", Qt::CaseInsensitive) == 0) {
        mem_val += display_val.toDouble();
    } else if (QString::compare(but_val, "M-", Qt::CaseInsensitive) == 0) {
        mem_val -= display_val.toDouble();
    } else if (QString::compare(but_val, "MC", Qt::CaseInsensitive) == 0) {
        mem_val = 0.0;
    } else {  // MR
        ui->Display->setText(QString::number(mem_val, 'g', 16));
    }
}

