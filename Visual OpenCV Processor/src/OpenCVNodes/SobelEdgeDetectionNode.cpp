#include "SobelEdgeDetectionNode.h"

void SobelEdgeDetectionNode::calculate() {
	if (!_originalImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：图像输入 ");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	if (_originalImage.channels() != 1) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("警告：需要单通道灰度图像！");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}
	
	QElapsedTimer timer;

	timer.start();
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;
	int ksize = _ui.ksize_spinBox->value();
	if (ksize % 2 == 0)
		ksize += 1; // Sobel算子大小必须为奇数
	int scale = _ui.scale_spinBox->value();
	int delta = _ui.delta_spinBox->value();
	Sobel(_originalImage, grad_x, CV_16S, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
	Sobel(_originalImage, grad_y, CV_16S, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);
	// 转换回 CV_8U
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, _outputImage);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));

	Q_EMIT dataUpdated(0);
}

SobelEdgeDetectionNode::SobelEdgeDetectionNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：图像输入 ");

	connect(_ui.ksize_spinBox, &QSpinBox::valueChanged, this, &SobelEdgeDetectionNode::calculate);
	connect(_ui.scale_spinBox, &QSpinBox::valueChanged, this, &SobelEdgeDetectionNode::calculate);
	connect(_ui.delta_spinBox, &QSpinBox::valueChanged, this, &SobelEdgeDetectionNode::calculate);
}

unsigned int SobelEdgeDetectionNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType SobelEdgeDetectionNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void SobelEdgeDetectionNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
	}
	else {
		_originalImage.release();
		_outputImage.release();
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> SobelEdgeDetectionNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else
		return nullptr;
}
