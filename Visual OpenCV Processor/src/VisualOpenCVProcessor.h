#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualOpenCVProcessor.h"

#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>

#include <memory>

#include "OpenCVNodes/OpenCVNodes.h"
#include "ToolNodes/ToolNodes.h"

#include "CustomDataFlowGraphicsScene.h"
#include "ParameterManager.h"
#include "ParamTypes.h"

class VisualOpenCVProcessor : public QMainWindow
{
    Q_OBJECT
private:
    Ui::VisualOpenCVProcessorClass ui;
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _registry;
    std::shared_ptr<QtNodes::DataFlowGraphModel> _dataFlowGraphModel;
    std::unique_ptr<VCVP::DataFlowGraphicsScene> _scene;
    std::unique_ptr<QtNodes::GraphicsView> _view;

	std::shared_ptr<ParameterManager> _paramManager;

	void connects();
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
    void setStyle();
    void setupMainWindowWidget();
public:
    VisualOpenCVProcessor(QWidget *parent = nullptr);
    ~VisualOpenCVProcessor();

};