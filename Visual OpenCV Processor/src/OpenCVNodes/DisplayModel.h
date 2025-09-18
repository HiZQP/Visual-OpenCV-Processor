#pragma once
#include <QtNodes/NodeDelegateModel>

#include <QLineEdit>
#include <QLabel>

#include "NodeDataType.h"

class DisplayModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT
public:
	DisplayModel() {}
	~DisplayModel() override {}
	unsigned int nPorts(QtNodes::PortType portType) const override {
		if (portType == QtNodes::PortType::In)
			return 1;
		else
			return 0;
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override { return nullptr; }
};

class TextDisplayModel : public DisplayModel {
	Q_OBJECT
private:
	QLabel* _text;

public:
	TextDisplayModel();
	~TextDisplayModel() override {}

	QString caption() const override { return "文本显示"; }
	QString name() const override { return "文本显示"; }

	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	QWidget* embeddedWidget() override { return _text; }
};

class ImageDisplayModel : public DisplayModel {
	Q_OBJECT
private:
	QWidget* _widget;
	QLabel* _size;
	QLabel* _imageLabel;
public:
	ImageDisplayModel();
	~ImageDisplayModel() override {}
	QString caption() const override { return "图像显示"; }
	QString name() const override { return "图像显示"; }
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override;
	QWidget* embeddedWidget() override { return _widget; }
};