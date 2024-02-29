#include <cmath>

#include "AlgoBase.h"

fun::AlgoBase::AlgoBase(const std::shared_ptr<Config> &config) :
    m_x(0.0),
    m_y(0.0),
    m_z(0.0),
    m_eta(0.0),
    m_phi(0.0)
{
    m_config = config;
}

void fun::AlgoBase::base(const size_t &idx){
    dm::point &p = m_config->getCloud()->at(idx);

    m_config->setCloudCordNext(m_config->getCoord(p.x, p.y, p.z, m_config->gran()), idx);
}

void fun::AlgoBase::rotation(const size_t &idx){
    dm::point &p = m_config->getCloud()->at(idx);

    // Convert angles from degrees to radians
    float etaRad = m_eta * M_PI / 180.0;
    float betaRad = m_beta * M_PI / 180.0;
    float phiRad = m_phi * M_PI / 180.0;

    // Rotation around the x-axis
    float yRotx = p.y * cos(etaRad) - p.z * sin(etaRad);
    float zRotx = p.y * sin(etaRad) + p.z * cos(etaRad);
    p.y = yRotx;
    p.z = zRotx;

    // Rotation around the Y-axis
    float xRoty = p.x * cos(betaRad) + p.z * sin(betaRad);
    float zRoty = -p.x * sin(betaRad) + p.z * cos(betaRad);
    p.x = xRoty;
    p.z = zRoty;

    // Rotation around the Z-axis
    float xRotz = p.x * cos(phiRad) - p.y * sin(phiRad);
    float yRotz = p.x * sin(phiRad) + p.y * cos(phiRad);
    p.x = xRotz;
    p.y = yRotz;

    m_config->setCloudCordNext(m_config->getCoord(p.x, p.y, p.z, m_config->gran()), idx);
}

void fun::AlgoBase::translation(const size_t &idx){
    dm::point &p = m_config->getCloud()->at(idx);

    p.x += m_x;
    p.y += m_y;
    p.z += m_z;

    m_config->setCloudCordNext(m_config->getCoord(p.x, p.y, p.z, m_config->gran()), idx);
}

void fun::AlgoBase::displacement(const size_t &idx){
    dm::point &p = m_config->getCloud()->at(idx);

    p.x *= 1.0 / (1.0 - m_x);
    p.y *= 1.0 / (1.0 - m_y);
    p.z *= 1.0 / (1.0 - m_z);

    m_config->setCloudCordNext(m_config->getCoord(p.x, p.y, p.z, m_config->gran()), idx);
}

void fun::AlgoBase::sphereMove(const size_t &idx) {

    float x = m_config->getCloud()->at(idx).x + m_config->speed() * m_config->getCloud()->at(idx).vx;
    float y = m_config->getCloud()->at(idx).y + m_config->speed() * m_config->getCloud()->at(idx).vy;
    float z = m_config->getCloud()->at(idx).z + m_config->speed() * m_config->getCloud()->at(idx).vz;

    std::string key = m_config->getCoord(x, y, z, m_config->gran());

    unsigned loc = m_config->findCloudCord(key);

    if (loc && loc != idx) {
        float vx = m_config->getCloud()->at(loc).vx;
        float vy = m_config->getCloud()->at(loc).vy;
        float vz = m_config->getCloud()->at(loc).vz;

        m_config->getCloud()->at(loc).vx = (-1.0 * m_config->getCloud()->at(loc).vx + m_config->getCloud()->at(idx).vx) * 0.5;
        m_config->getCloud()->at(loc).vy = (-1.0 * m_config->getCloud()->at(loc).vy + m_config->getCloud()->at(idx).vy) * 0.5;
        m_config->getCloud()->at(loc).vz = (-1.0 * m_config->getCloud()->at(loc).vz + m_config->getCloud()->at(idx).vz) * 0.5;

        m_config->getCloud()->at(idx).vx = (-1.0 * m_config->getCloud()->at(idx).vx + vx) * 0.5;
        m_config->getCloud()->at(idx).vy = (-1.0 * m_config->getCloud()->at(idx).vy + vy) * 0.5;
        m_config->getCloud()->at(idx).vz = (-1.0 * m_config->getCloud()->at(idx).vz + vz) * 0.5;

        x = m_config->getCloud()->at(idx).x + m_config->speed() * m_config->getCloud()->at(idx).vx;
        y = m_config->getCloud()->at(idx).y + m_config->speed() * m_config->getCloud()->at(idx).vy;
        z = m_config->getCloud()->at(idx).z + m_config->speed() * m_config->getCloud()->at(idx).vz;

        key = m_config->getCoord(x, y, z, m_config->gran());

        std::cout << "Collision for " << idx << " x " << loc << " " << key << " " << &m_config->getCloud()->at(loc) << " " << &m_config->getCloud()->at(idx) << std::endl;
    }

    m_config->getCloud()->at(idx).x = x;
    m_config->getCloud()->at(idx).y = y;
    m_config->getCloud()->at(idx).z = z;

    if (x > m_config->boundMax() - m_config->speed() || x < m_config->boundMin() + m_config->speed()) 
        {m_config->getCloud()->at(idx).vx *= -1;}
    if (y > m_config->boundMax() - m_config->speed() || y < m_config->boundMin() + m_config->speed()) 
        {m_config->getCloud()->at(idx).vy *= -1;}
    if (z > m_config->boundMax() - m_config->speed() || z < m_config->boundMin() + m_config->speed()) 
        {m_config->getCloud()->at(idx).vz *= -1;}

    m_config->setCloudCordNext(key, idx);
}