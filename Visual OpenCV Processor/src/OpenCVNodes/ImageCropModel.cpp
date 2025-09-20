#include "ImageCropModel.h"

#include <QGridLayout>
#include <QLabel>



void ImageCropModel::calculate()
{
	int x = std::max(0, std::min(_originalImage.cols - 1, _x->value()));
	int y = std::max(0, std::min(_originalImage.rows - 1, _y->value()));
	int w = std::max(1, std::min(_originalImage.cols - x, _width->value()));
	int h = std::max(1, std::min(_originalImage.rows - y, _height->value()));
	_croppedImage = _originalImage(cv::Rect(x, y, w, h)).clone();
	Q_EMIT dataUpdated(0);
}

ImageCropModel::ImageCropModel()
{
	_widget = new QWidget();
	_x = new QSpinBox();
	_y = new QSpinBox();
	_width = new QSpinBox();
	_height = new QSpinBox();
	auto layout = new QGridLayout();
	layout->addWidget(new QLabel("左上坐标X："), 0, 0);
	layout->addWidget(_x, 0, 1);
	layout->addWidget(new QLabel("左上坐标Y："), 1, 0);
	layout->addWidget(_y, 1, 1);
	layout->addWidget(new QLabel("宽："), 2, 0);
	layout->addWidget(_width, 2, 1);
	layout->addWidget(new QLabel("高："), 3, 0);
	layout->addWidget(_height, 3, 1);
	_widget->setLayout(layout);
	connect(_x, &QSpinBox::editingFinished, this, &ImageCropModel::calculate);
	connect(_y, &QSpinBox::editingFinished, this, &ImageCropModel::calculate);
	connect(_width, &QSpinBox::editingFinished, this, &ImageCropModel::calculate);
	connect(_height, &QSpinBox::editingFinished, this, &ImageCropModel::calculate);
}

unsigned int ImageCropModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 5;
	else
		return 1;
}

QtNodes::NodeDataType ImageCropModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Number", "坐标X" };
		else if (portIndex == 2)
			return QtNodes::NodeDataType{ "Number", "坐标Y" };
		else if (portIndex == 3)
			return QtNodes::NodeDataType{ "Number", "宽" };
		else
			return QtNodes::NodeDataType{ "Number", "高" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageCropModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			_width->setMaximum(_originalImage.cols);
			_height->setMaximum(_originalImage.rows);
			calculate();
		}
		else if (portIndex == 1) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			_x->setValue(static_cast<int>(numberData->get()));
			_x->setReadOnly(true);
			calculate();
		}
		else if (portIndex == 2) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			_y->setValue(static_cast<int>(numberData->get()));
			_y->setReadOnly(true);
			calculate();
		}
		else if (portIndex == 3) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			_width->setValue(static_cast<int>(numberData->get()));
			_width->setReadOnly(true);
			calculate();
		}
		else if (portIndex == 4) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			_height->setValue(static_cast<int>(numberData->get()));
			_height->setReadOnly(true);
			calculate();
		}
	}
	else if (portIndex == 0)
	{
		_originalImage.release();
		_croppedImage.release();
		Q_EMIT dataUpdated(0);
	}
	else if (portIndex == 1) {
		_x->setReadOnly(false);
	}
	else if (portIndex == 2) {
		_y->setReadOnly(false);
	}
	else if (portIndex == 3) {
		_width->setReadOnly(false);
	}
	else if (portIndex == 4) {
		_height->setReadOnly(false);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageCropModel::outData(QtNodes::PortIndex port)
{
	if (_croppedImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_croppedImage);
}

