#pragma once
#include <opencv2/opencv.hpp>
#include <QtNodes/NodeDelegateModel>
#include "NodeDataType.h"
#include "ui_ColorPickerNode.h"

#include <QColor>

class ColorPickerNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::ColorPickerNode _ui;
	QWidget* _widget;

	cv::Scalar _color;
	QColor _qcolor;
	void calculate();
public:
	ColorPickerNode();
	virtual ~ColorPickerNode() override {}
	QString caption() const override { return "拾色器"; }
	QString name() const override { return "拾色器"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

