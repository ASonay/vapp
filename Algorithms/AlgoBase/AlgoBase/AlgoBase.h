#ifndef ALGOBASE_H
#define ALGOBASE_H

#include <functional>
#include <unordered_map>

#include "Cloud.h"
#include "Config.h"

using algo = std::function<void(const size_t &)>;

namespace fun {
    class AlgoBase {
    public:
        AlgoBase(const std::shared_ptr<Config> &config);

        void base(const size_t &idx);

        void rotation(const size_t &idx);

        void translation(const size_t &idx);

        void displacement(const size_t &idx);

        void sphereMove(const size_t &idx);

        // Define constant variables

        void setX(float x) {m_x = x;}
        void setY(float y) {m_y = y;}
        void setZ(float z) {m_z = z;}
        
        void setEta(float eta) {m_eta = eta;}
        void setBeta(float beta) {m_beta = beta;}
        void setPhi(float phi) {m_phi = phi;}

    private:
        float m_x;
        float m_y;
        float m_z;
        float m_eta;
        float m_beta;
        float m_phi;

        std::shared_ptr<Config> m_config;

    };
}

#endif // ALGOBASE_H