#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QSpinBox>

class InRange : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _lowB;
	QSpinBox* _lowG;
	QSpinBox* _lowR;
	QSpinBox* _highB;
	QSpinBox* _highG;
	QSpinBox* _highR;

	cv::Mat _originalImage;
	cv::Mat _inRangeImage;
	void calculate();
public:
	InRange();
	virtual ~InRange() override {}
	QString caption() const override { return "图像阈值组件"; }
	QString name() const override { return "阈值"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

