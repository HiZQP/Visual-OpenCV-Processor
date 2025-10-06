#pragma once
#include <opencv2/opencv.hpp>
#include "NodeDataType.h"
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/AbstractGraphModel>

using ConnectionId = QtNodes::ConnectionId;
using ConnectionPolicy = QtNodes::ConnectionPolicy;
using NodeFlag = QtNodes::NodeFlag;
using NodeId = QtNodes::NodeId;
using NodeRole = QtNodes::NodeRole;
using PortIndex = QtNodes::PortIndex;
using PortRole = QtNodes::PortRole;
using PortType = QtNodes::PortType;
using StyleCollection = QtNodes::StyleCollection;


class DynamicPortsModel : public QtNodes::AbstractGraphModel
{
	Q_OBJECT
private:
	/*
	用于储存该模型中所有的节点ID
	NodeId 就是 unsigned int
	*/
	std::unordered_set<NodeId> _ids;
	/*
	用于储存该模型中所有的连接ID
	ConnectionId 结构体包含以下成员：
		NodeId outNodeId;    // 输出节点的ID
		PortIndex outPortIndex; // 输出端口的索引
		NodeId inNodeId;     // 输入节点的ID
		PortIndex inPortIndex;  // 输入端口的索引
	*/
	std::unordered_set<ConnectionId> _conns;
public:
	NodeId newNodeId() override {
		static NodeId id = 0;
		return ++id;
	}
	std::unordered_set<NodeId> allNodeIds() const override {
		return _ids;
	}
	std::unordered_set<ConnectionId> allConnectionIds(NodeId const nodeId) const override {
		std::unordered_set<ConnectionId> conns;
		for(auto conn : _conns) {
			if (conn.inNodeId == nodeId || conn.outNodeId == nodeId)
				conns.insert(conn);
		}
		return conns;
	}
};