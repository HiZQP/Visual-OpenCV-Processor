#pragma once
#include <opencv2/opencv.hpp>
#include <QtNodes/NodeDelegateModel>
#include <QColor>
#include "NodeDataType.h"

#include <QComboBox>
#include <QPushButton>

class ColorPicker : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QPushButton* _button;
	QComboBox* _colorSpace;
	cv::Scalar _color;
	void calculate();
public:
	ColorPicker();
	virtual ~ColorPicker() override {}
	QString caption() const override { return "颜色选择器"; }
	QString name() const override { return "颜色选择器"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

