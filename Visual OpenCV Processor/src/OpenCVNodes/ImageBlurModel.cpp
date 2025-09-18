#include "ImageBlurModel.h"

#include <QGridLayout>
#include <QLabel>

void ImageBlurModel::calculate()
{
	int ksize = _ksize->value();
	if (ksize % 2 == 0)
		ksize += 1;
	_ksize->setValue(ksize);
	int borderType = _borderType->currentData().toInt();
	if (_originalImage.data) {
		cv::blur(_originalImage, _blurredImage, cv::Size(ksize, ksize), cv::Point(-1, -1), borderType);
		Q_EMIT dataUpdated(0);
	}
}

ImageBlurModel::ImageBlurModel()
{
	_widget = new QWidget();
	_ksize = new QSpinBox();
	_ksize->setValue(5);
	_ksize->setMinimum(1);
	_ksize->setSingleStep(2); // 只允许奇数
	_borderType = new QComboBox();
	_borderType->addItem("默认", cv::BORDER_DEFAULT);
	_borderType->addItem("常量", cv::BORDER_CONSTANT);
	_borderType->addItem("复制", cv::BORDER_REPLICATE);
	_borderType->addItem("反射", cv::BORDER_REFLECT);
	_borderType->addItem("反射101", cv::BORDER_REFLECT_101);
	QLabel* label1 = new QLabel("核大小：");
	QLabel* label2 = new QLabel("边界类型：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_ksize, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_borderType, 1, 1);
	_widget->setLayout(layout);
	connect(_ksize, &QSpinBox::editingFinished, this, &ImageBlurModel::calculate);
	connect(_borderType, &QComboBox::currentTextChanged, this, &ImageBlurModel::calculate);
}

unsigned int ImageBlurModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageBlurModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageBlurModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
}

std::shared_ptr<QtNodes::NodeData> ImageBlurModel::outData(QtNodes::PortIndex port)
{
	if (_blurredImage.data)
		return std::make_shared<ImageData>(_blurredImage);
	else
		return nullptr;
}
