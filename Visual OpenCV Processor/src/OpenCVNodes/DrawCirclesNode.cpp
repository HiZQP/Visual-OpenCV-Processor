#include "DrawCirclesNode.h"

void DrawCirclesNode::calculate()
{
	QString infoText = "等待数据：";
	bool notOk = false;
	if (_originalImage.empty()) {
		infoText += "输入图像 ";
		notOk = true;
	}
	if (_circles.empty()) {
		infoText += "圆集 ";
		notOk = true;
	}
	if (notOk) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText(infoText);
		emit dataUpdated(0);
		return;
	}

	_outputImage = _originalImage.clone();
	int thickness = _ui.thickness_spinBox->value();
	int lineType = _ui.type_comboBox->currentIndex() == 0 ? cv::LINE_4 :
		_ui.type_comboBox->currentIndex() == 1 ? cv::LINE_8 : cv::LINE_AA;
	QElapsedTimer timer;

	timer.start();
	for(const auto& circle : _circles){
		cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
		int radius = cvRound(circle[2]);
		cv::circle(_outputImage, center, radius, cv::Scalar(_color.blue(), _color.green(), _color.red()), thickness, lineType);
	}
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1 ms").arg(elapsed / 1e6, 0, 'f', 2));
	Q_EMIT dataUpdated(0);
}

void DrawCirclesNode::openColorDialog()
{
	QColor color = QColorDialog::getColor(_color, nullptr, "选择颜色");
	if (color.isValid())
	{
		_color = color;
		_ui.color_pushButton->setStyleSheet(QString("background-color: %1").arg(_color.name()));
		calculate();
	}
}

DrawCirclesNode::DrawCirclesNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_color = QColor(255, 0, 0);
	_ui.color_pushButton->setStyleSheet(QString("background-color: %1").arg(_color.name()));
	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：输入图像 圆集");

	connect(_ui.thickness_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DrawCirclesNode::calculate);
	connect(_ui.type_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DrawCirclesNode::calculate);
	connect(_ui.color_pushButton, &QPushButton::clicked, this, &DrawCirclesNode::openColorDialog);
}

unsigned int DrawCirclesNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else
		return 1;
}

QtNodes::NodeDataType DrawCirclesNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In){
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "输入图像" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Vector", "圆集" };
	}
	else{
		return QtNodes::NodeDataType{ "Image", "输出图像" };
	}
}

void DrawCirclesNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if(nodeData){
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
		}
		else if (portIndex == 1) {
			auto vectorData = std::dynamic_pointer_cast<VectorData>(nodeData);
			_circles = vectorData->get();
		}
	}
	else{
		if (portIndex == 0){
			_originalImage.release();
		}
		else if (portIndex == 1){
			_circles.clear();
		}
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> DrawCirclesNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.empty())
		return nullptr;
	return std::make_shared<ImageData>(_outputImage);
}
