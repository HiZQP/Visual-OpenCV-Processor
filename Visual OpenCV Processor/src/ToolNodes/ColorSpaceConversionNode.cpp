#include "ColorSpaceConversionNode.h"

void ColorSpaceConversionNode::calculate()
{
	if (!_inputImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：输入图像");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}
	// 检查转换兼容性
	int channels = _inputImage.channels();
	int code = _ui.code_comboBox->currentData().toInt();
	int requiredChannels = isConversionCompatible(code);
	if (channels != requiredChannels) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText(QString("错误：需要 %1 通道，输入 %2 通道").arg(requiredChannels).arg(channels));
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	QElapsedTimer timer;
	timer.start();
	cv::cvtColor(_inputImage, _outputImage, code);
	double elapsed = timer.elapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));

	Q_EMIT dataUpdated(0);
}

int ColorSpaceConversionNode::isConversionCompatible(int conversionCode) {
	switch (conversionCode) {
		// 需要3通道输入的转换
	case cv::COLOR_BGR2RGB:
	case cv::COLOR_BGR2BGR565:
	case cv::COLOR_RGB2BGR565:
	case cv::COLOR_BGR2GRAY:
	case cv::COLOR_RGB2GRAY:
	case cv::COLOR_BGR2HSV:
		return 3;

		// 需要4通道输入的转换
	case cv::COLOR_BGRA2RGBA:
	case cv::COLOR_BGRA2BGR:
	case cv::COLOR_RGBA2BGR:
	case cv::COLOR_BGRA2GRAY:
	case cv::COLOR_RGBA2GRAY:
		return 4;

		// 需要1通道输入的转换
	case cv::COLOR_GRAY2BGR:
	case cv::COLOR_GRAY2BGRA:
	case cv::COLOR_GRAY2BGR565:
		return 1;
	default:
		return false; // 未知的转换代码
	}
}


ColorSpaceConversionNode::ColorSpaceConversionNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);


	_ui.code_comboBox->addItem("BGR2BGRA", cv::COLOR_BGR2BGRA);
	_ui.code_comboBox->addItem("BGRA2BGR", cv::COLOR_BGRA2BGR);
	_ui.code_comboBox->addItem("BGR2RGBA", cv::COLOR_BGR2RGBA);
	_ui.code_comboBox->addItem("RGBA2BGR", cv::COLOR_RGBA2BGR);
	_ui.code_comboBox->addItem("BGR2RGB", cv::COLOR_BGR2RGB);
	_ui.code_comboBox->addItem("BGR2HSV", cv::COLOR_BGR2HSV);
	_ui.code_comboBox->addItem("RGB2BGR", cv::COLOR_RGB2BGR);
	_ui.code_comboBox->addItem("BGRA2RGBA", cv::COLOR_BGRA2RGBA);
	_ui.code_comboBox->addItem("RGBA2BGRA", cv::COLOR_RGBA2BGRA);
	_ui.code_comboBox->addItem("BGR2GRAY", cv::COLOR_BGR2GRAY);
	_ui.code_comboBox->addItem("RGB2GRAY", cv::COLOR_RGB2GRAY);
	_ui.code_comboBox->addItem("GRAY2BGR", cv::COLOR_GRAY2BGR);
	_ui.code_comboBox->addItem("GRAY2RGB", cv::COLOR_GRAY2RGB);
	_ui.code_comboBox->addItem("GRAY2BGRA", cv::COLOR_GRAY2BGRA);
	_ui.code_comboBox->addItem("GRAY2RGBA", cv::COLOR_GRAY2RGBA);
	_ui.code_comboBox->addItem("BGRA2GRAY", cv::COLOR_BGRA2GRAY);
	_ui.code_comboBox->addItem("RGBA2GRAY", cv::COLOR_RGBA2GRAY);

	connect(_ui.code_comboBox, &QComboBox::currentTextChanged, this, &ColorSpaceConversionNode::calculate);
}

unsigned int ColorSpaceConversionNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ColorSpaceConversionNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ColorSpaceConversionNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_inputImage = imgData->get();
	}
	else {
		_inputImage.release();
		_outputImage.release();
	}
	calculate();

}

std::shared_ptr<QtNodes::NodeData> ColorSpaceConversionNode::outData(QtNodes::PortIndex port)
{
	if (_outputImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_outputImage);
}
