#include <fstream>
#include <sstream>
#include <string>

#include <random>

#include "Cloud.h"

Cloud::Cloud(const std::string name):
    m_name(name),
    m_vertex(0)
{
}

Cloud::~Cloud()
{
}

bool Cloud::readFile(const std::string &fileName, unsigned int size, std::string cord_type)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    std::string line;
    size_t vertexCount = 0;
    bool headerEnded = false;

    // Read header
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "element") {
            iss >> token;
            if (token == "vertex") {
                iss >> vertexCount;
            }
        } else if (token == "end_header") {
            headerEnded = true;
            break;
        }
    }

    if (!headerEnded || vertexCount == 0) {
        std::cerr << "Invalid PLY header or zero vertices." << std::endl;
        return false;
    }

    // Read vertex data
    resetCloud(vertexCount);

    for (size_t i = 0; i < vertexCount; ++i) {
        if (cord_type == "double") {
            dm::point_d p;
            file.read(reinterpret_cast<char*>(&p), size);
            m_cloud->at(i) = {
                .x = static_cast<float>(p.x),
                .y = static_cast<float>(p.y),
                .z = static_cast<float>(p.z),
                .R = p.R,
                .G = p.G,
                .B = p.B,
            };
        }
        else {
            dm::point p;
            file.read(reinterpret_cast<char*>(&p), size);
            m_cloud->at(i) = p;
        }
    }

    return true;
}

void Cloud::createCloud(
    const size_t &nPoint, 
    const float &sphere_volume, 
    const float &boundMin, 
    const float &boundMax
) {
    resetCloud(nPoint);

    unsigned count = 0;
    while (m_cloudCord->size() < nPoint) {
        size_t csize = m_cloudCord->size();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(boundMin, boundMax);

        float x = static_cast<float>(distr(gen));
        float y = static_cast<float>(distr(gen));
        float z = static_cast<float>(distr(gen));

        std::string key = getCoord(x, y, z, sphere_volume);

        m_cloudCord->insert({key, count});

        if (m_cloudCord->size() > csize) {
            m_cloud->at(count++) = {
                .x = x,
                .y = y,
                .z = z,
                .R = 0,
                .G = 0,
                .B = 0,
                .vx = static_cast<float>(distr(gen)),
                .vy = static_cast<float>(distr(gen)),
                .vz = static_cast<float>(distr(gen))
            };
        }
    }

}

std::string Cloud::getCoord(const float &x, const float &y, const float &z, const float &sphere_volume) {
    int g_x = static_cast<int>((x / sphere_volume) * 10) / 10;
    int g_y = static_cast<int>((y / sphere_volume) * 10) / 10;
    int g_z = static_cast<int>((z / sphere_volume) * 10) / 10;

    return std::to_string(g_x) + std::to_string(g_y) + std::to_string(g_z);
}

unsigned Cloud::findCloudCord(const std::string &key) {
    auto search = m_cloudCord->find(key);

    if (search != m_cloudCord->end()) {
        return search->second;
    }
    else {
        return 0;
    }
}

void Cloud::resetCloudCordNext() {
    m_cloudCord->clear();
    m_cloudCord->reserve(m_cloudCordNext->size());
    m_cloudCord->insert(m_cloudCordNext->begin(), m_cloudCordNext->end());
}
