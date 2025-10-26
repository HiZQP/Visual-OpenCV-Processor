#pragma once

#include <QtNodes/NodeDelegateModel>
#include "NodeDataType.h"
#include "ui_FileImageLoaderNode.h"

class FileImageLoaderNode : public QtNodes::NodeDelegateModel {
	Q_OBJECT

private:
	// 用于提供给emmeddedWidget方法的组件，应该包含该节点的所有交互组件
	Ui_FileImageLoaderNode _ui;
	QWidget* _widget;

	// 持有图像数据
	cv::Mat _image;
	void loadImageFromFile();
public:
	FileImageLoaderNode();
	// 节点名称
	QString caption() const override { return "图片加载组件"; }
	// 菜单名称
	QString name() const override { return "图片加载"; }
	// 端口数量
	virtual unsigned int nPorts(QtNodes::PortType portType) const override;
	// 端口类型
	virtual QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	virtual void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
	virtual QWidget* embeddedWidget() override { return _widget; }
	virtual bool resizable() const override { return true; }
};