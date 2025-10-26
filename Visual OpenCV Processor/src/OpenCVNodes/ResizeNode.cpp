#include "ResizeNode.h"
#include "NodeDataType.h"

void ResizeNode::calculate()
{
	if (_ratio) {
		ratio();
	}
	if (_originalImage.data) {
		int width = _ui.width_spinBox->value();
		int height = _ui.height_spinBox->value();
		int interpolation = _ui.interpolation_comboBox->currentData().toInt();
		cv::resize(_originalImage, _resizedImage, cv::Size(width, height), 0, 0, interpolation);
		Q_EMIT dataUpdated(0);
	}
}

void ResizeNode::ratio()
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

ResizeNode::ResizeNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);
	_ratio = false;

	connect(_ui.width_spinBox, &QSpinBox::editingFinished, this, &ResizeNode::calculate);
	connect(_ui.height_spinBox, &QSpinBox::editingFinished, this, &ResizeNode::calculate);
	connect(_ui.interpolation_comboBox, &QComboBox::currentTextChanged, this, &ResizeNode::calculate);
	connect(_ui.ratio_comboBox, &QComboBox::currentTextChanged, this, &ResizeNode::calculate);
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

unsigned int ResizeNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType ResizeNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Number", "宽度" };
		else
			return QtNodes::NodeDataType{ "Number", "高度" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ResizeNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	static bool in1 = false;
	static bool in2 = false;

	if (nodeData) {
		if (portIndex == 0) {
			auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imgData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int num = numberData->get();
			if (num <= 0)
				num = 1;
			if (num > 9999)
				num = 9999;
			_ui.ratio_checkBox->setEnabled(false);
			_ui.ratio_checkBox->setChecked(false);
			_ui.width_spinBox->setEnabled(false);
			in1 = true;
			_ui.width_spinBox->setValue(num);
			calculate();
		}
		else if (portIndex == 2) {
			auto numberData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int num = numberData->get();
			if (num <= 0)
				num = 1;
			if (num > 9999)
				num = 9999;
			_ui.ratio_checkBox->setEnabled(false);
			_ui.ratio_checkBox->setChecked(false);
			_ui.height_spinBox->setEnabled(false);
			in2 = true;
			_ui.height_spinBox->setValue(num);
			calculate();
		}
	}
	else {
		if (portIndex == 0) {
			_originalImage.release();
			_resizedImage.release();
			Q_EMIT dataUpdated(0);
		}
		else if (portIndex == 1) {
			_ui.ratio_checkBox->setEnabled(true);
			_ui.width_spinBox->setEnabled(true);
			in1 = false;
		}
		else if (portIndex == 2) {
			_ui.height_spinBox->setEnabled(true);
			in2 = false;
		}
	}

	if(in1==false && in2==false){
		_ui.ratio_checkBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> ResizeNode::outData(QtNodes::PortIndex port)
{
	if (_resizedImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_resizedImage);
}
