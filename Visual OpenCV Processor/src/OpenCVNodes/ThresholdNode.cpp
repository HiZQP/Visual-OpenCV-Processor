#include "ThresholdNode.h"

void ThresholdNode::calculate() {
	if (!_originalImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：图像输入 ");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	int threshold = _ui.threshold_spinBox->value();
	int maxValue = _ui.maxVal_spinBox->value();
	int type = _ui.type_comboBox->currentIndex();
	if (_originalImage.channels() == 3)
		cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGR2GRAY);
	else if (_originalImage.channels() == 4)
		cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGRA2GRAY);
	cv::THRESH_BINARY;
	QElapsedTimer timer;
	timer.start();
	cv::threshold(_originalImage, _outputImage, threshold, maxValue, type);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));
	Q_EMIT dataUpdated(0);
}

ThresholdNode::ThresholdNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：图像输入 ");
	
	connect(_ui.threshold_spinBox, &QSpinBox::editingFinished, this, &ThresholdNode::calculate);
	connect(_ui.maxVal_spinBox, &QSpinBox::editingFinished, this, &ThresholdNode::calculate);
	connect(_ui.type_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ThresholdNode::calculate);
}

unsigned int ThresholdNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType ThresholdNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Number", "阈值" };
		else
			return QtNodes::NodeDataType{ "Number", "最大值" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ThresholdNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imgData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int threshold = static_cast<int>(numData->get());
			if (threshold < 0) threshold = 0;
			if (threshold > 255) threshold = 255;
			_ui.threshold_spinBox->setValue(threshold);
			_ui.threshold_spinBox->setEnabled(false);
		}
		else if (portIndex == 2) {
			auto numData = std::dynamic_pointer_cast<NumberData>(nodeData);
			int threshold = static_cast<int>(numData->get());
			if (threshold < 0) threshold = 0;
			if (threshold > 255) threshold = 255;
			_ui.maxVal_spinBox->setValue(threshold);
			_ui.maxVal_spinBox->setEnabled(false);
		}
	}
	else if (portIndex == 0)
	{
		_originalImage.release();
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
	else if(portIndex == 1){
		_ui.threshold_spinBox->setEnabled(true);
	}
	else if (portIndex == 2) {
		_ui.maxVal_spinBox->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> ThresholdNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else 
		return nullptr;
}
