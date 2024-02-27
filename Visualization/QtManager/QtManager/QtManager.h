#ifndef QTMANAGER_H
#define QTMANAGER_H

#include <QMainWindow>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QFrameGraphNode>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QtGui/QScreen>
#include <Qt3DCore/QEntity>
#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QThread>
#include <QCoreApplication>

#include <QColor>
#include <QMainWindow>

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>

#include "Config.h"
#include "AlgoBase.h"

class QtManager : public QMainWindow {
    Q_OBJECT
public:
    QtManager(QWidget *parent = nullptr);
    virtual ~QtManager() {};

    void setConfig(const std::shared_ptr<Config> &config) {
        m_config = config;
        m_cloud = std::make_unique<dm::cloud>(*config->getCloud());
    }

    void setWindow();

private slots:
    void onClick(std::string func);

private:

    std::unique_ptr<QWidget> m_centralWidget;
    std::unique_ptr<Qt3DExtras::Qt3DWindow> m_view;
    std::unique_ptr<Qt3DCore::QEntity> m_rootEntity;
    std::unique_ptr<Qt3DCore::QEntity> m_cloudEntity;

    std::unordered_map<std::string, std::unique_ptr<QPushButton>> m_buttons;
    std::unordered_map<std::string, std::unique_ptr<QLineEdit>> m_inputFields;

    Qt3DRender::QBuffer *m_vertexBuffer;
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_colorAttribute;
    Qt3DRender::QGeometry *m_geometry;
    Qt3DRender::QGeometryRenderer *m_geometryRenderer;
    Qt3DExtras::QPhongMaterial *m_material;

    float *m_rawVertexArray;
    float m_sphere_volume;

    std::shared_ptr<Config> m_config;

    std::unique_ptr<dm::cloud> m_cloud;

    std::unordered_map<std::string, unsigned> m_sphere_cloud;

    Qt3DCore::QEntity* createPointCloud();
    void updatePoints(algo a);
    void updateArray(unsigned start, unsigned end, algo a);

    void setCamera(const QVector3D &pos, const QVector3D &cent);

    fun::AlgoBase m_algoBase;

    bool b_part1;
    bool b_part2;
    bool b_part3;

    int m_play;

};


#endif //QTMANAGER_H