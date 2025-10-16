#include "MorphologyModel.h"

#include <QFormLayout>

void MorphologyModel::calculate() {
	if (_originalImage.data) {
		int morphType = _morphType->currentData().toInt();
		int ksize = _kernelSize->value();
		int shapeType = _shapeType->currentData().toInt();
		cv::Mat element = cv::getStructuringElement(shapeType, cv::Size(ksize, ksize));
		cv::morphologyEx(_originalImage, _morphImage, morphType, element);
		Q_EMIT dataUpdated(0);
	}
}

MorphologyModel::MorphologyModel()
{
	_widget = std::make_unique<QWidget>();
	QFormLayout* layout = new QFormLayout();
	_morphType = new QComboBox();
	_morphType->addItem("膨胀", cv::MORPH_DILATE);
	_morphType->addItem("腐蚀", cv::MORPH_ERODE);
	_morphType->addItem("开运算", cv::MORPH_OPEN);
	_morphType->addItem("闭运算", cv::MORPH_CLOSE);
	_morphType->addItem("形态学梯度", cv::MORPH_GRADIENT);
	_morphType->addItem("礼帽", cv::MORPH_TOPHAT);
	_morphType->addItem("黑帽", cv::MORPH_BLACKHAT);
	_morphType->setCurrentIndex(0);
	layout->addRow("形态学操作类型", _morphType);
	connect(_morphType, &QComboBox::currentTextChanged, this, [this](const QString&) { calculate(); });
	_kernelSize = new QSpinBox();
	_kernelSize->setRange(1, 31);
	_kernelSize->setSingleStep(2);
	_kernelSize->setValue(3);
	layout->addRow("卷积核大小", _kernelSize);
	connect(_kernelSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) { calculate(); });
	_shapeType = new QComboBox();
	_shapeType->addItem("矩形", cv::MORPH_RECT);
	_shapeType->addItem("椭圆形", cv::MORPH_ELLIPSE);
	_shapeType->addItem("十字形", cv::MORPH_CROSS);
	_shapeType->setCurrentIndex(0);
	layout->addRow("卷积核形状", _shapeType);
	_widget->setLayout(layout);
	connect(_shapeType, &QComboBox::currentTextChanged, this, &MorphologyModel::calculate);
	connect(_kernelSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &MorphologyModel::calculate);
	connect(_morphType, &QComboBox::currentTextChanged, this, &MorphologyModel::calculate);
}

unsigned int MorphologyModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 1;
	else
		return 1;
}

QtNodes::NodeDataType MorphologyModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In)
		return QtNodes::NodeDataType{ "Image", "图像输入" };
	else
		return QtNodes::NodeDataType{ "Image", "图像输出" };
}

void MorphologyModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto imgData = std::dynamic_pointer_cast<ImageData>(nodeData);
		_originalImage = imgData->get();
		calculate();
	}
}

std::shared_ptr<QtNodes::NodeData> MorphologyModel::outData(QtNodes::PortIndex port)
{
	if (_morphImage.data) {
		return std::make_shared<ImageData>(_morphImage);
	}
	return nullptr;
}
