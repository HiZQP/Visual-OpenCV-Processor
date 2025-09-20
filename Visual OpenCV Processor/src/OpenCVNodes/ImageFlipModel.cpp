#include "ImageFlipModel.h"
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>

void ImageFlipModel::calculate()
{
	int flipCode = _flipCodeComboBox->currentData().toInt();
	cv::flip(_originalImage, _flippedImage, flipCode);
	Q_EMIT dataUpdated(0);
}

ImageFlipModel::ImageFlipModel()
{
	_widget = new QWidget();
	_flipCodeComboBox = new QComboBox();
	_flipCodeComboBox->addItem("水平翻转", 1);
	_flipCodeComboBox->addItem("垂直翻转", 0);
	_flipCodeComboBox->addItem("水平垂直翻转", -1);
	auto layout = new QGridLayout();
	layout->addWidget(new QLabel("翻转方式："), 0, 0);
	layout->addWidget(_flipCodeComboBox, 0, 1);
	_widget->setLayout(layout);
	connect(_flipCodeComboBox, &QComboBox::currentTextChanged, this, &ImageFlipModel::calculate);
}

unsigned int ImageFlipModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageFlipModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageFlipModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if(nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_flippedImage.release();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageFlipModel::outData(QtNodes::PortIndex port)
{
	if (_flippedImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_flippedImage);
}
