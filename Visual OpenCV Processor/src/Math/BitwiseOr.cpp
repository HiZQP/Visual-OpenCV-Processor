#include "BitwiseOr.h"

void BitwiseOr::calculate()
{
	if (_inputImage1.data && _inputImage2.data) {
		if (_inputImage1.size() != _inputImage2.size() || _inputImage1.type() != _inputImage2.type()) {
			_outputImage.release();
			Q_EMIT dataUpdated(0);
		}
		else {
			cv::bitwise_or(_inputImage1, _inputImage2, _outputImage);
		}
		Q_EMIT dataUpdated(0);
	}
}

unsigned int BitwiseOr::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else
		return 1;
}

QtNodes::NodeDataType BitwiseOr::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void BitwiseOr::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (portIndex == 0)
			_inputImage1 = imageData->get();
		else if (portIndex == 1)
			_inputImage2 = imageData->get();
		calculate();
	}
	else if (portIndex == 0) {
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
	else if (portIndex == 1) {
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> BitwiseOr::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data) {
		return std::make_shared<ImageData>(_outputImage);
	}
	return nullptr;
}
