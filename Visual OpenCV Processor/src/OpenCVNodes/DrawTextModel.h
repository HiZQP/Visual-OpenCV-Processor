#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class DrawTextModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	QWidget* _widget;
	QLineEdit* _text;
	QSpinBox* _posX;
	QSpinBox* _posY;
	QSpinBox* _fontScale;
	QSpinBox* _thickness;
	QSpinBox* _colorR;
	QSpinBox* _colorG;
	QSpinBox* _colorB;
	QComboBox* _fontFace;
	QComboBox* _lineType;

	cv::Mat _originalImage;
	cv::Mat _textImage;
	void calculate();
public:
	DrawTextModel();
	virtual ~DrawTextModel() override {}
	QString caption() const override { return "文本绘制组件"; }
	QString name() const override { return "绘制文本"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget; }
};

