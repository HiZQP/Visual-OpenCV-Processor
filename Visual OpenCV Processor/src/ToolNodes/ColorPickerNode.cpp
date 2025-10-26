#include "ColorPickerNode.h"
#include <QColorDialog>

void ColorPickerNode::calculate()
{

	QColorDialog colorDialog;
	colorDialog.setCurrentColor(_qcolor);
	colorDialog.setOption(QColorDialog::ShowAlphaChannel, false); // 可选：是否显示透明度
	colorDialog.setWindowTitle("选择颜色");
	if (colorDialog.exec() == QDialog::Accepted) {
		_qcolor = colorDialog.currentColor();
	}
	if (_qcolor.isValid()) {
		int spaceIndex = _ui.colorSpace_comboBox->currentIndex();
		switch (spaceIndex) {
		case 0: // BGR
			_color = cv::Scalar(_qcolor.blue(), _qcolor.green(), _qcolor.red());
			break;
		case 1: // RGB
			_color = cv::Scalar(_qcolor.red(), _qcolor.green(), _qcolor.blue());
			break;
		case 2: // HSV
			_color = cv::Scalar(_qcolor.hue(), _qcolor.saturation(), _qcolor.value());
			break;
		case 3: // HLS
			_color = cv::Scalar(_qcolor.hue(), _qcolor.lightness(), _qcolor.saturation());
			break;
		}
		_ui.color_pushButton->setStyleSheet(QString("background-color: %1;").arg(_qcolor.name()));
		Q_EMIT dataUpdated(0);
	}
}

ColorPickerNode::ColorPickerNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_qcolor = QColor(255, 0, 0);
	_color = cv::Scalar(_qcolor.blue(), _qcolor.green(), _qcolor.red());
	_ui.color_pushButton->setStyleSheet(QString("background-color: %1;").arg(_qcolor.name()));

	connect(_ui.color_pushButton, &QPushButton::clicked, this, &ColorPickerNode::calculate);
	connect(_ui.colorSpace_comboBox, &QComboBox::currentTextChanged, this, &ColorPickerNode::calculate);
}

unsigned int ColorPickerNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 0;
	else
		return 1;
}

QtNodes::NodeDataType ColorPickerNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	return QtNodes::NodeDataType{ "Scalar", "颜色" };
}

std::shared_ptr<QtNodes::NodeData> ColorPickerNode::outData(QtNodes::PortIndex port)
{
	return std::make_shared<ScalarData>(_color);
}
