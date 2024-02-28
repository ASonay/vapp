#include <cmath>

#include "AlgoBase.h"

fun::AlgoBase::AlgoBase() :
    m_x(0.0),
    m_y(0.0),
    m_z(0.0),
    m_eta(0.0),
    m_phi(0.0)
{

}

dm::point fun::AlgoBase::base(const dm::point &x){
    return x;
}

dm::point fun::AlgoBase::rotation(const dm::point &x){
    dm::point p;

    p.R = x.R;
    p.G = x.G;
    p.B = x.B;

    // Convert angles from degrees to radians
    float etaRad = m_eta * M_PI / 180.0;
    float betaRad = m_beta * M_PI / 180.0;
    float phiRad = m_phi * M_PI / 180.0;

    // Rotation around the x-axis
    p.y = x.y * cos(etaRad) - x.z * sin(etaRad);
    p.z = x.y * sin(etaRad) + x.z * cos(etaRad);

    // Rotation around the Y-axis
    float xRoty = x.x * cos(betaRad) + p.z * sin(betaRad);
    float zRoty = -x.x * sin(betaRad) + p.z * cos(betaRad);
    p.x = xRoty;
    p.z = zRoty;

    // Rotation around the Z-axis
    float xRotz = p.x * cos(phiRad) - p.y * sin(phiRad);
    float yRotz = p.x * sin(phiRad) + p.y * cos(phiRad);
    p.x = xRotz;
    p.y = yRotz;

    return p;
}

dm::point fun::AlgoBase::translation(const dm::point &x){
    dm::point p;

    p.R = x.R;
    p.G = x.G;
    p.B = x.B;

    p.x = x.x + m_x;
    p.y = x.y + m_y;
    p.z = x.z + m_z;

    return p;
}

dm::point fun::AlgoBase::displacement(const dm::point &x){
    dm::point p;

    p.R = x.R;
    p.G = x.G;
    p.B = x.B;

    p.x = x.x * 1.0 / (1.0 - m_x);
    p.y = x.y * 1.0 / (1.0 - m_y);
    p.z = x.z * 1.0 / (1.0 - m_z);

    return p;
}

std::unordered_map<std::string, unsigned> fun::AlgoBase::sphereMove(
    dm::cloud * c, 
    const std::unordered_map<std::string, unsigned> &umap,
    float sphere_volume,
    float speed
) {
    std::unordered_map<std::string, unsigned> umap_tmp;

    for (unsigned i=0; i<800; ++i) {

        float x = c->at(i).x + speed * c->at(i).vx;
        float y = c->at(i).y + speed * c->at(i).vy;
        float z = c->at(i).z + speed * c->at(i).vz;

        int g_x = static_cast<int>((x / sphere_volume) * 10) / 10;
        int g_y = static_cast<int>((y / sphere_volume) * 10) / 10;
        int g_z = 0;

        std::string key = std::to_string(g_x) + std::to_string(g_y) + std::to_string(g_z);
        auto search = umap.find(key);

        if (search != umap.end()) {
            unsigned loc = search->second;

            if (loc != i) {
                float vx = c->at(loc).vx;
                float vy = c->at(loc).vy;
                float vz = c->at(loc).vz;

                c->at(loc).vx = (-1.0 * c->at(loc).vx + c->at(i).vx) * 0.5;
                c->at(loc).vy = (-1.0 * c->at(loc).vy + c->at(i).vy) * 0.5;
                c->at(loc).vz = (-1.0 * c->at(loc).vz + c->at(i).vz) * 0.5;

                c->at(i).vx = (-1.0 * c->at(i).vx + vx) * 0.5;
                c->at(i).vy = (-1.0 * c->at(i).vy + vy) * 0.5;
                c->at(i).vz = (-1.0 * c->at(i).vz + vz) * 0.5;

                x = c->at(i).x + speed * c->at(i).vx;
                y = c->at(i).y + speed * c->at(i).vy;
                z = c->at(i).z + speed * c->at(i).vz;

                g_x = static_cast<int>((x / sphere_volume) * 10) / 10;
                g_y = static_cast<int>((y / sphere_volume) * 10) / 10;
                g_z = 0;

                key = std::to_string(g_x) + std::to_string(g_y) + std::to_string(g_z);
                std::cout << "Collision for " << i << " x " << loc << " " << key << " " << &c->at(loc) << " " << &c->at(i) << std::endl;
            }
        }

        c->at(i).x = x;
        c->at(i).y = y;
        c->at(i).z = z;

        if (x > 1 - speed || x < - 1 + speed) {c->at(i).vx *= -1;}
        if (y > 1 - speed || y < - 1 + speed) {c->at(i).vy *= -1;}
        if (z > 1 - speed || z < - 1 + speed) {c->at(i).vz *= -1;}

        umap_tmp[key] = i;
    }

    return umap_tmp;
}