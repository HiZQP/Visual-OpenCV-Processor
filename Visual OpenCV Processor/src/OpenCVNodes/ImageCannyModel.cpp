#include "ImageCannyModel.h"
#include <QGridLayout>
#include <QLabel>

void ImageCannyModel::calculate()
{
	int threshold1 = _threshold1->value();
	int threshold2 = _threshold2->value();
	bool L2gradient = _L2gradient->isChecked();
	cv::Canny(_originalImage, _cannyImage, threshold1, threshold2, 3, L2gradient);
	Q_EMIT dataUpdated(0);
}

ImageCannyModel::ImageCannyModel()
{
	_widget = new QWidget();
	_threshold1 = new QSpinBox();
	_threshold1->setValue(100);
	_threshold1->setMinimum(0);
	_threshold1->setMaximum(1000);
	QLabel* label1 = new QLabel("阈值1：");
	_threshold2 = new QSpinBox();
	_threshold2->setValue(200);
	_threshold2->setMinimum(0);
	_threshold2->setMaximum(1000);
	QLabel* label2 = new QLabel("阈值2：");
	_L2gradient = new QCheckBox();
	_L2gradient->setChecked(false);
	QLabel* label3 = new QLabel("使用更精确的L2梯度计算：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_threshold1, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_threshold2, 1, 1);
	layout->addWidget(label3, 2, 0);
	layout->addWidget(_L2gradient, 2, 1);
	_widget->setLayout(layout);
	connect(_threshold1, &QSpinBox::editingFinished, this, &ImageCannyModel::calculate);
	connect(_threshold2, &QSpinBox::editingFinished, this, &ImageCannyModel::calculate);
	connect(_L2gradient, &QCheckBox::checkStateChanged, this, &ImageCannyModel::calculate);
}

unsigned int ImageCannyModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageCannyModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageCannyModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_cannyImage.release();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageCannyModel::outData(QtNodes::PortIndex port)
{
	if (_cannyImage.data)
		return std::make_shared<ImageData>(_cannyImage);
	else
		return nullptr;
}
