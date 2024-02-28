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
    m_cloud.reset(new dm::cloud(vertexCount));
    m_vertex = vertexCount;

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

std::unordered_map<std::string, unsigned> Cloud::createCloud(size_t nPoint, float sphere_volume) {
    resetCloud(nPoint);

    std::unordered_map<std::string, unsigned> umap;

    unsigned count = 0;
    while (umap.size() < nPoint) {
        size_t csize = umap.size();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(-1.0f, 1.0f);

        float x = static_cast<float>(distr(gen));
        float y = static_cast<float>(distr(gen));
        // We want 2D surface
        float z = 0;

        int g_x = static_cast<int>((x / sphere_volume) * 10) / 10;
        int g_y = static_cast<int>((y / sphere_volume) * 10) / 10;
        int g_z = 0;

        std::string key = std::to_string(g_x) + std::to_string(g_y) + std::to_string(g_z);

        umap[key] = count;

        if (umap.size() > csize) {
            m_cloud->at(count++) = {
                .x = x,
                .y = y,
                .z = z,
                .R = 0,
                .G = 0,
                .B = 0,
                .vx = static_cast<float>(distr(gen)),
                .vy = static_cast<float>(distr(gen)),
                .vz = 0.0
            };
        }
    }

    return umap;
}