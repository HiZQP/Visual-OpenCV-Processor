#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QLabel>

class ImageFindContoursModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QComboBox* _wmode;
	QComboBox* _method;
	QLabel* _infoLabel;

	cv::Mat _originalImage;
	std::vector<std::vector<cv::Point>> _contours;
	void calculate();
public:
	ImageFindContoursModel();
	virtual ~ImageFindContoursModel() override {}
	QString caption() const override { return "图像轮廓检测组件"; }
	QString name() const override { return "轮廓检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

