#include "ParameterManager.h"

ParameterManager& ParameterManager::getInstance()
{
	static ParameterManager instance;
	return instance;
}