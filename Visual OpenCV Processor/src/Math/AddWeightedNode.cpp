#include "AddWeightedNode.h"

#include <QElapsedTimer>

void AddWeightedNode::calculate()
{
	if (_normalization)
		nomalization();

	if (_input1.empty() || _input2.empty()) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		QString status = "等待数据：";
		if (_input1.empty())
			status += "图像1 ";
		if (_input2.empty())
			status += "图像2 ";
		_ui.info_label->setText(status);
		return;
	}
	else if (_input1.size() != _input2.size() || _input1.type() != _input2.type()) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("输入图像尺寸或类型不匹配");
		return;
	}

	double alpha = _ui.alpha_doubleSpinBox->value();
	double beta = _ui.beta_doubleSpinBox->value();
	double gamma = _ui.gamma_doubleSpinBox->value();

	QElapsedTimer timer;
	timer.start();
	cv::addWeighted(_input1, alpha, _input2, beta, gamma, _output);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("混合完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));

	Q_EMIT dataUpdated(0);
}

void AddWeightedNode::nomalization()
{
	double alpha = _ui.alpha_doubleSpinBox->value();
	double beta = 1.0 - alpha;
	_ui.beta_doubleSpinBox->setValue(beta);
}

AddWeightedNode::AddWeightedNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);
	_normalization = true;
	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：图像1 图像2 ");

	connect(_ui.alpha_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &AddWeightedNode::calculate);
	connect(_ui.beta_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &AddWeightedNode::calculate);
	connect(_ui.gamma_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &AddWeightedNode::calculate);
	connect(_ui.normalization_checkBox, &QCheckBox::checkStateChanged, [this]() {
		if (_ui.normalization_checkBox->isChecked()) {
			_ui.beta_doubleSpinBox->setEnabled(false);
			_normalization = true;
		}
		else {
			_ui.beta_doubleSpinBox->setEnabled(true);
			_normalization = false;
		}
		});
}

unsigned int AddWeightedNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else
		return 1;
}

QtNodes::NodeDataType AddWeightedNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		if (portIndex == 0) {
			return QtNodes::NodeDataType{ "Image", "图像1输入" };
		}
		else {
			return QtNodes::NodeDataType{ "Image", "图像2输入" };
		}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void AddWeightedNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if(portIndex==0){
			auto inData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_input1 = inData->get();
		}
		else if(portIndex==1){
			auto inData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_input2 = inData->get();
		}
	}
	else {
		if (portIndex == 0) {
			_input1.release();
			_output.release();
		}
		else if (portIndex == 1) {
			_input2.release();
			_output.release();
		}
	}

	calculate();
}

std::shared_ptr<QtNodes::NodeData> AddWeightedNode::outData(QtNodes::PortIndex port)
{
	if (_output.data)
		return std::make_shared<ImageData>(_output);
	return nullptr;
}
