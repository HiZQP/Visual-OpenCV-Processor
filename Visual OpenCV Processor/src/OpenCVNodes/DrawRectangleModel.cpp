#include "DrawRectangleModel.h"

#include <QFormLayout>
#include <QLabel>

void DrawRectangleModel::calculate() {
	if (!_originalImage.empty()) {
		_outputImage = _originalImage.clone();
		int x1 = _x1SpinBox->value();
		int y1 = _y1SpinBox->value();
		int x2 = _x2SpinBox->value();
		int y2 = _y2SpinBox->value();
		int thickness = _thicknessSpinBox->value();
		int lineType = _lineTypeComboBox->currentData().toInt();
		cv::Scalar color(_colorBSpinBox->value(), _colorGSpinBox->value(), _colorRSpinBox->value());
		cv::rectangle(_outputImage, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness, lineType);
		Q_EMIT dataUpdated(0);
	}
}

DrawRectangleModel::DrawRectangleModel()
{
	_widget = new QWidget();
	_x1SpinBox = new QSpinBox();
	_x1SpinBox->setRange(0, 10000);
	_x1SpinBox->setValue(0);
	_y1SpinBox = new QSpinBox();
	_y1SpinBox->setRange(0, 10000);
	_y1SpinBox->setValue(0);
	_x2SpinBox = new QSpinBox();
	_x2SpinBox->setRange(0, 10000);
	_x2SpinBox->setValue(100);
	_y2SpinBox = new QSpinBox();
	_y2SpinBox->setRange(0, 10000);
	_y2SpinBox->setValue(100);
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
	QHBoxLayout* startLayout = new QHBoxLayout();
	startLayout->addWidget(new QLabel("X"));
	startLayout->addWidget(_x1SpinBox);
	startLayout->addWidget(new QLabel("Y"));
	startLayout->addWidget(_y1SpinBox);
	QHBoxLayout* endLayout = new QHBoxLayout();
	endLayout->addWidget(new QLabel("X"));
	endLayout->addWidget(_x2SpinBox);
	endLayout->addWidget(new QLabel("Y"));
	endLayout->addWidget(_y2SpinBox);
	QHBoxLayout* colorLayout = new QHBoxLayout();
	colorLayout->addWidget(new QLabel("B"));
	colorLayout->addWidget(_colorBSpinBox);
	colorLayout->addWidget(new QLabel("G"));
	colorLayout->addWidget(_colorGSpinBox);
	colorLayout->addWidget(new QLabel("R"));
	colorLayout->addWidget(_colorRSpinBox);
	QFormLayout* layout = new QFormLayout();
	layout->addRow("起点坐标", startLayout);
	layout->addRow("终点坐标", endLayout);
	layout->addRow("线条颜色", colorLayout);
	layout->addRow("线条粗细", _thicknessSpinBox);
	layout->addRow("线条类型", _lineTypeComboBox);
	_widget->setLayout(layout);
	connect(_x1SpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_y1SpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_x2SpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_y2SpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_thicknessSpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_lineTypeComboBox, &QComboBox::currentTextChanged, this, &DrawRectangleModel::calculate);
	connect(_colorBSpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_colorGSpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
	connect(_colorRSpinBox, &QSpinBox::editingFinished, this, &DrawRectangleModel::calculate);
}

unsigned int DrawRectangleModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 4;
	else
		return 1;
}

QtNodes::NodeDataType DrawRectangleModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Point", "起点" };
		else if (portIndex == 2)
			return QtNodes::NodeDataType{ "Point", "终点" };
		else if (portIndex == 3)
			return QtNodes::NodeDataType{ "Scalar", "颜色" };
	}
	else {
		return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
}

void DrawRectangleModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
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
			_x1SpinBox->setValue(point.x);
			_y1SpinBox->setValue(point.y);
			_x1SpinBox->setEnabled(false);
			_y1SpinBox->setEnabled(false);
			calculate();
		}
		else if (portIndex == 2) {
			auto pointData = std::dynamic_pointer_cast<PointData>(nodeData);
			cv::Point point = pointData->get();
			_x2SpinBox->setValue(point.x);
			_y2SpinBox->setValue(point.y);
			_x2SpinBox->setEnabled(false);
			_y2SpinBox->setEnabled(false);
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
		_x1SpinBox->setEnabled(true);
		_y1SpinBox->setEnabled(true);
	}
	else if (portIndex == 2) {
		_x2SpinBox->setEnabled(true);
		_y2SpinBox->setEnabled(true);
	}
	else if (portIndex == 3) {
		_colorBSpinBox->setEnabled(true);
		_colorGSpinBox->setEnabled(true);
		_colorRSpinBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> DrawRectangleModel::outData(QtNodes::PortIndex port)
{
	if (_outputImage.empty())
		return nullptr;
	return std::make_shared<ImageData>(_outputImage);
}
