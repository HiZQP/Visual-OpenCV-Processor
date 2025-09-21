#include "ImageHistEqualizeModel.h"

#include <QVBoxLayout>

void ImageHistEqualizeModel::calculate() {
	if (_originalImage.data) {
		if (_originalImage.channels() == 1) {
			cv::equalizeHist(_originalImage, _equalizedImage);
		}
		else if (_originalImage.channels() == 3) {
			cv::Mat ycrcb;
			cv::cvtColor(_originalImage, ycrcb, cv::COLOR_BGR2YCrCb);
			std::vector<cv::Mat> channels;
			cv::split(ycrcb, channels);
			cv::equalizeHist(channels[0], channels[0]);
			cv::merge(channels, ycrcb);
			cv::cvtColor(ycrcb, _equalizedImage, cv::COLOR_YCrCb2BGR);
		}
		else {
			_infoLabel->setText("仅支持单通道或三通道图像");
			_equalizedImage.release();
			Q_EMIT dataUpdated(0);
			return;
		}
		_infoLabel->setText(QString("图像已处理，宽高：%1 x %2").arg(_equalizedImage.cols).arg(_equalizedImage.rows));
		Q_EMIT dataUpdated(0);
	}
}

ImageHistEqualizeModel::ImageHistEqualizeModel()
{
	_widget = new QWidget();
	_infoLabel = new QLabel;
	_infoLabel->setMinimumSize(200, 30);

	_infoLabel->setText("无图像");
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(_infoLabel);
	_widget->setLayout(layout);
}

unsigned int ImageHistEqualizeModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageHistEqualizeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageHistEqualizeModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_equalizedImage.release();
		_infoLabel->setText("无图像");
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageHistEqualizeModel::outData(QtNodes::PortIndex port)
{
	if (_equalizedImage.data)
		return std::make_shared<ImageData>(_equalizedImage);
	else
		return nullptr;
}
