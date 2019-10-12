#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>
#include <iostream>
#include <carnot>

struct MyStruct
{
    int x, y, z;

    virtual void doIt() {
        carnot::print("MyStruct", x, y, z);
    }

    template <class Archive>
    void serialize(Archive &arch)
    {
        arch(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
    }
};

struct MyChildStruct : public MyStruct {
    int a = 4;
    virtual void doIt() override {
        carnot::print("MyChildStruct", a, x, y, z);
    }
    template <class Archive>
    void serialize(Archive &arch) {
        arch(cereal::make_nvp("MyStruct",cereal::base_class<MyStruct>(this)), CEREAL_NVP(a));
    }
};

struct MyGrandchildStruct : public MyChildStruct {
    int b = 69;
        virtual void doIt() override {
        carnot::print("MyGrandchildStruct", b, a, x, y, z);
    }
    template <class Archive>
    void serialize(Archive &arch) {
        arch(cereal::make_nvp("MyChildStruct",cereal::base_class<MyChildStruct>(this)), CEREAL_NVP(b));
    }
};

CEREAL_REGISTER_TYPE(MyChildStruct);
CEREAL_REGISTER_TYPE(MyGrandchildStruct);

template <typename Saveable>
void save(const Saveable &saveable, const std::string &filename, bool json = false)
{
    if (json) {
        std::ofstream file(filename);
        cereal::JSONOutputArchive archive(file);
        archive(saveable);    
    }
    else {
    std::ofstream file(filename, std::ios::binary);
        cereal::BinaryOutputArchive archive(file);
        archive(saveable);    
    }
}

template <typename Loadable>
void load(Loadable& loadable, const std::string& filename, bool json = false) {
    if (json) {
        std::ifstream file(filename);
        cereal::JSONInputArchive archive(file);
        archive(loadable);
    }
    else {
        std::ifstream file(filename, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        archive(loadable);
    }
}

int main()
{

    std::shared_ptr<MyStruct> j = std::make_shared<MyGrandchildStruct>();
    j->x = 33; j->y = 99; j->z = 55;

    j->doIt();

    save(j, "evan.json", true);

    std::shared_ptr<MyStruct> z;
    load(z, "evan.json", true);

    z->doIt();

    return 0;
}