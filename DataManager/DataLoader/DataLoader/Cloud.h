#ifndef CLOUD_H
#define CLOUD_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace dm {

    struct point_d {
        double x, y, z;
        unsigned char R, G, B;
        float vx=0, vy=0, vz=0;
    };

    struct point {
        float x, y, z;
        unsigned char R, G, B;
        float vx=0, vy=0, vz=0;
    };

    typedef std::vector<point> cloud;
}

class Cloud {

public:

    Cloud(const std::string name);
    ~Cloud();
    
    bool readFile(
        const std::string &fileName,
        unsigned int = 3 * sizeof(float) + 3 * sizeof(unsigned char),
        std::string cord_type = "float"
    );

    dm::cloud *getCloud() {return m_cloud.get();}

    void resetCloud(size_t nVertex) {
        m_cloud.reset(new dm::cloud(nVertex));
        m_cloudCordNext.reset(new std::vector<std::pair<std::string, unsigned>>(nVertex));
        m_cloudCord.reset(new std::unordered_map<std::string, unsigned>(nVertex));
        m_vertex = nVertex;
    }

    size_t getNVertex() {return m_vertex;}

    void createCloud(
        const size_t &nPoint, 
        const float &sphere_volume, 
        const float &boundMin, 
        const float &boundMax
    );

    void setCloudCordNext(std::string key, unsigned idx) {
        m_cloudCordNext->at(idx) = std::make_pair(key, idx);
    }

    unsigned findCloudCord(const std::string &key);
    void resetCloudCordNext();

    std::string getCoord(const float &x, const float &y, const float &z, const float &sphere_volume);

private:

    std::unique_ptr<dm::cloud> m_cloud;

    std::string m_name;
    size_t m_vertex;

    std::unique_ptr<std::unordered_map<std::string, unsigned>> m_cloudCord;
    std::unique_ptr<std::vector<std::pair<std::string, unsigned>>> m_cloudCordNext;

};

#endif // CLOUD_H