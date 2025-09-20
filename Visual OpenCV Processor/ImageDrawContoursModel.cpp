#include "ImageDrawContoursModel.h"

#include <QVBoxLayout>
#include <QLabel>

void ImageDrawContoursModel::calculate() {
	if (_originalImage.data && !_contours.empty()) {
		_outputImage = _originalImage.clone();
		if (_drawAll->isChecked())
			cv::drawContours(_outputImage, _contours, -1, cv::Scalar(0, 0, 255), 2);
		else
			cv::drawContours(_outputImage, _contours, _indexSpinBox->value(), cv::Scalar(0, 0, 255), 2);
		Q_EMIT dataUpdated(0);
	}
	else {
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
}

ImageDrawContoursModel::ImageDrawContoursModel()
{
	_widget = new QWidget();
	_drawAll = new QCheckBox("绘制所有轮廓");
	_drawAll->setChecked(false);
	_indexSpinBox = new QSpinBox();
	_indexSpinBox->setRange(0, 1000);
	_indexSpinBox->setValue(0);
	_indexSpinBox->setEnabled(true);
	QLabel* indexLabel = new QLabel("轮廓索引");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(_drawAll);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(indexLabel);
	hLayout->addWidget(_indexSpinBox);
	layout->addLayout(hLayout);
	_widget->setLayout(layout);
	connect(_drawAll, &QCheckBox::checkStateChanged, this, &ImageDrawContoursModel::calculate);
	connect(_indexSpinBox, &QSpinBox::editingFinished, this, &ImageDrawContoursModel::calculate);
}

unsigned int ImageDrawContoursModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType ImageDrawContoursModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Contours", "轮廓输入" };
		else
			return QtNodes::NodeDataType{ "Number", "索引" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageDrawContoursModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			calculate();
		}
		else if(portIndex == 1)
		{
			auto contoursData = std::dynamic_pointer_cast<ContoursData>(nodeData);
			_contours = contoursData->get();
			calculate();
		}
		else if (portIndex == 2)
		{
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int index = static_cast<int>(numberData->get());
			if(index >= static_cast<int>(_contours.size()))
				index = static_cast<int>(_contours.size()) - 1;
			_indexSpinBox->setValue(index);
			_indexSpinBox->setEnabled(false);
			_drawAll->setChecked(false);
			_drawAll->setEnabled(false);
			calculate();
		}
	}else if (portIndex == 2) {
		_drawAll->setEnabled(true);
		_indexSpinBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageDrawContoursModel::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else
		return nullptr;
}
