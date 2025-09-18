#include "ImageRotateModel.h"
#include <QGridLayout>
#include <QLabel>

void ImageRotateModel::calculate()
{
	double angle = _angle->text().toDouble();
	if (_originalImage.data) {
		cv::Point2f center((float)(_originalImage.cols / 2), (float)(_originalImage.rows / 2));
		cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
		cv::warpAffine(_originalImage, _rotatedImage, rot, _originalImage.size());
	}
	Q_EMIT dataUpdated(0);
}

ImageRotateModel::ImageRotateModel()
{
	_widget = new QWidget();
	_angle = new QLineEdit("0");
	QLabel* label1 = new QLabel("旋转角度（度）：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_angle, 0, 1);
	_widget->setLayout(layout);
	connect(_angle, &QLineEdit::editingFinished, [this]() {
		calculate();
		Q_EMIT dataUpdated(0);
		});
}

unsigned int ImageRotateModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else
		return 1;
}

QtNodes::NodeDataType ImageRotateModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "输入图像" };
		else
			return QtNodes::NodeDataType{ "Number", "角度"};
	else
		return QtNodes::NodeDataType{ "Image", "输出图像" };
}

void ImageRotateModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	static bool isValid = false;

	if (portIndex == 0) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imageData) {
			_originalImage = imageData->get();
			calculate();
		}
	}
	else {
		auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
		isValid = (numberData != nullptr);
		if (isValid) {
			_angle->setReadOnly(true);
			_angle->setStyleSheet("QLineEdit{ color: gray; }");
			_angle->setText(QString::number(numberData->get()));
			calculate();
		}else{
			_angle->setReadOnly(false);
			_angle->setStyleSheet("");
		}
	}
}

std::shared_ptr<QtNodes::NodeData> ImageRotateModel::outData(QtNodes::PortIndex port)
{
	if (_rotatedImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_rotatedImage);
}