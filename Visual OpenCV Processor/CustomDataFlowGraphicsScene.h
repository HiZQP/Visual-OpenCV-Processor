#pragma once
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphModel>

namespace VCVP {

    class  DataFlowGraphicsScene : public QtNodes::BasicGraphicsScene
    {
        Q_OBJECT
    public:
        DataFlowGraphicsScene(QtNodes::DataFlowGraphModel& graphModel, QObject* parent = nullptr);
        ~DataFlowGraphicsScene() = default;

    public:
        std::vector<QtNodes::NodeId> selectedNodes() const;
        QMenu* createSceneMenu(QPointF const scenePos) override;

    public Q_SLOTS:
        bool save() const;
        bool load();

    Q_SIGNALS:
        void sceneLoaded();

    private:
        QtNodes::DataFlowGraphModel& _graphModel;
    };

} // namespace QtNodes
