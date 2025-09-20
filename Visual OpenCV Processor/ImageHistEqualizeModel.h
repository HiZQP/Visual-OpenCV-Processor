#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLabel>

class ImageHistEqualizeModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QLabel* _infoLabel;
	cv::Mat _originalImage;
	cv::Mat _equalizedImage;
	void calculate();
public:
	ImageHistEqualizeModel();
	virtual ~ImageHistEqualizeModel() override {}
	QString caption() const override { return "图像直方图均衡化组件"; }
	QString name() const override { return "直方图均衡化"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

