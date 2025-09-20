#include "ImageImmedianBlurModel.h"

#include <QGridLayout>
#include <QLabel>

void ImageImmedianBlurModel::calculate()
{
	int ksize = _ksize->value();
	if (ksize % 2 == 0)
		ksize += 1;
	_ksize->setValue(ksize);
	cv::medianBlur(_originalImage, _blurredImage, ksize);
	Q_EMIT dataUpdated(0);
}

ImageImmedianBlurModel::ImageImmedianBlurModel()
{
	_widget = new QWidget();
	_ksize = new QSpinBox();
	_ksize->setValue(5);
	_ksize->setMinimum(1);
	_ksize->setSingleStep(2); // 只允许奇数
	QLabel* label1 = new QLabel("核大小：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_ksize, 0, 1);
	_widget->setLayout(layout);
	connect(_ksize, &QSpinBox::editingFinished, this, &ImageImmedianBlurModel::calculate);
}

unsigned int ImageImmedianBlurModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageImmedianBlurModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageImmedianBlurModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
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

std::shared_ptr<QtNodes::NodeData> ImageImmedianBlurModel::outData(QtNodes::PortIndex port)
{
	if (_blurredImage.data)
		return std::make_shared<ImageData>(_blurredImage);
	else
		return nullptr;
}
