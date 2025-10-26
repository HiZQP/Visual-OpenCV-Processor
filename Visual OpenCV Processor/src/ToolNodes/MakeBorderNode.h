#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_MakeBorderNode.h"

class MakeBorderNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::MakeBorderNode _ui;
	QWidget* _widget;

	cv::Mat _inputImage;
	cv::Mat _outputImage;
	void calculate();
public:
	MakeBorderNode();
	virtual ~MakeBorderNode() override {}
	QString caption() const override { return "图像边界扩展组件"; }
	QString name() const override { return "边界扩展"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

