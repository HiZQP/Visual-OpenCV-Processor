#include "ImageThresholdModel.h"

#include <QFormLayout>
#include <QLabel>


void ImageThresholdModel::calculate() {
	if (_originalImage.data) {
		int threshold = _thresholdSpinBox->value();
		int maxValue = _maxValueSpinBox->value();
		int type = _typeComboBox->currentData().toInt();
		if (_originalImage.channels() == 3)
			cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGR2GRAY);
		else if (_originalImage.channels() == 4)
			cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGRA2GRAY);
		cv::threshold(_originalImage, _outputImage, threshold, maxValue, type);
		Q_EMIT dataUpdated(0);
	}
	else {
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
}

ImageThresholdModel::ImageThresholdModel()
{
	_widget = new QWidget();
	_thresholdSpinBox = new QSpinBox();
	_thresholdSpinBox->setRange(0, 255);
	_thresholdSpinBox->setValue(255);
	_maxValueSpinBox = new QSpinBox();
	_maxValueSpinBox->setRange(0, 255);
	_maxValueSpinBox->setValue(0);
	_typeComboBox = new QComboBox();
	_typeComboBox->addItem("二值化", cv::THRESH_BINARY);
	_typeComboBox->addItem("反二值化", cv::THRESH_BINARY_INV);
	_typeComboBox->addItem("截断", cv::THRESH_TRUNC);
	_typeComboBox->addItem("零值化", cv::THRESH_TOZERO);
	_typeComboBox->addItem("反零值化", cv::THRESH_TOZERO_INV);
	_typeComboBox->setCurrentIndex(0);
	QFormLayout* layout = new QFormLayout();
	layout->addRow("阈值", _thresholdSpinBox);
	layout->addRow("最大值", _maxValueSpinBox);
	layout->addRow("阈值类型", _typeComboBox);
	_widget->setLayout(layout);
	connect(_thresholdSpinBox, &QSpinBox::editingFinished, this, &ImageThresholdModel::calculate);
	connect(_maxValueSpinBox, &QSpinBox::editingFinished, this, &ImageThresholdModel::calculate);
	connect(_typeComboBox, &QComboBox::currentTextChanged, this, &ImageThresholdModel::calculate);
}

unsigned int ImageThresholdModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType ImageThresholdModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Number", "阈值" };
		else
			return QtNodes::NodeDataType{ "Number", "最大值" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageThresholdModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imgData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int threshold = static_cast<int>(numData->get());
			if (threshold < 0) threshold = 0;
			if (threshold > 255) threshold = 255;
			_thresholdSpinBox->setValue(threshold);
			_thresholdSpinBox->setEnabled(false);
		}
		else if (portIndex == 2) {
			auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int threshold = static_cast<int>(numData->get());
			if (threshold < 0) threshold = 0;
			if (threshold > 255) threshold = 255;
			_maxValueSpinBox->setValue(threshold);
			_maxValueSpinBox->setEnabled(false);
		}
	}
	else if (portIndex == 0)
	{
		_originalImage.release();
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
	else if(portIndex == 1){
		_thresholdSpinBox->setEnabled(true);
	}
	else if (portIndex == 2) {
		_maxValueSpinBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageThresholdModel::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else 
		return nullptr;
}
