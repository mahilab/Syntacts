#include <carnot>
#include <TactorFX/TactorFX.hpp>
#include <TactorFX/Detail/SPSCQueue.hpp>
#include <thread>

using namespace carnot;
using namespace rigtorp;

class Root : public GameObject {
public:
    Root(SPSCQueue<int>* q) : q(q) {

    }

    void update() override {
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::Begin("TactorFX", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   
        if (ImGui::Button(ICON_FA_PLAY)) {
            q->try_push(Engine::frame());
        }
        ImGui::End();
    }
    
    SPSCQueue<int>* q;
};

int main(int argc, char const *argv[])
{
    SPSCQueue<int> q(2);
    auto t = std::thread([&] {
        while (true) {
            if (q.front())
            {
                std::cout << *q.front() << std::endl;
                q.pop();
            }
        }
    });
    /// Initialize and run Engine
    Engine::init(500, 500, WindowStyle::Close);
    Engine::window->setTitle("Lockless Testing");
    Engine::makeRoot<Root>(&q);
    Engine::run();  
    return 0;
}
