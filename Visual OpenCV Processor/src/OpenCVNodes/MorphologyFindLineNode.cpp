#include "MorphologyFindLineNode.h"

void MorphologyFindLineNode::calculate()
{
	if (!_inputImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：输入图像 ");
		_outputImage.release();
		emit dataUpdated(0);
		return;
	}

	if (_inputImage.channels() != 1) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("需要单通道图像");
		_inputImage.release();
		_outputImage.release();
		emit dataUpdated(0);
		return;
	}

	int lineType = _ui.lineType_comboBox->currentIndex();

	QElapsedTimer timer;
	double elapsed = 0;
	if (lineType == 0) {
		_outputImage = _inputImage.clone();
		int horizontalSize = _outputImage.cols / 30;

		timer.start();
		cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontalSize, 1));
		erode(_outputImage, _outputImage, horizontalStructure, cv::Point(-1, -1));
		dilate(_outputImage, _outputImage, horizontalStructure, cv::Point(-1, -1));
	}
	else if (lineType == 1) {
		_outputImage = _inputImage.clone();
		int verticalSize = _outputImage.rows / 30;

		timer.start();
		cv::Mat verticalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, verticalSize));
		erode(_outputImage, _outputImage, verticalStructure, cv::Point(-1, -1));
		dilate(_outputImage, _outputImage, verticalStructure, cv::Point(-1, -1));
	}
	elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("检测完成 %1ms").arg(elapsed / 1e6, 0, 'f', 2));
	emit dataUpdated(0);
}

MorphologyFindLineNode::MorphologyFindLineNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：输入图像 ");

	connect(_ui.lineType_comboBox, &QComboBox::currentTextChanged, this, &MorphologyFindLineNode::calculate);
}

unsigned int MorphologyFindLineNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType MorphologyFindLineNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "输入图像" };
	else
		return QtNodes::NodeDataType{ "Image", "输出图像" };
}

void MorphologyFindLineNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto data = std::dynamic_pointer_cast<ImageData>(nodeData);
		_inputImage = data->get();
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> MorphologyFindLineNode::outData(QtNodes::PortIndex port)
{
	if(_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else
		return nullptr;
}
