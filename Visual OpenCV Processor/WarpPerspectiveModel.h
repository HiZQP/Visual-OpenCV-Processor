#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QSpinBox>
#include <QPushButton>

class WarpPerspectiveModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _width;
	QSpinBox* _height;
	std::vector<cv::Point2f> _srcPoints;
	std::vector<cv::Point2f> _dstPoints;
	
	cv::Mat _originalImage;
	cv::Mat _warpedImage;
	void calculate();
public:
	WarpPerspectiveModel();
	virtual ~WarpPerspectiveModel() override = default;

	QString caption() const override { return "图像透视变换组件"; }
	QString name() const override { return "透视变换"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

