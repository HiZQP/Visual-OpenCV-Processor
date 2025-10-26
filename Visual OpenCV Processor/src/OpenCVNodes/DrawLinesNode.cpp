#include "DrawLinesNode.h"

#include <QFormLayout>
#include <QSpinBox>
#include <QColorDialog>

void DrawLinesNode::calculate() {
	if (!_originalImage.empty()) {
		_outputImage = _originalImage.clone();
		int thickness = _thickness->currentData().toInt();
		int lineType = _lineType->currentData().toInt();
		for (const auto& line : _lines) {
			cv::line(_outputImage, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), _color, thickness, lineType);
		}
		Q_EMIT dataUpdated(0);
	}
}

void DrawLinesNode::openColorDialog()
{
	QColor initial = QColor::fromRgb(_color[2], _color[1], _color[0]);
	QColor color = QColorDialog::getColor(initial, nullptr, "选择线条颜色");
	if (color.isValid()) {
		_color = cv::Scalar(color.blue(), color.green(), color.red());
		_colorPickerButton->setStyleSheet(QString("background-color: %1;").arg(color.name()));
		Q_EMIT dataUpdated(0);
	}
}

DrawLinesNode::DrawLinesNode()
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	_widget = new QWidget();
	_infoLabel = new QLabel("无输入");
	_thickness = new QComboBox();
	_thickness->addItem("1", 1);
	_thickness->addItem("2", 2);
	_thickness->addItem("3", 3);
	_thickness->addItem("4", 4);
	_thickness->addItem("5", 5);
	_thickness->setCurrentIndex(0);
	_lineType = new QComboBox();
	_lineType->addItem("8连接线", cv::LINE_8);
	_lineType->addItem("4连接线", cv::LINE_4);
	_lineType->addItem("抗锯齿线", cv::LINE_AA);
	_lineType->setCurrentIndex(0);
	_colorPickerButton = new QPushButton();
	_colorPickerButton->setStyleSheet("background-color: rgb(255, 0, 0);");
	_color = cv::Scalar(0, 0, 255);
	QFormLayout* layout = new QFormLayout();
	layout->addRow("线条粗细", _thickness);
	layout->addRow("线条类型", _lineType);
	layout->addRow("线条颜色", _colorPickerButton);
	mainLayout->addLayout(layout);
	mainLayout->addWidget(_infoLabel);
	_widget->setLayout(mainLayout);
	connect(_thickness, &QComboBox::currentTextChanged, this, &DrawLinesNode::calculate);
	connect(_lineType, &QComboBox::currentTextChanged, this, &DrawLinesNode::calculate);
	connect(_colorPickerButton, &QPushButton::clicked, this, &DrawLinesNode::openColorDialog);
}

unsigned int DrawLinesNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2; // 输入图像和线段数据
	else
		return 1; // 输出图像
}

QtNodes::NodeDataType DrawLinesNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{"Image", "输入图像"};
		else if (portIndex == 1)
			return QtNodes::NodeDataType{"Vector", "线段集"};
	}
	else {
		if (portIndex == 0)
			return QtNodes::NodeDataType{"Image", "输出图像" };
	}
}

void DrawLinesNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			_infoLabel->setText("就绪");
			calculate();
		}
		else if (portIndex == 1) {
			auto linesData = std::dynamic_pointer_cast<VectorData>(nodeData);
			_lines = linesData->get();
			calculate();
		}
	}
	else {
		if (portIndex == 0) {
			_originalImage.release();
			_outputImage.release();
			_infoLabel->setText("无输入");
			Q_EMIT dataUpdated(0);
		}
		else if (portIndex == 1) {
			_outputImage.release();
			_lines.clear();
			Q_EMIT dataUpdated(0);
		}
	}
}

std::shared_ptr<QtNodes::NodeData> DrawLinesNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data) {
		return std::make_shared<ImageData>(_outputImage);
	}
	else {
		return nullptr;
	}
}
