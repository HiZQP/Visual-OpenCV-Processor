#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_MorphologyFindLineNode.h"

class MorphologyFindLineNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::MorphologyFindLineNode _ui;
	QWidget* _widget;

	cv::Mat _inputImage;
	cv::Mat _outputImage;
	void calculate();
public:
	MorphologyFindLineNode();
	virtual ~MorphologyFindLineNode() override {}
	QString caption() const override { return "形态学直线检测"; }
	QString name() const override { return "形态学直线检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

