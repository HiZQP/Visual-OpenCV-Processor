#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_LaplacianEdgeDetectionModel.h"

class LaplacianEdgeDetectionModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::LaplacianEdgeDetectionModel _ui;
	QWidget* _widget;
	
	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	LaplacianEdgeDetectionModel();
	virtual ~LaplacianEdgeDetectionModel() override {}
	QString caption() const override { return "图像Laplacian边缘检测组件"; }
	QString name() const override { return "Laplacian边缘检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

