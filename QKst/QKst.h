#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QKst.h"

class QKst : public QMainWindow
{
    Q_OBJECT

public:
    QKst(QWidget *parent = Q_NULLPTR);

private:
    Ui::QKstClass ui;
};
