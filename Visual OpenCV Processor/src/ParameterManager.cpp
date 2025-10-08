#include "ParameterManager.h"

ParameterManager::ParameterManager(QWidget* parent, std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry, std::shared_ptr<QtNodes::DataFlowGraphModel> dataFlowGraph)
	: QWidget(parent), _registry(registry), _dataFlowGraph(dataFlowGraph)
{
	ui.setupUi(this);
	ui.paramAdd_verticalLayout->setAlignment(Qt::AlignTop);
	ui.paramList_verticalLayout->setAlignment(Qt::AlignTop);
}

ParameterManager::~ParameterManager()
{
	_paramTypes.clear();
}