#include "DrawCircleModel.h"

#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>

void DrawCircleModel::calculate() {
	if (!_originalImage.empty()) {
		_outputImage = _originalImage.clone();
		int centerX = _centerXSpinBox->value();
		int centerY = _centerYSpinBox->value();
		int radius = _radiusSpinBox->value();
		int thickness = _thicknessSpinBox->value();
		int lineType = _lineTypeComboBox->currentData().toInt();
		cv::Scalar color(_colorRSpinBox->value(), _colorGSpinBox->value(), _colorBSpinBox->value());
		cv::circle(_outputImage, cv::Point(centerX, centerY), radius, color, thickness, lineType);
		Q_EMIT dataUpdated(0);
	}
}

DrawCircleModel::DrawCircleModel()
{
	_widget = new QWidget();
	_centerXSpinBox = new QSpinBox();
	_centerXSpinBox->setRange(0, 10000);
	_centerXSpinBox->setValue(100);
	_centerYSpinBox = new QSpinBox();
	_centerYSpinBox->setRange(0, 10000);
	_centerYSpinBox->setValue(100);
	_radiusSpinBox = new QSpinBox();
	_radiusSpinBox->setRange(1, 10000);
	_radiusSpinBox->setValue(50);
	_thicknessSpinBox = new QSpinBox();
	_thicknessSpinBox->setRange(1, 100);
	_thicknessSpinBox->setValue(1);
	_lineTypeComboBox = new QComboBox();
	_lineTypeComboBox->addItem("8连接线", cv::LINE_8);
	_lineTypeComboBox->addItem("4连接线", cv::LINE_4);
	_lineTypeComboBox->addItem("抗锯齿线", cv::LINE_AA);
	_lineTypeComboBox->setCurrentIndex(0);
	_colorBSpinBox = new QSpinBox();
	_colorBSpinBox->setRange(0, 255);
	_colorBSpinBox->setValue(255);
	_colorGSpinBox = new QSpinBox();
	_colorGSpinBox->setRange(0, 255);
	_colorGSpinBox->setValue(0);
	_colorRSpinBox = new QSpinBox();
	_colorRSpinBox->setRange(0, 255);
	_colorRSpinBox->setValue(0);
	QHBoxLayout* centerLayout = new QHBoxLayout();
	centerLayout->addWidget(new QLabel("X"));
	centerLayout->addWidget(_centerXSpinBox);
	centerLayout->addWidget(new QLabel("Y"));
	centerLayout->addWidget(_centerYSpinBox);
	QHBoxLayout* radiusLayout = new QHBoxLayout();
	radiusLayout->addWidget(_radiusSpinBox);
	QHBoxLayout* thicknessLayout = new QHBoxLayout();
	thicknessLayout->addWidget(_thicknessSpinBox);
	QHBoxLayout* colorLayout = new QHBoxLayout();
	colorLayout->addWidget(new QLabel("B"));
	colorLayout->addWidget(_colorBSpinBox);
	colorLayout->addWidget(new QLabel("G"));
	colorLayout->addWidget(_colorGSpinBox);
	colorLayout->addWidget(new QLabel("R"));
	colorLayout->addWidget(_colorRSpinBox);
	QFormLayout* layout = new QFormLayout();
	layout->addRow("圆心坐标", centerLayout);
	layout->addRow("半径", radiusLayout);
	layout->addRow("线条粗细", thicknessLayout);
	layout->addRow("线条颜色", colorLayout);
	layout->addRow("线条类型", _lineTypeComboBox);
	_widget->setLayout(layout);
	connect(_centerXSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_centerYSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_radiusSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_thicknessSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_lineTypeComboBox, &QComboBox::currentTextChanged, this, &DrawCircleModel::calculate);
	connect(_colorBSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_colorGSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
	connect(_colorRSpinBox, &QSpinBox::editingFinished, this, &DrawCircleModel::calculate);
}

unsigned int DrawCircleModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 4;
	else
		return 1;
}

QtNodes::NodeDataType DrawCircleModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Point", "圆心" };
		else if (portIndex == 2)
			return QtNodes::NodeDataType{ "Number", "半径" };
		else if (portIndex == 3)
			return QtNodes::NodeDataType{ "Scalar", "颜色" };
	}
	else {
		return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
}

void DrawCircleModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto pointData = std::dynamic_pointer_cast<PointData>(nodeData);
			cv::Point point = pointData->get();
			_centerXSpinBox->setValue(point.x);
			_centerYSpinBox->setValue(point.y);
			_centerXSpinBox->setEnabled(false);
			_centerYSpinBox->setEnabled(false);
			calculate();
		}
		else if (portIndex == 2) {
			auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int radius = static_cast<int>(numData->get());
			_radiusSpinBox->setValue(radius);
			_radiusSpinBox->setEnabled(false);
			calculate();
		}
		else if (portIndex == 3) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			cv::Scalar color = scalarData->get();
			_colorBSpinBox->setValue(static_cast<int>(color[0]));
			_colorGSpinBox->setValue(static_cast<int>(color[1]));
			_colorRSpinBox->setValue(static_cast<int>(color[2]));
			_colorBSpinBox->setEnabled(false);
			_colorGSpinBox->setEnabled(false);
			_colorRSpinBox->setEnabled(false);
			calculate();
		}
	}
	else if (portIndex == 0) {
		_originalImage.release();
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
	else if (portIndex == 1) {
		_centerXSpinBox->setEnabled(true);
		_centerYSpinBox->setEnabled(true);
	}
	else if (portIndex == 2) {
		_radiusSpinBox->setEnabled(true);
	}
	else if (portIndex == 3) {
		_colorBSpinBox->setEnabled(true);
		_colorGSpinBox->setEnabled(true);
		_colorRSpinBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> DrawCircleModel::outData(QtNodes::PortIndex port)
{
	if (_outputImage.empty())
		return nullptr;
	return std::make_shared<ImageData>(_outputImage);
}
