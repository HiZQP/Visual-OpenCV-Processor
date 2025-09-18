#include "DisplayModel.h"

#include <QVBoxLayout>
#include <QLabel>

TextDisplayModel::TextDisplayModel()
{
	_text = new QLabel;
	_text->setText("无数据");
}

QtNodes::NodeDataType TextDisplayModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	return QtNodes::NodeDataType{ "String", "输入" };
}

void TextDisplayModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto strData = std::dynamic_pointer_cast<StringData>(nodeData);
		if (strData) {
			_text->setText(strData->get());
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		_text->setText("无数据");
		Q_EMIT dataUpdated(0);
	}
}

ImageDisplayModel::ImageDisplayModel()
{
	_size = new QLabel("宽高：");
	_imageLabel = new QLabel;
	_imageLabel->setAlignment(Qt::AlignCenter);
	_imageLabel->setText("无图像");
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(_size);
	layout->addWidget(_imageLabel);
	_widget = new QWidget;
	_widget->setLayout(layout);
	_imageLabel->setMinimumSize(200, 200);
}

QtNodes::NodeDataType ImageDisplayModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	return QtNodes::NodeDataType{ "Image", "输入" };
}

void ImageDisplayModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);

		QString sizeText = QString("宽高：%1 x %2").arg(imageData->get().cols).arg(imageData->get().rows);
		_size->setText(sizeText);
		cv::Mat rgbImage;
		cv::cvtColor(imageData->get(), rgbImage, cv::COLOR_BGR2RGB);
		QImage qimg((const uchar*)rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
		_imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
	else {
		_imageLabel->setText("无图像");
		_size->setText("宽高：");
		Q_EMIT dataUpdated(0);
	}
}