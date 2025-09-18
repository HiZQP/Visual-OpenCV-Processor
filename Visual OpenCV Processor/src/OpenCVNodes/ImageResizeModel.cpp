#include "ImageResizeModel.h"
#include "NodeDataType.h"

#include <QGridLayout>

void ImageResizeModel::calculate()
{
	int width = _width->text().toInt();
	int height = _height->text().toInt();
	int interpolation = _interpolation->currentData().toInt();
	if (_originalImage.data) {
		cv::resize(_originalImage, _resizedImage, cv::Size(width, height), 0, 0, interpolation);
		Q_EMIT dataUpdated(0);
	}
}

ImageResizeModel::ImageResizeModel()
{
	_widget = new QWidget();
	_width = new QLineEdit("640");
	_height = new QLineEdit("480");
	_interpolation = new QComboBox();
	_interpolation->addItem("最近邻插值", cv::INTER_NEAREST);
	_interpolation->addItem("双线性插值", cv::INTER_LINEAR);
	_interpolation->addItem("双三次插值", cv::INTER_CUBIC);
	_interpolation->addItem("区域插值", cv::INTER_AREA);
	_interpolation->addItem("Lanczos插值", cv::INTER_LANCZOS4);
	QLabel* label1 = new QLabel("宽（cols）：");
	QLabel* label2 = new QLabel("高（rows）：");
	QLabel* label3 = new QLabel("插值方式：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_width, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_height, 1, 1);
	layout->addWidget(label3, 2, 0);
	layout->addWidget(_interpolation, 2, 1);
	_widget->setLayout(layout);

	connect(_width, &QLineEdit::editingFinished, this, &ImageResizeModel::calculate);
	connect(_height, &QLineEdit::editingFinished, this, &ImageResizeModel::calculate);
	connect(_interpolation, &QComboBox::currentTextChanged, this, &ImageResizeModel::calculate);
}

unsigned int ImageResizeModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType ImageResizeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Number", "宽" };
		else
			return QtNodes::NodeDataType{ "Number", "高" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageResizeModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (portIndex == 0) {
		auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imgData) {
			_originalImage = imgData->get();
			calculate();
		}
		else {
			_originalImage.release();
			_resizedImage.release();
			Q_EMIT dataUpdated(0);
		}
	}
	else if (portIndex == 1) {
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
	else if (portIndex == 2) {
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		static bool isValid = false;
		isValid = (numberData != nullptr);
		if (isValid) {
			_height->setReadOnly(true);
			_height->setStyleSheet("QLineEdit { color: gray; }");
			_height->setText(QString::number(numberData->get()));
			calculate();
		}
		else {
			_height->setReadOnly(false);
			_height->setStyleSheet("");
		}
	}
}

std::shared_ptr<QtNodes::NodeData> ImageResizeModel::outData(QtNodes::PortIndex port)
{
	if (_resizedImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_resizedImage);
}
