#include "BCGAdjustment.h"

void BCGAdjustment::calculate()
{
	if (!_originalImage.empty()) {
		_outputImage = _originalImage.clone();
		double alpha = _ui.alpha_doubleSpinBox->value();
		int beta = _ui.beta_spinBox->value();
		double gamma = _ui.gamma_doubleSpinBox->value();
		_outputImage.convertTo(_outputImage, -1, alpha, beta);

		if (_gamma) {

			cv::Mat lookUpTable(1, 256, CV_8U);
			uchar* p = lookUpTable.ptr();
			for (int i = 0; i < 256; ++i)
				p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

			cv::LUT(_outputImage, lookUpTable, _outputImage);
		}

		Q_EMIT dataUpdated(0);
	}
}

BCGAdjustment::BCGAdjustment()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	connect(_ui.alpha_doubleSpinBox, &QDoubleSpinBox::valueChanged, this, &BCGAdjustment::calculate);
	connect(_ui.beta_spinBox, &QSpinBox::valueChanged, this, &BCGAdjustment::calculate);
	connect(_ui.gamma_doubleSpinBox, &QDoubleSpinBox::valueChanged, this, &BCGAdjustment::calculate);
	connect(_ui.gamma_checkBox, &QCheckBox::checkStateChanged, [this]() {
		if (_ui.gamma_checkBox->isChecked()) {
			_ui.gamma_doubleSpinBox->setEnabled(true);
			_gamma = true;
		}
		else {
			_ui.gamma_doubleSpinBox->setEnabled(false);
			_gamma = false;
		}
		calculate();
		});
}

unsigned int BCGAdjustment::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType BCGAdjustment::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "输入图像" };
	else
		return QtNodes::NodeDataType{ "Image", "输出图像" };
}

void BCGAdjustment::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
	}
	else {
		_originalImage.release();
		_outputImage.release();
		Q_EMIT dataUpdated(0);
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> BCGAdjustment::outData(QtNodes::PortIndex port)
{
	if(_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	return nullptr;
}
