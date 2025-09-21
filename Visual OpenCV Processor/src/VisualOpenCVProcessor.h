#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualOpenCVProcessor.h"

#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>

#include <memory>

#include "OpenCVNodes/OpenCVNodes.h"
#include "ToolNodes/ToolNodes.h"

class VisualOpenCVProcessor : public QMainWindow
{
    Q_OBJECT
private:
    Ui::VisualOpenCVProcessorClass ui;
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> m_registry;
    std::shared_ptr<QtNodes::DataFlowGraphModel> m_dataFlowGraphModel;  // ÐÂÔö
    std::unique_ptr<QtNodes::DataFlowGraphicsScene> m_scene;
    std::unique_ptr<QtNodes::GraphicsView> m_view;

	void connects();
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
    void setStyle();
    void setupMainWindowWidget();
public:
    VisualOpenCVProcessor(QWidget *parent = nullptr);
    ~VisualOpenCVProcessor();

};

