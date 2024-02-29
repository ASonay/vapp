#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

#include "Cloud.h"

class Config : public Cloud {

public:
    
    Config(const std::string &name);
    ~Config();

    // Some common variables can be put

    void setNumberOfThreads(unsigned numberOfThreads) {m_numberOfThreads = numberOfThreads;}
    unsigned numberOfThreads() {return m_numberOfThreads;}

    void setBoundMin(float boundMin) {m_boundMin = boundMin;}
    float boundMin() {return m_boundMin;}

    void setBoundMax(float boundMax) {m_boundMax = boundMax;}
    float boundMax() {return m_boundMax;}

    void setSpeed(float speed) {m_speed = speed;}
    float speed() {return m_speed;}

    void setGran(float gran) {m_gran = gran;}
    float gran() {return m_gran;}

    std::string getLibDir() {return m_vapp_dir;}

private:

    unsigned m_numberOfThreads;

    float m_boundMin;
    float m_boundMax;

    float m_speed;
    float m_gran;

    std::string m_vapp_dir;

};

#endif // CONFIG_H