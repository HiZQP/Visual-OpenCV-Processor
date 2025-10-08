#include "WarpPerspectiveModel.h"

#include <QFormLayout>
#include <QLabel>

void WarpPerspectiveModel::calculate()
{
	if (_originalImage.data) {
		int w = _width->value();
		int h = _height->value();
		cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(_srcPoints, _dstPoints);
		cv::warpPerspective(_originalImage, _warpedImage, perspectiveMatrix, cv::Size(w, h));
	}
}

WarpPerspectiveModel::WarpPerspectiveModel()
{
	_srcPoints.reserve(4);
	_dstPoints.reserve(4);
	_widget = new QWidget();
	_width = new QSpinBox();
	_height = new QSpinBox();
	_width->setRange(1, 10000);
	_height->setRange(1, 10000);
	_width->setValue(300);
	_height->setValue(300);
	QFormLayout* layout = new QFormLayout();
	layout->addRow(new QLabel("输出宽度"), _width);
	layout->addRow(new QLabel("输出高度"), _height);
	_widget->setLayout(layout);
}

unsigned int WarpPerspectiveModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType WarpPerspectiveModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Points", "原图四点" };
		else
			return QtNodes::NodeDataType{ "Points", "目标四点" };
	}
	else {
		return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
}

void WarpPerspectiveModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto pointsData = std::dynamic_pointer_cast<PointsData>(nodeData);
			_srcPoints.clear();
			_srcPoints = pointsData->get();
		}
		else if (portIndex == 2) {
			auto pointsData = std::dynamic_pointer_cast<PointsData>(nodeData);
			_dstPoints.clear();
			_dstPoints = pointsData->get();
		}
	}
	else {
		if (portIndex == 0) {
			_warpedImage.release();
		}
	}
}

std::shared_ptr<QtNodes::NodeData> WarpPerspectiveModel::outData(QtNodes::PortIndex port)
{
	if (_warpedImage.data)
		return std::make_shared<ImageData>(_warpedImage);
	else
		return nullptr;
}
