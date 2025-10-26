#include "HoughCirclesNode.h"

void HoughCirclesNode::calculate()
{
	if (_inputImage.empty()) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：输入图像");
		_circles.clear();
		return;
	}
	if(_inputImage.channels() != 1){
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("错误：需要单通道灰度图像");
		_circles.clear();
		return;
	}
	double dp = _ui.dp_doubleSpinBox->value();
	int minDist = _ui.minD_spinBox->value();
	int p1 = _ui.p1_spinBox->value();
	int p2 = _ui.p2_spinBox->value();
	int minR = _ui.minR_spinBox->value();
	int maxR = _ui.maxR_spinBox->value();
	QElapsedTimer timer;

	timer.start();
	cv::HoughCircles(_inputImage, _circles, cv::HOUGH_GRADIENT, dp, minDist, p1, p2, minR, maxR);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("%1 已检测 %2 ms").arg(static_cast<int>(_circles.size())).arg(elapsed / 1e6, 0, 'f', 2));\

	emit dataUpdated(0);
}

HoughCirclesNode::HoughCirclesNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：输入图像");

	connect(_ui.dp_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
	connect(_ui.minD_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
	connect(_ui.p1_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
	connect(_ui.p2_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
	connect(_ui.minR_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
	connect(_ui.maxR_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughCirclesNode::calculate);
}

unsigned int HoughCirclesNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType HoughCirclesNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Vector", "圆集" };
}

void HoughCirclesNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto inData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_inputImage = inData->get();
	}else {
		_inputImage.release();
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> HoughCirclesNode::outData(QtNodes::PortIndex port)
{
	if(_circles.empty())
		return nullptr;
	return std::make_shared<VectorData>(_circles);
}
