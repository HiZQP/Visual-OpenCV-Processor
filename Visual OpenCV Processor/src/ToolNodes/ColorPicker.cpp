#include "ColorPicker.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

void ColorPicker::calculate()
{
	QColor color = QColorDialog::getColor(QColor(255, 0, 0), nullptr, "选择颜色");
	if (color.isValid()) {
		int spaceIndex = _colorSpace->currentIndex();
		switch (spaceIndex) {
		case 0: // BGR
			_color = cv::Scalar(color.blue(), color.green(), color.red());
			break;
		case 1: // RGB
			_color = cv::Scalar(color.red(), color.green(), color.blue());
			break;
		case 2: // HSV
			_color = cv::Scalar(color.hue(), color.saturation(), color.value());
			break;
		case 3: // HLS
			_color = cv::Scalar(color.hue(), color.lightness(), color.saturation());
			break;
		}
		_button->setStyleSheet(QString("background-color: %1;").arg(color.name()));
		Q_EMIT dataUpdated(0);
	}
}

ColorPicker::ColorPicker()
{
	_widget = new QWidget();
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QHBoxLayout* layout = new QHBoxLayout();
	QLabel* label = new QLabel("选择颜色:");
	_button = new QPushButton();
	QHBoxLayout* colorSpaceLayout = new QHBoxLayout();
	QLabel* colorSpaceLabel = new QLabel("颜色空间:");
	_colorSpace = new QComboBox();
	_colorSpace->addItem("BGR");
	_colorSpace->addItem("RGB");
	_colorSpace->addItem("HSV");
	_colorSpace->addItem("HLS");
	colorSpaceLayout->addWidget(colorSpaceLabel);
	colorSpaceLayout->addWidget(_colorSpace);
	_button->setStyleSheet("background-color: rgb(255, 0, 0);");
	_color = cv::Scalar(0, 0, 255);
	layout->addWidget(label);
	layout->addWidget(_button);
	mainLayout->addLayout(layout);
	mainLayout->addLayout(colorSpaceLayout);
	_widget->setLayout(mainLayout);
	connect(_button, &QPushButton::clicked, this, &ColorPicker::calculate);
	connect(_colorSpace, &QComboBox::currentTextChanged, this, &ColorPicker::calculate);
}

unsigned int ColorPicker::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 0;
	else
		return 1;
}

QtNodes::NodeDataType ColorPicker::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	return QtNodes::NodeDataType{ "Scalar", "颜色" };
}

std::shared_ptr<QtNodes::NodeData> ColorPicker::outData(QtNodes::PortIndex port)
{
	return std::make_shared<ScalarData>(_color);
}
