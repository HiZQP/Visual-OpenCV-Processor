#include "ImageImbilateralFilter.h"

#include <QGridLayout>
#include <QLabel>

void ImageImbilateralFilter::calculate()
{
	int d = _d->value();
	if (d % 2 == 0)
		d += 1;
	_d->setValue(d);
	double sigmaColor = _sigmaColor->value();
	double sigmaSpace = _sigmaSpace->value();
	if (_originalImage.data) {
		cv::bilateralFilter(_originalImage, _blurredImage, d, sigmaColor, sigmaSpace);
		Q_EMIT dataUpdated(0);
	}
}

ImageImbilateralFilter::ImageImbilateralFilter()
{
	_widget = new QWidget();
	_d = new QSpinBox();
	_d->setValue(9);
	_d->setMinimum(1);
	_d->setSingleStep(2); // 只允许奇数
	_sigmaColor = new QDoubleSpinBox();
	_sigmaColor->setValue(75.0);
	_sigmaColor->setMinimum(1.0);
	_sigmaColor->setMaximum(500.0);
	_sigmaColor->setSingleStep(1.0);
	_sigmaSpace = new QDoubleSpinBox();
	_sigmaSpace->setValue(75.0);
	_sigmaSpace->setMinimum(1.0);
	_sigmaSpace->setMaximum(500.0);
	_sigmaSpace->setSingleStep(1.0);
	QLabel* label1 = new QLabel("领域直径：");
	QLabel* label2 = new QLabel("颜色空间标准差：");
	QLabel* label3 = new QLabel("坐标空间标准差：");
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_d, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_sigmaColor, 1, 1);
	layout->addWidget(label3, 2, 0);
	layout->addWidget(_sigmaSpace, 2, 1);
	_widget->setLayout(layout);
	connect(_d, &QSpinBox::editingFinished, this, &ImageImbilateralFilter::calculate);
	connect(_sigmaColor, &QDoubleSpinBox::editingFinished, this, &ImageImbilateralFilter::calculate);
	connect(_sigmaSpace, &QDoubleSpinBox::editingFinished, this, &ImageImbilateralFilter::calculate);
}

unsigned int ImageImbilateralFilter::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageImbilateralFilter::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageImbilateralFilter::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
		calculate();
	}
}

std::shared_ptr<QtNodes::NodeData> ImageImbilateralFilter::outData(QtNodes::PortIndex port)
{
	if (_blurredImage.data)
		return std::make_shared<ImageData>(_blurredImage);
	else
		return nullptr;
}
