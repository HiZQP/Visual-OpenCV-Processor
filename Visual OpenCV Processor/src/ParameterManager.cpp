#include "ParameterManager.h"

ParameterManager::ParameterManager(QObject* parent, std::shared_ptr<NodeEditorManager> nodeEditorManager)
	: _nodeEditorManager(nodeEditorManager)
{
	ui.setupUi(this);
	ui.paramAdd_verticalLayout->setAlignment(Qt::AlignTop);
	ui.paramList_verticalLayout->setAlignment(Qt::AlignTop);
}

ParameterManager::~ParameterManager()
{
	_paramTypes.clear();
}