#ifndef ALGOBASE_H
#define ALGOBASE_H

#include <functional>
#include <unordered_map>

#include "Cloud.h"

using algo = std::function<dm::point(const dm::point &)>;

namespace fun {
    class AlgoBase {
    public:
        AlgoBase();

        dm::point base(const dm::point &x);

        dm::point rotation(const dm::point &x);

        dm::point translation(const dm::point &x);

        dm::point displacement(const dm::point &x);

        std::unordered_map<std::string, unsigned> sphereMove(
            dm::cloud * c, 
            const std::unordered_map<std::string, unsigned> &umap,
            float sphere_volume,
            float speed
        );

        // Define constant variables

        void setX(float x) {m_x = x;}
        void setY(float y) {m_y = y;}
        void setZ(float z) {m_z = z;}
        void setEta(float eta) {m_eta = eta;}
        void setPhi(float phi) {m_phi = phi;}

    private:
        float m_x;
        float m_y;
        float m_z;
        float m_eta;
        float m_phi;
    };
}

#endif // ALGOBASE_H