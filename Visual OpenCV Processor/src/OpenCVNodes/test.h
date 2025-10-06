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
	���ڴ����ģ�������еĽڵ�ID
	NodeId ���� unsigned int
	*/
	std::unordered_set<NodeId> _ids;
	/*
	���ڴ����ģ�������е�����ID
	ConnectionId �ṹ��������³�Ա��
		NodeId outNodeId;    // ����ڵ��ID
		PortIndex outPortIndex; // ����˿ڵ�����
		NodeId inNodeId;     // ����ڵ��ID
		PortIndex inPortIndex;  // ����˿ڵ�����
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