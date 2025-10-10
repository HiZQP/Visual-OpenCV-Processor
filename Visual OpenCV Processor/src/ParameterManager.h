#pragma once

#include <QWidget>
#include "ui_ParameterManager.h"

#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>

#include <QtNodes/DataFlowGraphModel>
#include <unordered_map>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QtNodes/NodeDelegateModelRegistry>
#include "NodeEditorManager.h"

template<typename T>
class ParamGetModel;

class ParameterManager : public QWidget, public std::enable_shared_from_this<ParameterManager>
{
public:
	using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
	using RegistryItemCreator = std::function<RegistryItemPtr()>;

	Q_OBJECT
signals:
	void registerParamType(QVariant creator);
	void createNode(const QString& name);
private:
	Ui::ParameterManagerClass ui;

	std::vector<QString> _paramTypes;
	std::unordered_map<QString, std::pair<QString, std::shared_ptr<QtNodes::NodeData>>> _params;
	std::shared_ptr<NodeEditorManager> _nodeEditorManager;

public:
	ParameterManager(QObject* parent, std::shared_ptr<NodeEditorManager> nodeEditorManager);
	~ParameterManager();

	ParameterManager(const ParameterManager&) = delete;
	ParameterManager& operator=(const ParameterManager&) = delete;
	ParameterManager(ParameterManager&&) = delete;
	ParameterManager& operator=(ParameterManager&&) = delete;

	template<typename T>
	typename std::enable_if<std::is_base_of<QtNodes::NodeData, T>::value, void>::type
	registerParamType() {
		T temp;
		QtNodes::NodeDataType type = temp.type();

		auto it = std::find(_paramTypes.begin(), _paramTypes.end(), type.name);
		if (it == _paramTypes.end()) {
			_paramTypes.push_back(type.name);
			auto paramTypeButton = new QPushButton(type.name + "(" + type.id + ")");
			ui.paramAdd_verticalLayout->addWidget(paramTypeButton);
			connect(paramTypeButton, &QPushButton::clicked, [this, type]() {

				// 使用 weak_ptr 来避免在构造函数中调用 shared_from_this()
				auto weakThis = std::weak_ptr<ParameterManager>(shared_from_this());

				static int count = 1;
				QString id = QString("%1_%2").arg(type.id).arg(count);
				RegistryItemCreator creator = [id, weakThis]() -> std::unique_ptr<QtNodes::NodeDelegateModel> {
					if (auto sharedThis = weakThis.lock()) {
						return std::make_unique<ParamGetModel<T>>(id, sharedThis);
					}
					return nullptr;
					};
				_nodeEditorManager->registerParamType(creator);

				QString name = QString("%1_%2").arg(type.name).arg(count);
				count++;
				this->addParam<T>(id, name);
				});
		}
	}

	template<typename T>
	typename std::enable_if<std::is_base_of<QtNodes::NodeData, T>::value, void>::type
		addParam(const QString& id ,const QString& name) {
		auto button = new QPushButton();
		button->setText(name);
		auto layout = new QHBoxLayout();
		button->setLayout(layout);

		QMenu* menu = new QMenu();
		auto getAction = menu->addAction("获取");
		auto setAction = menu->addAction("编辑");
		auto deleteAction = menu->addAction("删除");
		button->setMenu(menu);

		ui.paramList_verticalLayout->addWidget(button);

		_params[id].second = std::make_shared<T>(0);
		_params[id].first = name;

		connect(button, &QPushButton::click, button, &QPushButton::showMenu);
		connect(getAction, &QAction::triggered, [this, id]() {
			auto it = _params.find(id);
			if (it != _params.end())
				_nodeEditorManager->createNode(id);
			});
		connect(deleteAction, &QAction::triggered, [this, id, button]() {
			auto it = _params.find(id);
			if (it != _params.end()) {
				_params.erase(it);
				button->deleteLater();
			}
			});
	}

	std::shared_ptr<QtNodes::NodeData> getParam(const QString& name) {
		auto it = _params.find(name);
		if (it != _params.end()) {
			return std::shared_ptr<QtNodes::NodeData>(it->second.second.get());
		}
		return nullptr;
	}

	QString getName(const QString& id) {
		auto it = _params.find(id);
		if (it != _params.end()) {
			return it->second.first;
		}
		return QString();
	}
};

template<typename T>
class ParamGetModel : public QtNodes::NodeDelegateModel
{
	static_assert(std::is_base_of<QtNodes::NodeData, T>::value, "T must be derived from ParamTypeBase");
private:
	QString _paramId;
	std::weak_ptr<ParameterManager> _paramManager;
public:

	ParamGetModel() = default;
	ParamGetModel(const QString& id, std::shared_ptr<ParameterManager> paramManager) 
		: _paramId(id), _paramManager(paramManager) {}
	QString name() const override {	
		return _paramId;
	}
	QString caption() const override {
		auto pm = _paramManager.lock();
		if (pm) {
			return pm->getName(_paramId);
		}
	}
	unsigned int nPorts(QtNodes::PortType portType) const override {
		if (portType == QtNodes::PortType::In)
			return 0;
		else
			return 1;
	}
	QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
		if (portType == QtNodes::PortType::Out && portIndex == 0) {
			T temp;
			return temp.type();
		}
		return QtNodes::NodeDataType{};
	}
	std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override {
		if (port == 0) {
			if (auto pm = _paramManager.lock()) {
				auto param = pm->getParam(_paramId);
				if (param) {
					T* data = dynamic_cast<T*>(param.get());
					return std::make_shared<T>(data->get());
				}
			}
		}
		return nullptr;
	}
	void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex portIndex) override {}
	QWidget* embeddedWidget() override { return nullptr; }
};