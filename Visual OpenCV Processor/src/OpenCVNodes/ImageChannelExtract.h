#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QLabel>

class ImageChannelExtract : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QComboBox* _channelSelect;

	cv::Mat _originalImage;
	cv::Mat _channelImage;
	void calculate();
public:
	ImageChannelExtract();
	virtual ~ImageChannelExtract() override {}
	QString caption() const override { return "图像通道提取组件"; }
	QString name() const override { return "通道提取"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

