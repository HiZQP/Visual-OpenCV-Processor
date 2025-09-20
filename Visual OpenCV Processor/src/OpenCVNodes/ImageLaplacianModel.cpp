#include "ImageLaplacianModel.h"

#include <QFormLayout>
#include <QLabel>

void ImageLaplacianModel::calculate() {
	int ksize = _ksize->value();
	int scale = _scale->currentData().toInt();
	int delta = _delta->currentData().toInt();
	int ddepth = _ddepth->currentData().toInt();
	cv::Laplacian(_originalImage, _laplacianImage, ddepth, ksize, scale, delta);
	Q_EMIT dataUpdated(0);
}

ImageLaplacianModel::ImageLaplacianModel()
{
	_widget = new QWidget();
	QFormLayout* layout = new QFormLayout();
	_ksize = new QSpinBox();
	_ksize->setRange(1, 31);
	_ksize->setSingleStep(2);
	_ksize->setValue(3);
	layout->addRow("Laplacian算子大小", _ksize);
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

unsigned int ImageLaplacianModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageLaplacianModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageLaplacianModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_laplacianImage.release();
		Q_EMIT dataInvalidated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageLaplacianModel::outData(QtNodes::PortIndex port)
{
	if(_laplacianImage.data)
		return std::make_shared<ImageData>(_laplacianImage);
	else
		return nullptr;
}
