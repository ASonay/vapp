#include "Config.h"

Config::Config(const std::string &name) :
    Cloud(name),
    m_numberOfThreads(1)
{
    m_vapp_dir = getenv("VAPP");

    std::cout << "Lib path found: " << m_vapp_dir << std::endl;
}

Config::~Config()
{
}