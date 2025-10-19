#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_AddWeightedNode.h"

class AddWeightedNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::AddWeightedNode _ui;
	QWidget* _widget;
	bool _normalization;

	cv::Mat _input1;
	cv::Mat _input2;
	cv::Mat _output;
	void calculate();
	void nomalization();
public:
	AddWeightedNode();
	virtual ~AddWeightedNode() override {}
	QString caption() const override { return "图像混合节点"; }
	QString name() const override { return "图像混合"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

