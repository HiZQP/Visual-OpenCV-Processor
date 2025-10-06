#include "DrawTextModel.h"

#include <QFormLayout>
#include <QLabel>

void DrawTextModel::calculate()
{
	if (_originalImage.data) {
		_textImage = _originalImage.clone();
		std::string text = _text->text().toStdString();
		double fontScale = _fontScale->value();
		int thickness = _thickness->value();
		int fontFace = _fontFace->currentData().toInt();
		int lineType = _lineType->currentData().toInt();
		int baseline = 0;
		cv::Scalar color(_colorB->value(), _colorG->value(), _colorR->value());
		cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
		cv::Point pos(_posX->value(), _posY->value());
		cv::putText(_textImage, text, pos, fontFace, fontScale, color, thickness, lineType);
		Q_EMIT dataUpdated(0);
	}
}

DrawTextModel::DrawTextModel()
{
	_widget = new QWidget();
	_text = new QLineEdit();
	_text->setText("OpenCV");
	auto pos = new QHBoxLayout();
	_posX = new QSpinBox();
	_posX->setValue(50);
	_posY = new QSpinBox();
	_posY->setValue(50);
	pos->addWidget(new QLabel("X:"));
	pos->addWidget(_posX);
	pos->addWidget(new QLabel("Y:"));
	pos->addWidget(_posY);
	_fontScale = new QSpinBox();
	_fontScale->setValue(1);
	_fontScale->setMinimum(1);
	_thickness = new QSpinBox();
	_thickness->setValue(2);
	_thickness->setMinimum(1);
	auto hLayout = new QHBoxLayout();
	_colorB = new QSpinBox();
	_colorB->setRange(0, 255);
	_colorB->setValue(0);
	_colorG = new QSpinBox();
	_colorG->setRange(0, 255);
	_colorG->setValue(255);
	_colorR = new QSpinBox();
	_colorR->setRange(0, 255);
	_colorR->setValue(0);
	hLayout->addWidget(new QLabel("R:"));
	hLayout->addWidget(_colorR);
	hLayout->addWidget(new QLabel("G:"));
	hLayout->addWidget(_colorG);
	hLayout->addWidget(new QLabel("B:"));
	hLayout->addWidget(_colorB);
	_fontFace = new QComboBox();
	_fontFace->addItem("HERSHEY_SIMPLEX", cv::FONT_HERSHEY_SIMPLEX);
	_fontFace->addItem("HERSHEY_PLAIN", cv::FONT_HERSHEY_PLAIN);
	_fontFace->addItem("HERSHEY_DUPLEX", cv::FONT_HERSHEY_DUPLEX);
	_fontFace->addItem("HERSHEY_COMPLEX", cv::FONT_HERSHEY_COMPLEX);
	_fontFace->addItem("HERSHEY_TRIPLEX", cv::FONT_HERSHEY_TRIPLEX);
	_fontFace->addItem("HERSHEY_COMPLEX_SMALL", cv::FONT_HERSHEY_COMPLEX_SMALL);
	_fontFace->addItem("HERSHEY_SCRIPT_SIMPLEX", cv::FONT_HERSHEY_SCRIPT_SIMPLEX);
	_fontFace->addItem("HERSHEY_SCRIPT_COMPLEX", cv::FONT_HERSHEY_SCRIPT_COMPLEX);
	_lineType = new QComboBox();
	_lineType->addItem("8连通", cv::LINE_8);
	_lineType->addItem("4连通", cv::LINE_4);
	_lineType->addItem("抗锯齿", cv::LINE_AA);
	auto layout = new QFormLayout();
	layout->addRow("文本", _text);
	layout->addRow("位置", pos);
	layout->addRow("字体大小", _fontScale);
	layout->addRow("字体粗细", _thickness);
	layout->addRow("字体颜色", hLayout);
	layout->addRow("字体类型", _fontFace);
	layout->addRow("线条类型", _lineType);	
	_widget->setLayout(layout);
	connect(_text, &QLineEdit::editingFinished, this, &DrawTextModel::calculate);
	connect(_fontScale, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_thickness, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_fontFace, &QComboBox::currentTextChanged, this, &DrawTextModel::calculate);
	connect(_lineType, &QComboBox::currentTextChanged, this, &DrawTextModel::calculate);
	connect(_posX, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_posY, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_colorR, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_colorG, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
	connect(_colorB, &QSpinBox::editingFinished, this, &DrawTextModel::calculate);
}

unsigned int DrawTextModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 4;
	else
		return 1;
}

QtNodes::NodeDataType DrawTextModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "String", "文本" };
		else if (portIndex == 2)
			return QtNodes::NodeDataType{ "Point", "位置" };
		else
			return QtNodes::NodeDataType{ "Scalar", "颜色" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void DrawTextModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			_posX->setRange(0, _originalImage.cols);
			_posY->setRange(0, _originalImage.rows);
			calculate();
		}
		else if (portIndex == 1) {
			auto stringData = std::dynamic_pointer_cast<StringData>(nodeData);
			_text->setEnabled(false);
			_text->setText(stringData->get());
			calculate();
		}
		else if (portIndex == 2) {
			auto pointData = std::dynamic_pointer_cast<PointData>(nodeData);
			_posX->setEnabled(false);
			_posY->setEnabled(false);
			_posX->setValue(static_cast<int>(pointData->get().x));
			_posY->setValue(static_cast<int>(pointData->get().y));
			calculate();
		}
		else if (portIndex == 3) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			_colorR->setEnabled(false);
			_colorG->setEnabled(false);
			_colorB->setEnabled(false);
			_colorB->setValue(static_cast<int>(scalarData->get()[0]));
			_colorG->setValue(static_cast<int>(scalarData->get()[1]));
			_colorR->setValue(static_cast<int>(scalarData->get()[2]));
			calculate();
		}
	}
	else if (portIndex == 0) {
		_originalImage.release();
		_textImage.release();
		calculate();
	}
	else if (portIndex == 1) {
		_text->setEnabled(true);
	}
	else if (portIndex == 2) {
		_posX->setEnabled(true);
		_posY->setEnabled(true);
	}
	else if (portIndex == 3) {
		_colorR->setEnabled(true);
		_colorG->setEnabled(true);
		_colorB->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> DrawTextModel::outData(QtNodes::PortIndex port)
{
	if (_textImage.data)
		return std::make_shared<ImageData>(_textImage);
	else
		return nullptr;
}
