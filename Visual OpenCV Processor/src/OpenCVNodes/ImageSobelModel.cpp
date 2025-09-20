#include "ImageSobelModel.h"

#include <QFormLayout>
#include <QLabel>

void ImageSobelModel::calculate() {
	int ksize = _ksize->value();
	int scale = _scale->currentData().toInt();
	int delta = _delta->currentData().toInt();
	int ddepth = _ddepth->currentData().toInt();
	if (_originalImage.data) {
		cv::Sobel(_originalImage, _sobelImage, ddepth, 1, 1, ksize, scale, delta);
		Q_EMIT dataUpdated(0);
	}
}

ImageSobelModel::ImageSobelModel()
{
	_widget = new QWidget();
	QFormLayout* layout = new QFormLayout();
	_ksize = new QSpinBox();
	_ksize->setRange(1, 31);
	_ksize->setSingleStep(2);
	_ksize->setValue(3);
	layout->addRow("Sobel算子大小", _ksize);
	connect(_ksize, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) { calculate(); });
	_scale = new QComboBox();
	_scale->addItem("1", 1);
	_scale->addItem("2", 2);
	_scale->addItem("4", 4);
	_scale->addItem("8", 8);
	_scale->setCurrentIndex(0);
	layout->addRow("缩放比例", _scale);
	connect(_scale, &QComboBox::currentTextChanged, this, [this](const QString&) { calculate(); });
	_delta = new QComboBox();
	_delta->addItem("0", 0);
	_delta->addItem("50", 50);
	_delta->addItem("100", 100);
	_delta->addItem("150", 150);
	_delta->setCurrentIndex(0);
	layout->addRow("Delta值", _delta);
	connect(_delta, &QComboBox::currentTextChanged, this, [this](const QString&) { calculate(); });
	_ddepth = new QComboBox();
	_ddepth->addItem("CV_8U", cv::DataType<uint8_t>::depth);
	_ddepth->addItem("CV_16U", cv::DataType<uint16_t>::depth);
	_ddepth->addItem("CV_16S", cv::DataType<int16_t>::depth);
	_ddepth->addItem("CV_32F", cv::DataType<float>::depth);
	_ddepth->addItem("CV_64F", cv::DataType<double>::depth);
	_ddepth->setCurrentIndex(3);
	layout->addRow("目标图像深度", _ddepth);
	connect(_ddepth, &QComboBox::currentTextChanged, this, [this](const QString&) { calculate(); });
	_widget->setLayout(layout);
}

unsigned int ImageSobelModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageSobelModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageSobelModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imageData) {
			_originalImage = imageData->get();
			calculate();
		}
	}
}

std::shared_ptr<QtNodes::NodeData> ImageSobelModel::outData(QtNodes::PortIndex port)
{
	if (_sobelImage.data)
		return std::make_shared<ImageData>(_sobelImage);
	else
		return nullptr;
}
