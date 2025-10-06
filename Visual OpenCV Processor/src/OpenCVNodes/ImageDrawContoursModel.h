#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

class ImageDrawContoursModel : public QtNodes::NodeDelegateModel
{
	Q_OBJECT
private:
	std::unique_ptr<QWidget> _widget;
	QCheckBox* _drawAll;
	QSpinBox* _indexSpinBox;

	cv::Mat _originalImage;
	std::vector<std::vector<cv::Point>> _contours;
	cv::Mat _outputImage;
	void calculate();
public:
	ImageDrawContoursModel();
	~ImageDrawContoursModel() override = default;

	// 明确禁止拷贝，允许移动
	ImageDrawContoursModel(const ImageDrawContoursModel&) = delete;
	ImageDrawContoursModel& operator=(const ImageDrawContoursModel&) = delete;
	ImageDrawContoursModel(ImageDrawContoursModel&&) = default;
	ImageDrawContoursModel& operator=(ImageDrawContoursModel&&) = default;

	QString caption() const override { return "图像绘制轮廓组件"; }
	QString name() const override { return "绘制轮廓"; }
	unsigned int nPorts(QtNodes::PortType portType) const override;
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	QWidget* embeddedWidget() override { return _widget.get(); }
};

