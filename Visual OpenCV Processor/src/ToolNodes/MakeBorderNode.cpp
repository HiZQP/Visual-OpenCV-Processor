#include "MakeBorderNode.h"
#include <QColorDialog>

void MakeBorderNode::calculate()
{
	if (_inputImage.empty()) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：输入图像 ");
		_outputImage.release();
		emit dataUpdated(0);
		return;
	}
	int top = _ui.top_spinBox->value();
	int bottom = _ui.bottom_spinBox->value();
	int left = _ui.left_spinBox->value();
	int right = _ui.right_spinBox->value();
	int borderType = _ui.borderType_comboBox->currentData().toInt();
	QColor color = _ui.color_pushButton->palette().button().color();
	cv::Scalar borderColor(color.blue(), color.green(), color.red());
	QElapsedTimer timer;

	timer.start();
	cv::copyMakeBorder(_inputImage, _outputImage, top, bottom, left, right, borderType, borderColor);
	double elapsed = timer.elapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1 ms").arg(elapsed / 1e6, 0, 'f', 2));
	emit dataUpdated(0);
}

MakeBorderNode::MakeBorderNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：输入图像 ");

	_ui.borderType_comboBox->addItem("常量", cv::BORDER_CONSTANT);
	_ui.borderType_comboBox->addItem("复制", cv::BORDER_REPLICATE);
	_ui.borderType_comboBox->addItem("反射", cv::BORDER_REFLECT);
	_ui.borderType_comboBox->addItem("反射101", cv::BORDER_REFLECT_101);
	_ui.borderType_comboBox->addItem("环绕", cv::BORDER_WRAP);
	_ui.borderType_comboBox->setCurrentIndex(0);

	_ui.color_pushButton->setStyleSheet("background-color: red");

	connect(_ui.top_spinBox, &QSpinBox::editingFinished, this, &MakeBorderNode::calculate);
	connect(_ui.bottom_spinBox, &QSpinBox::editingFinished, this, &MakeBorderNode::calculate);
	connect(_ui.left_spinBox, &QSpinBox::editingFinished, this, &MakeBorderNode::calculate);
	connect(_ui.right_spinBox, &QSpinBox::editingFinished, this, &MakeBorderNode::calculate);
	connect(_ui.borderType_comboBox, &QComboBox::currentTextChanged, this, &MakeBorderNode::calculate);
	connect(_ui.color_pushButton, &QPushButton::clicked, [this]() {
		QColor color = QColorDialog::getColor(Qt::white, _widget, "选择边界颜色");
		if (color.isValid()) {
			_ui.color_pushButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
			calculate();
		}
		});
}

unsigned int MakeBorderNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType MakeBorderNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	} else {
		return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
}

void MakeBorderNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_inputImage = imageData->get();
	} else {
		_inputImage.release();
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> MakeBorderNode::outData(QtNodes::PortIndex port)
{
	if(_outputImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_outputImage);
}
