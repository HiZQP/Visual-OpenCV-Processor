#include "ColorPicker.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

ColorPicker::ColorPicker()
{
	_widget = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout();
	QLabel* label = new QLabel("选择颜色:");
	QPushButton* button = new QPushButton();
	button->setStyleSheet("background-color: rgb(255, 0, 0);");
	_color = cv::Scalar(0, 0, 255);
	layout->addWidget(label);
	layout->addWidget(button);
	_widget->setLayout(layout);
	connect(button, &QPushButton::clicked, this, [this, button]() {
		QColor initialColor = QColor(_color[2], _color[1], _color[0]);
		QColor color = QColorDialog::getColor(initialColor, nullptr, "选择颜色");
		if (color.isValid()) {
			_color = cv::Scalar(color.blue(), color.green(), color.red());
			button->setStyleSheet(QString("background-color: %1;").arg(color.name()));
			Q_EMIT dataUpdated(0);
		}
		});
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
