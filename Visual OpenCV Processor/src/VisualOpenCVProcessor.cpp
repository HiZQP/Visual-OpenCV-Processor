#include "VisualOpenCVProcessor.h"
#include <opencv2/opencv.hpp>

VisualOpenCVProcessor::VisualOpenCVProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

VisualOpenCVProcessor::~VisualOpenCVProcessor()
{}

