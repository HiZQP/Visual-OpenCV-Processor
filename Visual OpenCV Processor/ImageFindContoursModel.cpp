#include "ImageFindContoursModel.h"

#include <QFormLayout>

void ImageFindContoursModel::calculate() {
	if (_originalImage.data) {
		int mode = _wmode->currentData().toInt();
		int method = _method->currentData().toInt();
		_contours.clear();
		cv::findContours(_originalImage, _contours, mode, method);
		_infoLabel->setText(QString("检测到 %1 个轮廓").arg(_contours.size()));
		Q_EMIT dataUpdated(0);
	}
	else {
		_infoLabel->setText("无输入");
	}
}

ImageFindContoursModel::ImageFindContoursModel()
{
	_widget = new QWidget();
	_wmode = new QComboBox();
	_wmode->addItem("RETR_EXTERNAL", cv::RetrievalModes::RETR_EXTERNAL);
	_wmode->addItem("RETR_LIST", cv::RetrievalModes::RETR_LIST);
	_wmode->addItem("RETR_CCOMP", cv::RetrievalModes::RETR_CCOMP);
	_wmode->addItem("RETR_TREE", cv::RetrievalModes::RETR_TREE);
	_wmode->setCurrentIndex(0);
	_method = new QComboBox();
	_method->addItem("CHAIN_APPROX_NONE", cv::CHAIN_APPROX_NONE);
	_method->addItem("CHAIN_APPROX_SIMPLE", cv::CHAIN_APPROX_SIMPLE);
	_method->addItem("CHAIN_APPROX_TC89_L1", cv::CHAIN_APPROX_TC89_L1);
	_method->addItem("CHAIN_APPROX_TC89_KCOS", cv::CHAIN_APPROX_TC89_KCOS);
	_method->setCurrentIndex(1);
	_infoLabel = new QLabel("无输入");
	QFormLayout* layout = new QFormLayout();
	layout->addRow("轮廓检索模式", _wmode);
	layout->addRow("轮廓近似方法", _method);
	layout->addRow(_infoLabel);
	_widget->setLayout(layout);
	connect(_wmode, &QComboBox::currentTextChanged, this, &ImageFindContoursModel::calculate);
	connect(_method, &QComboBox::currentTextChanged, this, &ImageFindContoursModel::calculate);
}

unsigned int ImageFindContoursModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType ImageFindContoursModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Contours", "轮廓输出" };
}

void ImageFindContoursModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	// 检查是否为单通道二值图像
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		if (imageData && imageData->get().data && imageData->get().channels() == 1) {
			_originalImage = imageData->get();
			calculate();
		}
		else {
			_infoLabel->setText("需要单通道二值图像");
			_originalImage.release();
			_contours.clear();
			Q_EMIT dataUpdated(0);
		}
	}
	else {
		_infoLabel->setText("无输入");
		_originalImage.release();
		_contours.clear();
		Q_EMIT dataUpdated(0);
	}
}

std::shared_ptr<QtNodes::NodeData> ImageFindContoursModel::outData(QtNodes::PortIndex port)
{
	if (!_contours.empty()) {
		return std::make_shared<ContoursData>(_contours);
	}
	else {
		return nullptr;
	}
}
