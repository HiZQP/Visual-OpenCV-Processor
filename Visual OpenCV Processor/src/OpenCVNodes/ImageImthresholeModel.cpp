#include "ImageImthresholeModel.h"

#include <QGridLayout>
#include <QLabel>

void ImageImthresholeModel::calculate()
{
	int thresh = _thresh->value();
	int maxval = _maxval->value();
	int type = _type->currentData().toInt();
	if (_originalImage.data) {
		cv::threshold(_originalImage, _threshImage, thresh, maxval, type);
		Q_EMIT dataUpdated(0);
	}
}

ImageImthresholeModel::ImageImthresholeModel()
{
	_widget = new QWidget();
	_thresh = new QSpinBox();
	_thresh->setValue(127);
	_thresh->setMinimum(0);
	_thresh->setMaximum(255);
	_maxval = new QSpinBox();
	_maxval->setValue(255);
	_maxval->setMinimum(1);
	_maxval->setMaximum(255);
	_type = new QComboBox();
	_type->addItem("二值化", cv::THRESH_BINARY);
	_type->addItem("反二值化", cv::THRESH_BINARY_INV);
	_type->addItem("截断", cv::THRESH_TRUNC);
	_type->addItem("零值化", cv::THRESH_TOZERO);
	_type->addItem("反零值化", cv::THRESH_TOZERO_INV);
	QLabel* label1 = new QLabel("阈值：");
	QLabel* label2 = new QLabel("最大值：");
	QLabel* label3 = new QLabel("类型：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_thresh, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_maxval, 1, 1);
	layout->addWidget(label3, 2, 0);
	layout->addWidget(_type, 2, 1);
	_widget->setLayout(layout);
	connect(_thresh, &QSpinBox::editingFinished, this, &ImageImthresholeModel::calculate);
	connect(_maxval, &QSpinBox::editingFinished, this, &ImageImthresholeModel::calculate);
	connect(_type, &QComboBox::currentTextChanged, this, &ImageImthresholeModel::calculate);
}

unsigned int ImageImthresholeModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageImthresholeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageImthresholeModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
}

std::shared_ptr<QtNodes::NodeData> ImageImthresholeModel::outData(QtNodes::PortIndex port)
{
	if (_threshImage.data)
		return std::make_shared<ImageData>(_threshImage);
	else
		return nullptr;
}
