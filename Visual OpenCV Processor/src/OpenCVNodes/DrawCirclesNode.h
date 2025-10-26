#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include "ui_DrawCirclesNode.h"

#include <QColorDialog>

class DrawCirclesNode : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	Ui::DrawCirclesNode _ui;
	QWidget* _widget;

	QColor _color;
	cv::Mat _originalImage;
	cv::Mat _outputImage;
	std::vector<cv::Vec4f> _circles;
	void calculate();
	void openColorDialog();
public:
	DrawCirclesNode();
	virtual ~DrawCirclesNode() override = default;
	QString caption() const override { return "圆集绘制组件"; }
	QString name() const override { return "圆集绘制"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

