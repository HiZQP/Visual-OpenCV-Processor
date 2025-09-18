#include "ImageConvertColorModel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

void ImageConvertColorModel::calculate()
{
	if (_inputImage.empty()) {
		_outputImage.release();
		Q_EMIT dataInvalidated(0);
		return;
	}
		// 检查转换兼容性
	int channels = _inputImage.channels();
	int code = _codeBox->currentData().toInt();
	int requiredChannels = isConversionCompatible(code);
	if (channels != requiredChannels) {
		_infoLabel->setText(QString("输入图像通道数为 %1 ，%2 需要 %3 通道").arg(channels).arg(_codeBox->currentText()).arg(requiredChannels));
		_outputImage.release();
		Q_EMIT dataInvalidated(0);
		return;
	}

	cv::cvtColor(_inputImage, _outputImage, code);

	if (_outputImage.empty()) {
		Q_EMIT dataInvalidated(0);
		return;
	}
	else {
		_infoLabel->setText("输入合法");
	}

	Q_EMIT dataUpdated(0);
}

int ImageConvertColorModel::isConversionCompatible(int conversionCode) {
	switch (conversionCode) {
		// 需要3通道输入的转换
	case cv::COLOR_BGR2RGB:
	case cv::COLOR_BGR2BGR565:
	case cv::COLOR_RGB2BGR565:
	case cv::COLOR_BGR2GRAY:
	case cv::COLOR_RGB2GRAY:
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


ImageConvertColorModel::ImageConvertColorModel()
{
	_widget = new QWidget();
	_widget->setFixedSize(300, 100);
	_codeBox = new QComboBox();
	_infoLabel = new QLabel("无输入");
	_codeBox->addItem("BGR2BGRA", cv::COLOR_BGR2BGRA);
	_codeBox->addItem("BGRA2BGR", cv::COLOR_BGRA2BGR);
	_codeBox->addItem("BGR2RGBA", cv::COLOR_BGR2RGBA);
	_codeBox->addItem("RGBA2BGR", cv::COLOR_RGBA2BGR);
	_codeBox->addItem("BGR2RGB", cv::COLOR_BGR2RGB);
	_codeBox->addItem("RGB2BGR", cv::COLOR_RGB2BGR);
	_codeBox->addItem("BGRA2RGBA", cv::COLOR_BGRA2RGBA);
	_codeBox->addItem("RGBA2BGRA", cv::COLOR_RGBA2BGRA);
	_codeBox->addItem("BGR2GRAY", cv::COLOR_BGR2GRAY);
	_codeBox->addItem("RGB2GRAY", cv::COLOR_RGB2GRAY);
	_codeBox->addItem("GRAY2BGR", cv::COLOR_GRAY2BGR);
	_codeBox->addItem("GRAY2RGB", cv::COLOR_GRAY2RGB);
	_codeBox->addItem("GRAY2BGRA", cv::COLOR_GRAY2BGRA);
	_codeBox->addItem("GRAY2RGBA", cv::COLOR_GRAY2RGBA);
	_codeBox->addItem("BGRA2GRAY", cv::COLOR_BGRA2GRAY);
	_codeBox->addItem("RGBA2GRAY", cv::COLOR_RGBA2GRAY);
	auto hLayout = new QHBoxLayout();
	auto vLayout = new QVBoxLayout();
	hLayout->addWidget(new QLabel("转换代码："));
	hLayout->addWidget(_codeBox);
	vLayout->addLayout(hLayout);
	vLayout->addWidget(_infoLabel);

	_widget->setLayout(vLayout);

	connect(_codeBox, &QComboBox::currentTextChanged, this, &ImageConvertColorModel::calculate);
}

unsigned int ImageConvertColorModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageConvertColorModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void ImageConvertColorModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
	static bool isValid = false;
	isValid = (imageData != nullptr);
	if (isValid) {
		_inputImage = imageData->get();
		calculate();
	}
	else {
		_infoLabel->setText("无输入");
		_inputImage.release();
		_outputImage.release();
		Q_EMIT dataInvalidated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageConvertColorModel::outData(QtNodes::PortIndex port)
{
	if (_outputImage.empty()) {
		return nullptr;
	}
	return std::make_shared<ImageData>(_outputImage);
}
