#include "CameraCaptureNode.h"

#include <memory>

#include <QHBoxLayout>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

void CameraCaptureNode::capture()
{
	if (_cap.isOpened()) {
		_cap >> _cameraFrame;
		Q_EMIT dataUpdated(1);
	}
	if (_isCapturingFrames) {
		QTimer::singleShot(0, this, &CameraCaptureNode::capture);
	}
}

void CameraCaptureNode::captureFrames()
{
	_isCapturingFrames = !_isCapturingFrames;
	if (_isCapturingFrames) {
		_ui.captureOneFrame_pushButton->setEnabled(false);
		_ui.keepCapture_pushButton->setText("停止捕获");
		capture();
	}
	else {
		_ui.captureOneFrame_pushButton->setEnabled(true);
		_ui.keepCapture_pushButton->setText("连续捕获");
	}
}

void CameraCaptureNode::beginSearch()
{
	_isCapturingFrames = false;
	_ui.captureOneFrame_pushButton->setEnabled(false);
	_ui.keepCapture_pushButton->setEnabled(false);
	_ui.search_pushButton->setEnabled(false);
	_ui.camera_comboBox->clear();
	_ui.progressBar->setValue(0);
	_ui.info_label->setText("正在查找摄像头...");
	auto future = QtConcurrent::run([this]() {
		std::vector<int> availableCameras;
		for (int i = 0; i < 10; ++i) {
			cv::VideoCapture cap(i);
			if (cap.isOpened()) {
				availableCameras.push_back(i);
				cap.release();
			}
			QMetaObject::invokeMethod(this, "updateProgress", Qt::QueuedConnection, Q_ARG(int, (i + 1) * 10));
		}
		return availableCameras;
		});
	_searchWatcher.setFuture(future);
}

void CameraCaptureNode::searchComplete()
{
	auto availableCameras = _searchWatcher.result();
	_ui.camera_comboBox->clear();
	if (availableCameras.empty()) {
		_ui.info_label->setStyleSheet("QLabel { color : red; }");
		_ui.info_label->setText("未找到摄像头");
		_ui.captureOneFrame_pushButton->setEnabled(false);
		_ui.keepCapture_pushButton->setEnabled(false);
	}
	else {
		for (auto index : availableCameras) {
			_ui.camera_comboBox->addItem(QString("摄像头 %1").arg(index));
		}
		_ui.info_label->setStyleSheet("QLabel { color : green; }");
		_ui.info_label->setText(QString("找到 %1 个摄像头").arg(availableCameras.size()));
		_ui.captureOneFrame_pushButton->setEnabled(true);
		_ui.keepCapture_pushButton->setEnabled(true);
		_cap.open(availableCameras[0]);
	}
	_ui.search_pushButton->setEnabled(true);
}

void CameraCaptureNode::updateProgress(int value)
{
	_ui.progressBar->setValue(value);
}

CameraCaptureNode::CameraCaptureNode()
{
	_widget = new QWidget();
	_ui.setupUi(_widget);

	_ui.info_label->setStyleSheet("QLabel { color : orange; }");
	_ui.info_label->setText("未查找摄像头");
	
	connect(_ui.captureOneFrame_pushButton, &QPushButton::clicked, this, &CameraCaptureNode::capture);
	connect(_ui.keepCapture_pushButton, &QPushButton::clicked, this, &CameraCaptureNode::captureFrames);
	connect(_ui.search_pushButton, &QPushButton::clicked, this, &CameraCaptureNode::beginSearch);
	connect(&_searchWatcher, &QFutureWatcher<std::vector<int>>::finished, this, &CameraCaptureNode::searchComplete);
	connect(_ui.camera_comboBox, &QComboBox::currentTextChanged, [this]() {
		_cap.release();
		_cap.open(_ui.camera_comboBox->currentIndex());
		_cameraFrame.release();
		Q_EMIT dataUpdated(1);
		});
}

unsigned int CameraCaptureNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::Out)
		return 2;
	else
		return 0;
}

QtNodes::NodeDataType CameraCaptureNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::Out) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Terget", "触发" };
		else
			return QtNodes::NodeDataType{ "Image", "图像输出" };
	}
	return QtNodes::NodeDataType{};

}




std::shared_ptr<QtNodes::NodeData> CameraCaptureNode::outData(QtNodes::PortIndex port)
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

unsigned int CaptureTriggerNode::nPorts(QtNodes::PortType portType) const
{
	if (portType == QtNodes::PortType::In)
		return 2;
	else 
		return 0;
}

QtNodes::NodeDataType CaptureTriggerNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	if (portType == QtNodes::PortType::In) {
		if (portIndex == 0)
			return QtNodes::NodeDataType{ "Terget", "目标" };
		else
			return QtNodes::NodeDataType{ "Image", "触发" };
	}
}

void CaptureTriggerNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex)
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
					_cameraReadModel->capture();
				}
				});
		}
	}
	else if (portIndex == 0) {
		_cameraReadModel = nullptr;
	}
}
