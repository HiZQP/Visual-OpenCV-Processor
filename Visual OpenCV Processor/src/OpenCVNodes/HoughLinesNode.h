#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

class HoughLinesNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QSpinBox* _rho;
	QDoubleSpinBox* _theta;
	QSpinBox* _threshold;
	QSpinBox* _minLineLength;
	QDoubleSpinBox* _maxLineGap;
	QLabel* _infoLabel;

	cv::Mat _originalImage;
	std::vector<cv::Vec4i> _lines;
	void calculate();
public:
	HoughLinesNode();
	virtual ~HoughLinesNode() override {}
	QString caption() const override { return "霍夫直线检测组件"; }
	QString name() const override { return "霍夫直线检测"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};
