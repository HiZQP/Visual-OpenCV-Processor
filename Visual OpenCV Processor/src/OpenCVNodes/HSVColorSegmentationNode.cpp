#include "HSVColorSegmentationNode.h"

void HSVColorSegmentationNode::calculate()
{
	if (!_originalImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：图像输入 ");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	if (_originalImage.channels() != 3) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("错误：需要3通道HSV图像！");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	QElapsedTimer timer;
	cv::Scalar low(_ui.lowerH_spinBox->value() / 2, _ui.lowerS_spinBox->value(), _ui.lowerV_spinBox->value());
	cv::Scalar high(_ui.upperH_spinBox->value() / 2, _ui.upperS_spinBox->value(), _ui.upperV_spinBox->value());

	timer.start();
	cv::inRange(_originalImage, low, high, _outputImage);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));

	Q_EMIT dataUpdated(0);
}

HSVColorSegmentationNode::HSVColorSegmentationNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：图像输入 ");
	
	connect(_ui.lowerV_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
	connect(_ui.lowerS_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
	connect(_ui.lowerH_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
	connect(_ui.upperV_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
	connect(_ui.upperS_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
	connect(_ui.upperH_spinBox, &QSpinBox::valueChanged, this, &HSVColorSegmentationNode::calculate);
}

unsigned int HSVColorSegmentationNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType HSVColorSegmentationNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Scalar", "上界" };
		else
			return QtNodes::NodeDataType{ "Scalar", "下界" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void HSVColorSegmentationNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
		}
		else if (portIndex == 1) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			cv::Scalar high = scalarData->get();
			_ui.upperV_spinBox->setValue(high[2]);
			_ui.upperS_spinBox->setValue(high[1]);
			_ui.upperH_spinBox->setValue(high[0]);
			_ui.upperV_spinBox->setEnabled(false);
			_ui.upperS_spinBox->setEnabled(false);
			_ui.upperH_spinBox->setEnabled(false);
		}
		else if (portIndex == 2) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			cv::Scalar low = scalarData->get();
			_ui.lowerV_spinBox->setValue(low[2]);
			_ui.lowerS_spinBox->setValue(low[1]);
			_ui.lowerH_spinBox->setValue(low[0]);
			_ui.lowerV_spinBox->setEnabled(false);
			_ui.lowerS_spinBox->setEnabled(false);
			_ui.lowerH_spinBox->setEnabled(false);
		}
	}
	else if (portIndex == 0) {
		_originalImage.release();
		_outputImage.release();
	}
	else if (portIndex == 1) {
		_ui.upperH_spinBox->setEnabled(true);
		_ui.upperS_spinBox->setEnabled(true);
		_ui.upperV_spinBox->setEnabled(true);
	}
	else if (portIndex == 2) {
		_ui.lowerH_spinBox->setEnabled(true);
		_ui.lowerS_spinBox->setEnabled(true);
		_ui.lowerV_spinBox->setEnabled(true);
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> HSVColorSegmentationNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.data) {
		return std::make_shared<ImageData>(_outputImage);
	}
	return nullptr;
}
