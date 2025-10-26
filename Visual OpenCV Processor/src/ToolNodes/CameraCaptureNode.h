#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_CameraCaptureNode.h"

#include <QComboBox>
#include <QPushButton>
#include <QPointer>
#include <QProgressBar>
#include <QFutureWatcher>

// （一般情况）现在连接CameraReadModel和CaptureTriggerModel两个节点并反复点击捕获按钮会导致卡顿

class CameraCaptureNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
friend class CaptureTriggerNode;
private:
	Ui::CameraCaptureNode _ui;
	QWidget* _widget;
	bool _isCapturingFrames = false;

	QFutureWatcher<std::vector<int>> _searchWatcher;

	cv::VideoCapture _cap;
	cv::Mat _cameraFrame;
	void capture();
	void captureFrames();
	void beginSearch();
	void searchComplete();
private slots:
	void updateProgress(int value);
public:
	CameraCaptureNode();
	// 明确禁止拷贝，允许移动
	CameraCaptureNode(const CameraCaptureNode&) = delete;
	CameraCaptureNode& operator=(const CameraCaptureNode&) = delete;
	CameraCaptureNode(CameraCaptureNode&&) = default;
	CameraCaptureNode& operator=(CameraCaptureNode&&) = default;

	QString caption() const override { return "摄像头读取组件"; }
	QString name() const override { return "摄像头读取"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

class CaptureTriggerNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QPointer<CameraCaptureNode> _cameraReadModel;  // 使用 QPointer
public:
	CaptureTriggerNode() = default;
	virtual ~CaptureTriggerNode() override = default;
	// 明确禁止拷贝，允许移动
	CaptureTriggerNode(const CaptureTriggerNode&) = delete;
	CaptureTriggerNode& operator=(const CaptureTriggerNode&) = delete;
	CaptureTriggerNode(CaptureTriggerNode&&) = default;
	CaptureTriggerNode& operator=(CaptureTriggerNode&&) = default;

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
	CameraCaptureNode* _cameraReadModel;
public:
	CameraReadModelPtrData(CameraCaptureNode* cameraReadModel) : _cameraReadModel(cameraReadModel) {}
	QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{ "Terget", "触发" }; }
	CameraCaptureNode* get() const { return _cameraReadModel; }
};