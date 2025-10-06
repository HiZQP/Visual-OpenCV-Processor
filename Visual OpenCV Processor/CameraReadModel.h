#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QPushButton>

// （一般情况）现在连接CameraReadModel和CaptureTriggerModel两个节点并反复点击捕获按钮会导致卡顿

class CameraReadModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
friend class CaptureTriggerModel;
private:
	QWidget* _widget;
	QComboBox* _cameraIndex;
	QPushButton* _videoCaptureButton;

	cv::VideoCapture _cap;
	cv::Mat _cameraFrame;
	void readFrame();
public:
	CameraReadModel();
	virtual ~CameraReadModel() override {}
	QString caption() const override { return "摄像头读取组件"; }
	QString name() const override { return "摄像头读取"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

class CaptureTriggerModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	CameraReadModel* _cameraReadModel;
public:
	CaptureTriggerModel();
	virtual ~CaptureTriggerModel() override {}
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