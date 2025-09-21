#include "ImageGaussianBlurModel.h"

#include <QLabel>
#include <QGridLayout>


void ImageGaussianBlurModel::calculate()
{
	if (_originalImage.data) {
		int ksize = _kernelSize->value();
		if (ksize % 2 == 0) // ksize must be odd
			ksize += 1;
		_kernelSize->setValue(ksize);
		int borderType = _borderType->currentData().toInt();
		cv::GaussianBlur(_originalImage, _blurredImage, cv::Size(ksize, ksize), 0, 0, borderType);
		Q_EMIT dataUpdated(0);
	}
}

ImageGaussianBlurModel::ImageGaussianBlurModel()
{
	_widget = new QWidget();
	_kernelSize = new QSpinBox();
	_kernelSize->setValue(5);
	_kernelSize->setMinimum(1);
	_kernelSize->setSingleStep(2);
	_kernelSize->setToolTip("高斯核的大小，必须为正奇数");
	_borderType = new QComboBox();
	// 高斯模糊的边界类型选项
	_borderType->addItem("默认", cv::BORDER_DEFAULT);
	_borderType->addItem("常量", cv::BORDER_CONSTANT);
	_borderType->addItem("复制", cv::BORDER_REPLICATE);
	_borderType->addItem("反射", cv::BORDER_REFLECT);
	_borderType->addItem("反射101", cv::BORDER_REFLECT_101);
	_borderType->setCurrentIndex(4);
	_borderType->setToolTip("像素外推法");
	QLabel* label1 = new QLabel("高斯核大小：");
	QLabel* label2 = new QLabel("边界类型：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_kernelSize, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_borderType, 1, 1);
	_widget->setLayout(layout);
	connect(_kernelSize, &QSpinBox::editingFinished, this, &ImageGaussianBlurModel::calculate);
	connect(_borderType, &QComboBox::currentTextChanged, this, &ImageGaussianBlurModel::calculate);
}

unsigned int ImageGaussianBlurModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageGaussianBlurModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageGaussianBlurModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_blurredImage.release();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageGaussianBlurModel::outData(QtNodes::PortIndex port)
{
	if (_blurredImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_blurredImage);
}
