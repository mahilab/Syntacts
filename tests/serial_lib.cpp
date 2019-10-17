#include "serial_lib.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>

namespace fs = std::filesystem;

CEREAL_REGISTER_TYPE(mylib::Derived);

namespace mylib {

Base::Base() {}
Base::Base(int x) : m_x(x) {}
void Base::go() {
    std::cout << m_x << std::endl;
}

Derived::Derived() {}
Derived::Derived(int x, int y) : Base(x), m_y(y) {} 
void Derived::go() { 
    std::cout << m_x << ", " << m_y << std::endl; 
}

void save(const std::shared_ptr<Base>& base, const std::string& filename) {
    std::ofstream file(filename);
    cereal::JSONOutputArchive archive(file);
    archive(base);  
}

void load(      std::shared_ptr<Base>& base, const std::string& filename) {
    std::ifstream file(filename);
    cereal::JSONInputArchive archive(file);
    archive(base);
}

}