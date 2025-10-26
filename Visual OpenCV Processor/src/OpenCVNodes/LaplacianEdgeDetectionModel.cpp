#include "LaplacianEdgeDetectionModel.h"

void LaplacianEdgeDetectionModel::calculate() {
	if (!_originalImage.data) {
		_ui.info_label->setStyleSheet("QLabel { color : orange; }");
		_ui.info_label->setText("等待数据：图像输入 ");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	if (_originalImage.channels() != 1) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("警告：需要单通道灰度图像！");
		_outputImage.release();
		Q_EMIT dataUpdated(0);
		return;
	}

	int ksize = _ui.ksize_spinBox->value();
	if (ksize % 2 == 0)
		ksize += 1; // Sobel算子大小必须为奇数
	int scale = _ui.scale_spinBox->value();
	int delta = _ui.delta_spinBox->value();
	QElapsedTimer timer;
	timer.start();
	cv::Laplacian(_originalImage, _outputImage, CV_16S, ksize, scale, delta, cv::BORDER_DEFAULT);
	// 转换回 CV_8U
	convertScaleAbs(_outputImage, _outputImage);
	double elapsed = timer.nsecsElapsed();

	_ui.info_label->setStyleSheet("QLabel { color : green; }");
	_ui.info_label->setText(QString("完成 %1 ms").arg(elapsed / 1e6, 0, 'f', 2));

	Q_EMIT dataUpdated(0);
}

LaplacianEdgeDetectionModel::LaplacianEdgeDetectionModel()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);
	
	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("等待数据：图像输入 ");

	connect(_ui.ksize_spinBox, &QSpinBox::valueChanged, this, &LaplacianEdgeDetectionModel::calculate);
	connect(_ui.scale_spinBox, &QSpinBox::valueChanged, this, &LaplacianEdgeDetectionModel::calculate);
	connect(_ui.delta_spinBox, &QSpinBox::valueChanged, this, &LaplacianEdgeDetectionModel::calculate);
}

unsigned int LaplacianEdgeDetectionModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType LaplacianEdgeDetectionModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void LaplacianEdgeDetectionModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imageData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imageData->get();
	}
	else {
		_originalImage.release();
		_outputImage.release();
	}
	calculate();
}

std::shared_ptr<QtNodes::NodeData> LaplacianEdgeDetectionModel::outData(QtNodes::PortIndex port)
{
	if(_outputImage.data)
		return std::make_shared<ImageData>(_outputImage);
	else
		return nullptr;
}
