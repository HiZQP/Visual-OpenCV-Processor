#include "ImageChannelExtract.h"

#include <QFormLayout>

void ImageChannelExtract::calculate()
{
	if (_originalImage.data) {
		if(_originalImage.channels() == 1)
		{
			_channelImage.release();
			Q_EMIT dataUpdated(0);
			return;
		}
		int ch = _channelSelect->currentData().toInt();
		cv::extractChannel(_originalImage, _channelImage, ch);
		Q_EMIT dataUpdated(0);
	}
}

ImageChannelExtract::ImageChannelExtract()
{
	_widget = new QWidget();
	_channelSelect = new QComboBox();
	_channelSelect->setMinimumWidth(50);
	auto layout = new QFormLayout();
	layout->addRow("选择通道", _channelSelect);
	_widget->setLayout(layout);
	connect(_channelSelect, &QComboBox::currentTextChanged, this, &ImageChannelExtract::calculate);
}

unsigned int ImageChannelExtract::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageChannelExtract::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageChannelExtract::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		_channelSelect->clear();
		int channels = _originalImage.channels();
		if (channels == 1) {
			_channelSelect->addItem("单通道图像", 0);
			_channelSelect->setEnabled(false);
		}
		else if (channels == 3) {
			_channelSelect->addItem("通道0", 0);
			_channelSelect->addItem("通道1", 1);
			_channelSelect->addItem("通道2", 2);
			_channelSelect->setEnabled(true);
		}
		else if (channels == 4) {
			_channelSelect->addItem("通道0", 0);
			_channelSelect->addItem("通道1", 1);
			_channelSelect->addItem("通道2", 2);
			_channelSelect->addItem("通道3", 3);
			_channelSelect->setEnabled(true);
		}
		else {
			for (int i = 0; i < channels; i++) {
				_channelSelect->addItem(QString("通道%1").arg(i), i);
			}
			_channelSelect->setEnabled(true);
		}
		calculate();
	}
	else {
		_channelSelect->clear();
		_originalImage.release();
		_channelImage.release();
		Q_EMIT dataInvalidated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageChannelExtract::outData(QtNodes::PortIndex port)
{
	if (_channelImage.data)
		return std::make_shared<ImageData>(_channelImage);
	else 
		return nullptr;
}
