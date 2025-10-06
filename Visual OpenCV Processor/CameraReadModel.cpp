#include "CameraReadModel.h"

#include <memory>

#include <QHBoxLayout>
#include <QTimer>

void CameraReadModel::readFrame()
{
	_cap >> _cameraFrame;
	Q_EMIT dataUpdated(1);
}

CameraReadModel::CameraReadModel()
{
	// 创建子节点并连接

	_widget = new QWidget();
	_cameraIndex = new QComboBox();
	_pictureCaptureButton = new QPushButton();
	for (int i = 0; i < 10; i++)
	{
		cv::VideoCapture cap(i);
		if (cap.isOpened())
		{
			_cameraIndex->addItem(QString::number(i));
			cap.release();
		}
	}
	_cameraIndex->setCurrentIndex(0);
	_cap.open(_cameraIndex->currentIndex());
	_pictureCaptureButton->setText("捕获一帧");
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(_cameraIndex);
	layout->addWidget(_pictureCaptureButton);
	_widget->setLayout(layout);
	connect(_pictureCaptureButton, &QPushButton::clicked, this, &CameraReadModel::readFrame);
	connect(_cameraIndex, &QComboBox::currentTextChanged, [this]() {
		_cap.release();
		_cap.open(_cameraIndex->currentIndex());
		_cameraFrame.release();
		Q_EMIT dataUpdated(0);
		});
}

unsigned int CameraReadModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::Out)
		return 2;
	else
		return 0;
}

QtNodes::NodeDataType CameraReadModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::Out) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Terget", "触发" };
		else
			return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
}




std::shared_ptr<QtNodes::NodeData> CameraReadModel::outData(QtNodes::PortIndex port)
{
	if (port == 1) {
		if (_cameraFrame.data) {
			return std::make_shared<ImageData>(_cameraFrame);
		}
		return nullptr;
	} else if(port == 0) {
		return std::make_shared<CameraReadModelPtrData>(this);
	}
}

///////////////////////////////////////////////////////////////////

CaptureTriggerModel::CaptureTriggerModel()
{
	_cameraReadModel = nullptr;
}

unsigned int CaptureTriggerModel::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else 
		return 0;
}

QtNodes::NodeDataType CaptureTriggerModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Terget", "目标" };
		else
			return QtNodes::NodeDataType{ "Image", "触发" };
	}
}

void CaptureTriggerModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
{
	if (nodeData) {
		auto cameraReadModelPtrData = std::dynamic_pointer_cast<CameraReadModelPtrData>(nodeData);
		if (cameraReadModelPtrData && portIndex == 0) {
			_cameraReadModel = cameraReadModelPtrData->get();
		}
		else if (portIndex == 1 && _cameraReadModel) {
			// 使用单次定时器，在事件循环的下一个周期触发
			QTimer::singleShot(0, [this]() {
				if (_cameraReadModel) {
					_cameraReadModel->readFrame();
				}
				});
		}
	}
	else if (portIndex == 0) {
		_cameraReadModel = nullptr;
	}
}
