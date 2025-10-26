#include "HoughLinesNode.h"

#include <QFormLayout>
#include <QVBoxLayout>

void HoughLinesNode::calculate() {
	if (_originalImage.data) {
		_lines.clear();

		// 霍夫直线检测参数
		double rho = _rho->value();
		double theta = _theta->value() * CV_PI / 180.0; // 转换为弧度
		int threshold = _threshold->value();
		double minLineLength = _minLineLength->value();
		double maxLineGap = _maxLineGap->value();

		// 执行霍夫直线检测
		cv::HoughLinesP(_originalImage, _lines, rho, theta, threshold, minLineLength, maxLineGap);

		_infoLabel->setText(QString("检测到 %1 条直线").arg(_lines.size()));
		Q_EMIT dataUpdated(0);
	}
}

HoughLinesNode::HoughLinesNode()
{
	_widget = new QWidget();

	// 创建参数控件
	_rho = new QSpinBox();
	_rho->setRange(1, 10);
	_rho->setValue(1);

	_theta = new QDoubleSpinBox();
	_theta->setRange(0.1, 10.0);
	_theta->setSingleStep(0.1);
	_theta->setValue(1.0);

	_threshold = new QSpinBox();
	_threshold->setRange(1, 200);
	_threshold->setValue(50);

	_minLineLength = new QSpinBox();
	_minLineLength->setRange(0, 1000);
	_minLineLength->setValue(0);

	_maxLineGap = new QDoubleSpinBox();
	_maxLineGap->setRange(0.0, 100.0);
	_maxLineGap->setSingleStep(0.1);
	_maxLineGap->setValue(0.0);

	_infoLabel = new QLabel("无输入");

	// 布局
	QFormLayout* layout = new QFormLayout();
	layout->addRow("距离分辨率(rho)", _rho);
	layout->addRow("角度分辨率(theta)", _theta);
	layout->addRow("累加器阈值", _threshold);
	layout->addRow("最小线段长度", _minLineLength);
	layout->addRow("最大线段间隙", _maxLineGap);
	layout->addRow(_infoLabel);
	_widget->setLayout(layout);

	// 连接信号
	connect(_rho, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughLinesNode::calculate);
	connect(_theta, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &HoughLinesNode::calculate);
	connect(_threshold, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughLinesNode::calculate);
	connect(_minLineLength, QOverload<int>::of(&QSpinBox::valueChanged), this, &HoughLinesNode::calculate);
	connect(_maxLineGap, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &HoughLinesNode::calculate);
}

unsigned int HoughLinesNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType HoughLinesNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Vector", "线段集" };
}

void HoughLinesNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imageData->get().channels() == 1) {
			_originalImage = imageData->get();
			calculate();
		}
		else {
			_infoLabel->setText("需要单通道二值图像");
			_originalImage.release();
			_lines.clear();
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		_infoLabel->setText("无输入");
		_originalImage.release();
		_lines.clear();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> HoughLinesNode::outData(QtNodes::PortIndex port)
{
	if (!_lines.empty()) {
		return std::make_shared<VectorData>(_lines);
	}
	else {
		return nullptr;
	}
}
