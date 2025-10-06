#include "InRange.h"

#include <QFormLayout>
#include <QLabel>

void InRange::calculate()
{
	if (_originalImage.data) {
		cv::Scalar low(_lowR->value(), _lowG->value(), _lowB->value());
		cv::Scalar high(_highR->value(), _highG->value(), _highB->value());
		cv::inRange(_originalImage, low, high, _inRangeImage);
		Q_EMIT dataUpdated(0);
	}
}

InRange::InRange()
{
	_widget = new QWidget();
	auto hLayout = new QHBoxLayout();
	_lowB = new QSpinBox();
	_lowB->setRange(0, 255);
	_lowB->setValue(0);
	_lowG = new QSpinBox();
	_lowG->setRange(0, 255);
	_lowG->setValue(0);
	_lowR = new QSpinBox();
	_lowR->setRange(0, 255);
	_lowR->setValue(0);
	hLayout->addWidget(new QLabel("I:"));
	hLayout->addWidget(_lowR);
	hLayout->addWidget(new QLabel("J:"));
	hLayout->addWidget(_lowG);
	hLayout->addWidget(new QLabel("K:"));
	hLayout->addWidget(_lowB);
	auto hLayout2 = new QHBoxLayout();
	_highB = new QSpinBox();
	_highB->setRange(0, 255);
	_highB->setValue(255);
	_highG = new QSpinBox();
	_highG->setRange(0, 255);
	_highG->setValue(255);
	_highR = new QSpinBox();
	_highR->setRange(0, 255);
	_highR->setValue(255);
	hLayout2->addWidget(new QLabel("I:"));
	hLayout2->addWidget(_highR);
	hLayout2->addWidget(new QLabel("J:"));
	hLayout2->addWidget(_highG);
	hLayout2->addWidget(new QLabel("K:"));
	hLayout2->addWidget(_highB);
	auto layout = new QFormLayout();
	layout->addRow(new QLabel("高阈值"), hLayout2);
	layout->addRow(new QLabel("低阈值"), hLayout);
	_widget->setLayout(layout);
	connect(_lowB, &QSpinBox::valueChanged, this, &InRange::calculate);
	connect(_lowG, &QSpinBox::valueChanged, this, &InRange::calculate);
	connect(_lowR, &QSpinBox::valueChanged, this, &InRange::calculate);
	connect(_highB, &QSpinBox::valueChanged, this, &InRange::calculate);
	connect(_highG, &QSpinBox::valueChanged, this, &InRange::calculate);
	connect(_highR, &QSpinBox::valueChanged, this, &InRange::calculate);
}

unsigned int InRange::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 3;
	else
		return 1;
}

QtNodes::NodeDataType InRange::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Image", "图像输入" };
		else if (portIndex == 1)
			return QtNodes::NodeDataType{ "Scalar", "高阈值" };
		else
			return QtNodes::NodeDataType{ "Scalar", "低阈值" };
	}
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void InRange::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		if (portIndex == 0) {
			auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
			_originalImage = imageData->get();
			calculate();
		}
		else if (portIndex == 1) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			cv::Scalar high = scalarData->get();
			_highB->setValue(high[2]);
			_highG->setValue(high[1]);
			_highR->setValue(high[0]);
			_highB->setEnabled(false);
			_highG->setEnabled(false);
			_highR->setEnabled(false);
			calculate();
		}
		else if (portIndex == 2) {
			auto scalarData = std::dynamic_pointer_cast<ScalarData>(nodeData);
			cv::Scalar low = scalarData->get();
			_lowB->setValue(low[2]);
			_lowG->setValue(low[1]);
			_lowR->setValue(low[0]);
			_lowB->setEnabled(false);
			_lowG->setEnabled(false);
			_lowR->setEnabled(false);
			calculate();
		}
	}
	else if (portIndex == 0) {
		_originalImage.release();
		_inRangeImage.release();
		Q_EMIT dataInvalidated(0);
	}
	else if (portIndex == 1) {
		_lowB->setEnabled(true);
		_lowG->setEnabled(true);
		_lowR->setEnabled(true);
	}
	else if (portIndex == 2) {
		_highB->setEnabled(true);
		_highG->setEnabled(true);
		_highR->setEnabled(true);
	}
}

std::shared_ptr<QtNodes::NodeData> InRange::outData(QtNodes::PortIndex port)
{
	if (_inRangeImage.data) {
		return std::make_shared<ImageData>(_inRangeImage);
	}
	return nullptr;
}
