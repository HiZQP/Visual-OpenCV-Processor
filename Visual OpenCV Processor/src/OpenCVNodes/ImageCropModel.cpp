#include "ImageCropModel.h"

#include <QGridLayout>
#include <QLabel>



void ImageCropModel::calculate()
{
	if (!_originalImage.empty())
	{
		int x = std::max(0, std::min(_originalImage.cols - 1, _x->text().toInt()));
		int y = std::max(0, std::min(_originalImage.rows - 1, _y->text().toInt()));
		int w = std::max(1, std::min(_originalImage.cols - x, _width->text().toInt()));
		int h = std::max(1, std::min(_originalImage.rows - y, _height->text().toInt()));
		_croppedImage = _originalImage(cv::Rect(x, y, w, h)).clone();
		Q_EMIT dataUpdated(0);
	}
}

ImageCropModel::ImageCropModel()
{
	_widget = new QWidget();
	_x = new QLineEdit("0");
	_y = new QLineEdit("0");
	_width = new QLineEdit("100");
	_height = new QLineEdit("100");
	auto layout = new QGridLayout();
	layout->addWidget(new QLabel("左上坐标X："), 0, 0);
	layout->addWidget(_x, 0, 1);
	layout->addWidget(new QLabel("左上坐标Y："), 1, 0);
	layout->addWidget(_y, 1, 1);
	layout->addWidget(new QLabel(QUTF8("宽：")), 2, 0);
	layout->addWidget(_width, 2, 1);
	layout->addWidget(new QLabel(QUTF8("高：")), 3, 0);
	layout->addWidget(_height, 3, 1);
	_widget->setLayout(layout);
	connect(_x, &QLineEdit::editingFinished, this, &ImageCropModel::calculate);
	connect(_y, &QLineEdit::editingFinished, this, &ImageCropModel::calculate);
	connect(_width, &QLineEdit::editingFinished, this, &ImageCropModel::calculate);
	connect(_height, &QLineEdit::editingFinished, this, &ImageCropModel::calculate);
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
	if (portIndex == 0)
	{
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imageData)
		{
			_originalImage = imageData->get();
			calculate();
		}else
		{
			_originalImage.release();
			_croppedImage.release();
			Q_EMIT dataUpdated(0);
		}
	}
	else if (portIndex == 1)
	{
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		static bool isValid = false;
		isValid = (numberData != nullptr);
		if (isValid) {
			_x->setReadOnly(true);
			_x->setStyleSheet("QLineEdit{ color: gray; }");
			_x->setText(QString::number(numberData->get()));
			calculate();
		}
		else {
			_x->setReadOnly(false);
			_x->setStyleSheet("");
		}
	}
	else if (portIndex == 2)
	{
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		static bool isValid = false;
		isValid = (numberData != nullptr);
		if (isValid) {
			_y->setReadOnly(true);
			_y->setStyleSheet("QLineEdit{ color: gray; }");
			_y->setText(QString::number(numberData->get()));
			calculate();
		}
		else {
			_y->setReadOnly(false);
			_y->setStyleSheet("");
		}
	}
	else if (portIndex == 3)
	{
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		static bool isValid = false;
		isValid = (numberData != nullptr);
		if (isValid) {
			_width->setReadOnly(true);
			_width->setStyleSheet("QLineEdit{ color: gray; }");
			_width->setText(QString::number(numberData->get()));
			calculate();
		}
		else {
			_width->setReadOnly(false);
			_width->setStyleSheet("");
		}
	}
	else if (portIndex == 4)
	{
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		static bool isValid = false;
		isValid = (numberData != nullptr);
		if (isValid) {
			_height->setReadOnly(true);
			_height->setStyleSheet("QLineEdit{ color: gray; }");
			_height->setText(QString::number(numberData->get()));
			calculate();
		}
		else {
			_height->setReadOnly(false);
			_height->setStyleSheet("");
		}
	}
}

std::shared_ptr<QtNodes::NodeData> ImageCropModel::outData(QtNodes::PortIndex port)
{
	return std::make_shared<ImageData>(_croppedImage);
}

