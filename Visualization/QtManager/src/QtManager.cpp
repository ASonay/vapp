#include <unistd.h>

#include "QtManager.h"

QtManager::QtManager(QWidget *parent) :
    QMainWindow(parent),
    m_sphere_volume(0.002),
    b_part1(true),
    b_part2(false),
    b_part3(false),
    m_play(-1)
{
    m_centralWidget.reset(new QWidget(this));
}

void QtManager::setWindow()
{
    // Top Layouts
    auto hLayout_1 = new QHBoxLayout();
    auto hLayout_2 = new QHBoxLayout();
    hLayout_1->setAlignment(Qt::AlignTop);
    hLayout_2->setAlignment(Qt::AlignTop);

    // Create buttons for top layout
    for (auto & item : {"Rotate", "Translate", "Displacement", "Start", "Stop"}) {
        m_buttons[item] = std::make_unique<QPushButton>(item);
        hLayout_1->addWidget(m_buttons[item].get());
        connect(m_buttons[item].get(), &QPushButton::clicked, this, [this, item](){
            std::string s = item;
            onClick(s);
        });
    }

    // Create input fields
    for (auto & item : {"Eta", "Phi", "X", "Y", "Z", "Speed"}){
        m_inputFields[item] = std::make_unique<QLineEdit>();
        m_inputFields[item]->setPlaceholderText(item);
        hLayout_2->addWidget(m_inputFields[item].get());
    }
    hLayout_2->setSpacing(10);

    // Connect button signal to slot

    auto *lLayout = new QVBoxLayout();
    for (auto & key : {"Part 1", "Part 2", "Part 3"}) {
        m_buttons[key] = std::make_unique<QPushButton>(key);
        lLayout->addWidget(m_buttons[key].get());
        connect(m_buttons[key].get(), &QPushButton::clicked, this, [this, key](){
            std::string s = key;
            onClick(s);
        });
    }

    // Create Qt3DWindow for 3D content
    m_view.reset(new Qt3DExtras::Qt3DWindow());
    m_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(m_view.get(), this);

    // Set minimum size for the container
    auto *rLayout = new QVBoxLayout();
    rLayout->setAlignment(Qt::AlignBottom);

    container->setMinimumSize(QSize(400, 500));
    rLayout->addWidget(container);

    // Main layout
    auto *mainLayout = new QGridLayout(m_centralWidget.get());

    // Add top layouts
    mainLayout->addLayout(hLayout_1, 0, 0, 1, 2); // Span both columns
    mainLayout->addLayout(hLayout_2, 1, 0, 1, 2); // Span both columns

    // Add left panel and right bottom container
    mainLayout->addLayout(lLayout, 2, 0); // Row 2, Column 0
    mainLayout->addLayout(rLayout, 2, 1); // Row 2, Column 1

    // Adjust column stretch factors if needed
    mainLayout->setColumnStretch(0, 1); // Gives the left panel a fixed width
    mainLayout->setColumnStretch(1, 3); // Allows the right bottom container to take more space

    m_centralWidget->setLayout(mainLayout);

    // Root entity
    m_rootEntity.reset(new Qt3DCore::QEntity());

    // Point Cloud
    m_cloudEntity.reset(createPointCloud());

    // Update Point Clouds
    updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.base(p); });

    // Set the scene root entity
    m_view->setRootEntity(m_rootEntity.get());

    // Set camera
    setCamera(QVector3D(0, 1.0, -3.0), QVector3D(0, 1.2, 0));

    // Set the central widget and layout
    this->setCentralWidget(m_centralWidget.get());
}

void QtManager::setCamera(const QVector3D &pos, const QVector3D &cent) {
    // Camera
    auto cameraEntity = m_view->camera();
    cameraEntity->setPosition(pos);
    cameraEntity->setViewCenter(cent);

    // Camera controller
    auto camController = new Qt3DExtras::QOrbitCameraController(m_rootEntity.get());
    camController->setCamera(cameraEntity);

    // Lightning
    auto *lightEntity = new Qt3DCore::QEntity(m_rootEntity.get());
    auto *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.5);
    lightEntity->addComponent(light);
    auto *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);
}

