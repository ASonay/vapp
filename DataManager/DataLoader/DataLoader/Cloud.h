#ifndef CLOUD_H
#define CLOUD_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace dm {
    struct point {
        float x, y, z;
        uint8_t R, G, B;
        float vx=0, vy=0, vz=0;
    };

    typedef std::vector<point> cloud;
}

class Cloud {

public:

    Cloud(const std::string name);
    ~Cloud();
    
    bool readFile(const std::string &fileName);

    dm::cloud *getCloud() {return m_cloud.get();}

    void resetCloud(size_t nVertex) {
        m_cloud.reset(new dm::cloud(nVertex));
        m_vertex = nVertex;
    }

    size_t getNVertex() {return m_vertex;}

    std::unordered_map<std::string, unsigned> createCloud(size_t nPoint, float sphere_volume);

private:

    std::unique_ptr<dm::cloud> m_cloud;

    std::string m_name;
    size_t m_vertex;

};

#endif // CLOUD_H