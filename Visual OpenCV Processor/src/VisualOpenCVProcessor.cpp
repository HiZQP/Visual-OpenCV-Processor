#include "VisualOpenCVProcessor.h"
#include <opencv2/opencv.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>

void VisualOpenCVProcessor::setupMainWindowWidget(){
    QHBoxLayout* hLayout = new QHBoxLayout(ui.centralWidget);

	_nodeEditorManager = std::make_shared<NodeEditorManager>(hLayout, this);

	connect(_nodeEditorManager.get(), &NodeEditorManager::senceModified, [this]() {
		ui.centralWidget->setWindowModified(true);
		});
	
	// 添加一个初始节点
	//QtNodes::NodeId id = _dataFlowGraphModel->addNode("从文件加载");
	//_dataFlowGraphModel->setNodeData(id, QtNodes::NodeRole::Position, QPointF(0, 0));
}

VisualOpenCVProcessor::VisualOpenCVProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupMainWindowWidget();
}

VisualOpenCVProcessor::~VisualOpenCVProcessor() {
}