void QtManager::onClick(std::string func) {

    if (func == "Rotate" && b_part1) {
        bool b_eta, b_phi;
        double eta = m_inputFields["Eta"]->text().toDouble(&b_eta);
        double phi = m_inputFields["Phi"]->text().toDouble(&b_phi);

        if (!b_eta) {eta=0.0;}
        if (!b_phi) {phi=0.0;}

        if (b_eta || b_phi) {
            m_algoBase.setEta(eta);
            m_algoBase.setPhi(phi);
            updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.rotation(p); });
        }
     }
    else if (func == "Translate" && b_part1) {
        bool b_x, b_y, b_z;
        double X = m_inputFields["X"]->text().toDouble(&b_x);
        double Y = m_inputFields["Y"]->text().toDouble(&b_y);
        double Z = m_inputFields["Z"]->text().toDouble(&b_z);

        if (!b_x) {X=0.0;}
        if (!b_y) {Y=0.0;}
        if (!b_z) {Z=0.0;}

        if (b_x || b_y || b_z) {
            m_algoBase.setX(X);
            m_algoBase.setY(Y);
            m_algoBase.setZ(Z);

            updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.translation(p); });
        }
     }
    else if (func == "Displacement" && b_part1) {
        bool b_x, b_y, b_z;
        double X = m_inputFields["X"]->text().toDouble(&b_x);
        double Y = m_inputFields["Y"]->text().toDouble(&b_y);
        double Z = m_inputFields["Z"]->text().toDouble(&b_z);

        if (!b_x) {X=0.0;}
        if (!b_y) {Y=0.0;}
        if (!b_z) {Z=0.0;}

        if (b_x || b_y || b_z) {
            m_algoBase.setX(X);
            m_algoBase.setY(Y);
            m_algoBase.setZ(Z);

            updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.displacement(p); });
        }
     }
    else if (func == "Stop") {
        m_play = 0;
    }
    else if (func == "Start" && b_part2) {
        m_play = 1;

        bool b_speed;
        double speed = m_inputFields["Speed"]->text().toDouble(&b_speed);

        if (!b_speed) {speed=0.001;}
        
        while (m_play) {
            m_sphere_cloud = m_algoBase.sphereMove(
                m_config->getCloud(),
                m_sphere_cloud,
                m_sphere_volume,
                speed
            );
            updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.base(p); });
            QCoreApplication::processEvents(); // Keep UI responsive
        }

     }
     else if (func == "Part 1") {
        if (!m_config->readFile(m_config->getLibDir()+"/Data/cloud.ply")) {
            std::cerr << "Can not read the file" << std::endl;
        }
        else {
            std::cerr << "Successfully read file" << std::endl;
        }
        m_cloudEntity.reset(createPointCloud());
        updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.base(p); });
        setCamera(QVector3D(0, 0, -4.0), QVector3D(0, 1.2, 0));
        b_part1 = true;
        b_part2 = false;
        b_part3 = false;
     }
     else if (func == "Part 2") {
        m_sphere_cloud = m_config->createCloud(800, m_sphere_volume);
        m_cloudEntity.reset(createPointCloud());
        updatePoints([this](const dm::point& p) -> dm::point { return m_algoBase.base(p); });
        setCamera(QVector3D(0, 0.0, -5.0), QVector3D(0, 0, 0));
        b_part2 = true;
        b_part1 = false;
        b_part3 = false;
     }
     else {
        std::cerr << "Do nothing!" << std::endl;
     }
}

Qt3DCore::QEntity* QtManager::createPointCloud() {

    m_material = new Qt3DExtras::QPhongMaterial(m_rootEntity.get());
    m_material->setAmbient(Qt::white);

    m_geometry = new Qt3DRender::QGeometry(m_rootEntity.get());

    m_vertexBuffer = new Qt3DRender::QBuffer();

    m_positionAttribute = new Qt3DRender::QAttribute();
    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(6 * sizeof(float));
    m_positionAttribute->setCount(m_config->getNVertex());


    m_colorAttribute = new Qt3DRender::QAttribute();
    m_colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());
    m_colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    m_colorAttribute->setVertexSize(3);
    m_colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_colorAttribute->setBuffer(m_vertexBuffer);
    m_colorAttribute->setByteStride(6 * sizeof(float));
    m_colorAttribute->setByteOffset(3 * sizeof(float));
    m_colorAttribute->setCount(m_config->getNVertex());

    m_geometry->addAttribute(m_positionAttribute);
    m_geometry->addAttribute(m_colorAttribute);

    m_geometryRenderer = new Qt3DRender::QGeometryRenderer;
    m_geometryRenderer->setGeometry(m_geometry);
    m_geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);

    auto *entity = new Qt3DCore::QEntity(m_rootEntity.get());

    entity->addComponent(m_geometryRenderer);
    entity->addComponent(m_material);

    return entity;
}

void QtManager::updateArray(unsigned start, unsigned end, algo a) {

    for (unsigned i = start; i < end; ++i) {
        m_config->getCloud()->at(i) = a(m_config->getCloud()->at(i));
        // Position
        m_rawVertexArray[6*i+0] = m_config->getCloud()->at(i).x;
        m_rawVertexArray[6*i+1] = m_config->getCloud()->at(i).y;
        m_rawVertexArray[6*i+2] = m_config->getCloud()->at(i).z;
        // Color (RGB normalized to 0-1 range)
        m_rawVertexArray[6*i+3] = m_config->getCloud()->at(i).R / 255.;
        m_rawVertexArray[6*i+4] = m_config->getCloud()->at(i).G / 255.;
        m_rawVertexArray[6*i+5] = m_config->getCloud()->at(i).B / 255.;
    }
}

void QtManager::updatePoints(algo a) {

    QByteArray bufferBytes;
    bufferBytes.resize(m_config->getNVertex() * (3 + 3) * sizeof(float));

    m_rawVertexArray = reinterpret_cast<float *>(bufferBytes.data());

    unsigned numberOfThreads = m_config->numberOfThreads(); 
    unsigned size = m_config->getNVertex();
    unsigned segmentSize = size / numberOfThreads;
    std::vector<std::thread> threads;

    for (unsigned i = 0; i < numberOfThreads; ++i) {
        unsigned start = i * segmentSize;
        unsigned end = (i + 1) * segmentSize;
        if (i == numberOfThreads - 1) {
            end = size; // Ensure the last thread covers the remainder.
        }

        threads.emplace_back([this, start, end, a] { this->updateArray(start, end, a); });
    }

    // Join threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    m_vertexBuffer->setData(bufferBytes);
}