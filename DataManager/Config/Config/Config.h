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

    std::string getLibDir() {return m_vapp_dir;}

private:

    unsigned m_numberOfThreads;

    std::string m_vapp_dir;

};

#endif // CONFIG_H