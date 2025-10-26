#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_MatCreateNode.h"

class MatCreateNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::MatCreateNode _ui;
	QWidget* _widget;
	bool _ratio;

	cv::Mat _output;
	void createMat();
	void ratio();
public:
	MatCreateNode();
	virtual ~MatCreateNode() override {}
	QString caption() const override { return "Mat图像创建节点"; }
	QString name() const override { return "Mat图像创建"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

