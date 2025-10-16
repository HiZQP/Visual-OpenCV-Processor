#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QPushButton>
#include <QPointer>

// （一般情况）现在连接CameraReadModel和CaptureTriggerModel两个节点并反复点击捕获按钮会导致卡顿

class CameraReadModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
friend class CaptureTriggerModel;
private:
	std::unique_ptr<QWidget> _widget;
	QComboBox* _cameraIndex;
	QPushButton* _pictureCaptureButton;
	QPushButton* _videoCaptureButton;
	QPushButton* _searchCameraButton;
	bool _isCapturingFrames = false;

	cv::VideoCapture _cap;
	cv::Mat _cameraFrame;
	void capture();
	void captureFrames();
	void searchCameras();
public:
	CameraReadModel();
	virtual ~CameraReadModel() override = default;
	// 明确禁止拷贝，允许移动
	CameraReadModel(const CameraReadModel&) = delete;
	CameraReadModel& operator=(const CameraReadModel&) = delete;
	CameraReadModel(CameraReadModel&&) = default;
	CameraReadModel& operator=(CameraReadModel&&) = default;

	QString caption() const override { return "摄像头读取组件"; }
	QString name() const override { return "摄像头读取"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget.get(); }
};

class CaptureTriggerModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QPointer<CameraReadModel> _cameraReadModel;  // 使用 QPointer
public:
	CaptureTriggerModel() = default;
	virtual ~CaptureTriggerModel() override = default;
	// 明确禁止拷贝，允许移动
	CaptureTriggerModel(const CaptureTriggerModel&) = delete;
	CaptureTriggerModel& operator=(const CaptureTriggerModel&) = delete;
	CaptureTriggerModel(CaptureTriggerModel&&) = default;
	CaptureTriggerModel& operator=(CaptureTriggerModel&&) = default;

	QString caption() const override { return "捕获触发"; }
	QString name() const override { return "捕获触发"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override { return nullptr; }
	QWidget* embeddedWidget() override { return nullptr; }
};

class CameraReadModelPtrData : public QtNodes::NodeData
{
private:
	CameraReadModel* _cameraReadModel;
public:
	CameraReadModelPtrData(CameraReadModel* cameraReadModel) : _cameraReadModel(cameraReadModel) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Terget", "触发" }; }
	CameraReadModel* get() const { return _cameraReadModel; }
};