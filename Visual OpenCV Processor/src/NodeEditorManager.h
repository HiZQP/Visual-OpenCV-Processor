#pragma once

#include <QtNodes/NodeDelegateModelRegistry>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include "CustomDataFlowGraphicsScene.h"

#include <QObject>
#include <QLayout>
#include <QString>
#include <memory>

class NodeEditorManager : public QObject
{
	Q_OBJECT
public:
	using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
	using RegistryItemCreator = std::function<RegistryItemPtr()>;
signals:
	void senceModified();
public slots:
private:
	std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _registry;
	std::shared_ptr<QtNodes::DataFlowGraphModel> _dataFlowGraphModel;
	std::unique_ptr<VCVP::DataFlowGraphicsScene> _scene;
	std::unique_ptr<QtNodes::GraphicsView> _view;

	std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
	void setStyle();
public:
	NodeEditorManager(QLayout* layout, QObject* parent = nullptr);
	~NodeEditorManager();

	NodeEditorManager(const NodeEditorManager&) = delete;
	NodeEditorManager& operator=(const NodeEditorManager&) = delete;
	NodeEditorManager(NodeEditorManager&&) = delete;
	NodeEditorManager& operator=(NodeEditorManager&&) = delete;

	void registerParamType(RegistryItemCreator creator);
	void createNode(const QString& paramId);
};