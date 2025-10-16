#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QLabel>
#include <QPushButton>

#include "NodeDataType.h"

#define QUTF8(s) QString::fromUtf8(s) 

class ImageLoaderModel : public QtNodes::NodeDelegateModel {
	Q_OBJECT

private:
	// 用于提供给emmeddedWidget方法的组件，应该包含该节点的所有交互组件
	QWidget* m_widget;
	// 节点组件
	QLabel* m_label;
	QPushButton* m_button;
	// 持有图像数据
	cv::Mat m_image;
	
public:
	ImageLoaderModel();
	~ImageLoaderModel() override = default;

	// 节点名称
	QString caption() const override { return QUTF8("图片加载组件"); }
	// 菜单名称
	QString name() const override { return QUTF8("从文件加载"); }

	// 端口数量
	virtual unsigned int nPorts(QtNodes::PortType portType) const override;
	// 端口类型
	virtual QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
	virtual void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

	virtual QWidget* embeddedWidget() override { return m_widget; }

	virtual bool resizable() const override { return true; }

};