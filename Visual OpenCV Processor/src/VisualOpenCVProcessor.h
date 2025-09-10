#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualOpenCVProcessor.h"

class VisualOpenCVProcessor : public QMainWindow
{
    Q_OBJECT

public:
    VisualOpenCVProcessor(QWidget *parent = nullptr);
    ~VisualOpenCVProcessor();

private:
    Ui::VisualOpenCVProcessorClass ui;
};

