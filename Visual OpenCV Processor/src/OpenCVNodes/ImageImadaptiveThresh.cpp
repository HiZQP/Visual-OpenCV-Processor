#include "ImageImadaptiveThresh.h"

#include <QGridLayout>
#include <QLabel>

void ImageImadaptiveThresh::calculate()
{
	int method = _method->currentData().toInt();
	int type = _type->currentData().toInt();
	int blockSize = _blockSize->value();
	if (blockSize % 2 == 0)
		blockSize += 1;
	_blockSize->setValue(blockSize);
	int C = _C->value();
	if (_originalImage.data) {
		cv::adaptiveThreshold(_originalImage, _threshImage, 255, method, type, blockSize, C);
		Q_EMIT dataUpdated(0);
	}
}

ImageImadaptiveThresh::ImageImadaptiveThresh()
{
	_widget = new QWidget();
	_method = new QComboBox();
	_method->addItem("均值法", cv::ADAPTIVE_THRESH_MEAN_C);
	_method->addItem("高斯法", cv::ADAPTIVE_THRESH_GAUSSIAN_C);
	_type = new QComboBox();
	_type->addItem("二值化", cv::THRESH_BINARY);
	_type->addItem("反二值化", cv::THRESH_BINARY_INV);
	_blockSize = new QSpinBox();
	_blockSize->setValue(11);
	_blockSize->setMinimum(3);
	_blockSize->setSingleStep(2); // 只允许奇数
	_C = new QSpinBox();
	_C->setValue(2);
	_C->setMinimum(-100);
	_C->setMaximum(100);
	_info = new QLabel("无输入");
	QLabel* label1 = new QLabel("自适应方法：");
	QLabel* label2 = new QLabel("阈值类型：");
	QLabel* label3 = new QLabel("领域大小：");
	QLabel* label4 = new QLabel("常数C：");
	QVBoxLayout* vLayout = new QVBoxLayout();
	QGridLayout* layout = new QGridLayout();
	layout->addWidget(label1, 0, 0);
	layout->addWidget(_method, 0, 1);
	layout->addWidget(label2, 1, 0);
	layout->addWidget(_type, 1, 1);
	layout->addWidget(label3, 2, 0);
	layout->addWidget(_blockSize, 2, 1);
	layout->addWidget(label4, 3, 0);
	layout->addWidget(_C, 3, 1);
	vLayout->addLayout(layout);
	vLayout->addWidget(_info);
	_widget->setLayout(vLayout);
	connect(_method, &QComboBox::currentTextChanged, this, &ImageImadaptiveThresh::calculate);
	connect(_type, &QComboBox::currentTextChanged, this, &ImageImadaptiveThresh::calculate);
	connect(_blockSize, &QSpinBox::editingFinished, this, &ImageImadaptiveThresh::calculate);
	connect(_C, &QSpinBox::editingFinished, this, &ImageImadaptiveThresh::calculate);
}

unsigned int ImageImadaptiveThresh::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageImadaptiveThresh::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageImadaptiveThresh::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	// 验证是否为单通道8位图像
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		QString infoText;
		cv::Mat image = imageData->get();

		if (image.channels() != 1 || image.depth() != CV_8U) {
			QString info = "不是单通道8位图像 ";
			infoText += info;
			_info->setText(infoText);
			_threshImage.release();
			Q_EMIT dataInvalidated(0);
			return;
		} else{
			_info->setText("输入合法");
		}

		_originalImage = imageData->get();
		calculate();
	}
	else {
		_originalImage.release();
		_threshImage.release();
		_info->setText("无输入");
		Q_EMIT dataInvalidated(0);
	}

}

std::shared_ptr<QtNodes::NodeData> ImageImadaptiveThresh::outData(QtNodes::PortIndex port)
{
	if (_threshImage.data)
		return std::make_shared<ImageData>(_threshImage);
	else
		return nullptr;
}
