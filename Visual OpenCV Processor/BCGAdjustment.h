#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_BCGAdjustment.h"

class BCGAdjustment : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::BCGAdjustment _ui;
	QWidget* _widget;
	bool _gamma = false;

	cv::Mat _originalImage;
	cv::Mat _outputImage;
	void calculate();
public:
	BCGAdjustment();
	virtual ~BCGAdjustment() override {}
	QString caption() const override { return "BCG调整组件"; }
	QString name() const override { return "BCG调整"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

