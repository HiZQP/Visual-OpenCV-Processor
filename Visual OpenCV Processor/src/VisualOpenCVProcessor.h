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
#include "NodeEditorManager.h"
#include "ParamTypes.h"

class VisualOpenCVProcessor : public QMainWindow
{
    Q_OBJECT
private:
    Ui::VisualOpenCVProcessorClass ui;

	std::shared_ptr<NodeEditorManager> _nodeEditorManager;
    std::shared_ptr<ParameterManager> _paramManager;

    void setupMainWindowWidget();
public:
    VisualOpenCVProcessor(QWidget *parent = nullptr);
    ~VisualOpenCVProcessor();
	QWidget* getCentralWidget() { return ui.centralWidget; }
};