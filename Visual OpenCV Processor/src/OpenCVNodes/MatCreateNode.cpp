#include "MatCreateNode.h"

#define INVALID_TYPE -1

void MatCreateNode::createMat()
{
	if (_ratio) {
		ratio();
	}
	int width = _ui.width_spinBox->value();
	int height = _ui.height_spinBox->value();
	int bit = _ui.bit_comboBox->currentText().toInt();
	int channel = _ui.channel_comboBox->currentText().toInt();
	std::string sign;
	if (_ui.sign_comboBox->currentText() == "无符号")
		sign = "U";
	else if (_ui.sign_comboBox->currentText() == "有符号")
		sign = "S";
	else
		sign = "F";

	int type = -1;
	try {
		if (bit == 8)
			if (sign == "U")
				type = CV_8UC(channel);
			else if (sign == "S")
				type = CV_8SC(channel);
			else
				throw INVALID_TYPE;
		else if (bit == 16)
			if (sign == "U")
				type = CV_16UC(channel);
			else if (sign == "S")
				type = CV_16SC(channel);
			else
				type = CV_16FC(channel);
		else if (bit == 32)
			if (sign == "S")
				type = CV_32SC(channel);
			else if (sign == "F")
				type = CV_32FC(channel);
			else
				throw INVALID_TYPE;
		else if (bit == 64)
			if (sign == "F")
				type = CV_64FC(channel);
			else
				throw INVALID_TYPE;
		else
			throw INVALID_TYPE;
	}
	catch (const int& e) {
		if (e == INVALID_TYPE) {
			_ui.info_label->setStyleSheet("QLabel { color : red; }");
			_ui.info_label->setText("不支持该类型");
			_output.release();
			return;
		}
	}
	_output = cv::Mat::zeros(height, width, type);
	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText("创建完成");

	Q_EMIT dataUpdated(0);
}

void MatCreateNode::ratio()
{
	int ratio_index = _ui.ratio_comboBox->currentIndex();
	if (ratio_index == 0) {//4:3
		int height = _ui.height_spinBox->value();
		int width = height * 4 / 3;
		_ui.width_spinBox->setValue(width);
	}
	else if (ratio_index == 1) {//16:9
		int height = _ui.height_spinBox->value();
		int width = height * 16 / 9;
		_ui.width_spinBox->setValue(width);
	}
	else if (ratio_index == 2) {//1:1
		int height = _ui.height_spinBox->value();
		int width = height;
		_ui.width_spinBox->setValue(width);
	}
	else if (ratio_index == 3) {//3:4
		int height = _ui.height_spinBox->value();
		int width = height * 3 / 4;
		_ui.width_spinBox->setValue(width);
	}
	else if (ratio_index == 4) {//2:3
		int height = _ui.height_spinBox->value();
		int width = height * 2 / 3;
		_ui.width_spinBox->setValue(width);
	}
}

MatCreateNode::MatCreateNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);
	createMat();
	_ui.info_label->setText("创建默认CV_8UC3");
	connect(_ui.height_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatCreateNode::createMat);
	connect(_ui.width_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatCreateNode::createMat);
	connect(_ui.bit_comboBox, &QComboBox::currentTextChanged, this, &MatCreateNode::createMat);
	connect(_ui.sign_comboBox, &QComboBox::currentTextChanged, this, &MatCreateNode::createMat);
	connect(_ui.channel_comboBox, &QComboBox::currentTextChanged, this, &MatCreateNode::createMat);
	connect(_ui.ratio_checkBox, &QCheckBox::checkStateChanged, [this]() {
		if (_ui.ratio_checkBox->isChecked()) {
			_ui.ratio_comboBox->setEnabled(true);
			_ui.width_spinBox->setEnabled(false);
			_ratio = true;
		}
		else {
			_ui.ratio_comboBox->setEnabled(false);
			_ui.width_spinBox->setEnabled(true);
			_ratio = false;
		}
		});
}

unsigned int MatCreateNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 0;
	else
		return 1;
}

QtNodes::NodeDataType MatCreateNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

std::shared_ptr<QtNodes::NodeData> MatCreateNode::outData(QtNodes::PortIndex port)
{
	if(_output.data)
		return std::make_shared<ImageData>(_output);
	else
		return nullptr;
}
