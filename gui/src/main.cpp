// #define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU

#include "VisualizerWindow.hpp"
#include "ChannelWindow.hpp"
#include "DesignerWindow.hpp"
#include "DeviceBar.hpp"
#include "InfoBar.hpp"
#include "LibraryWindow.hpp"

class SyntactsGui : public GameObject {
public:

    SyntactsGui() {
        m_device     = makeChild<DeviceBar>("Device");
        m_visualizer = makeChild<VisualizerWindow>("Visualizer");
        m_info       = makeChild<InfoBar>("Info");
        m_designer   = makeChild<DesignerWindow>("Designer");
        m_channel    = makeChild<ChannelWindow>("Channel");
        m_library    = makeChild<LibraryWindow>("Library");
        positionWindows();
    }

private:

    Handle<InfoBar>          m_info;
    Handle<LibraryWindow>    m_library;
    Handle<DeviceBar>        m_device;
    Handle<DesignerWindow>   m_designer;
    Handle<ChannelWindow>    m_channel;
    Handle<VisualizerWindow> m_visualizer;

    void start() override {
        ImGuiStyle * imStyle = &ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Border] = colors[ImGuiCol_TabActive];
        colors[ImGuiCol_Separator] = colors[ImGuiCol_TabActive];
        colors[ImGuiCol_DragDropTarget] = colors[ImGuiCol_Text];
    }

    void positionWindows() {
        auto windowSize = Vector2f(900,540);
        float margin      = 5;
        float rowHeight   = 35;
        float columnWidth = 150;
        float desHeight = 300;
        float columnHeight = windowSize.y - rowHeight - 3 * margin;
        float centerWidth = windowSize.x - 2 * (2 * margin + columnWidth);
        float fullWidth = windowSize.x - 2 * margin;
        float l = margin;
        float t = margin;
        m_library->rect = FloatRect(l, t, columnWidth, columnHeight);
        t += columnHeight + margin;
        m_info->rect = FloatRect(l, t, fullWidth, rowHeight);
        t = margin;
        l += columnWidth + margin;
        m_device->rect = FloatRect(l, t, centerWidth, rowHeight);
        t += rowHeight + margin;
        m_designer->rect = FloatRect(l, t, centerWidth, desHeight);   
        t += desHeight + margin;
        m_visualizer->rect = FloatRect(l, t, centerWidth, windowSize.y - desHeight -rowHeight * 2 - margin * 5);  
        t = margin;
        l += centerWidth + margin;
        m_channel->rect = FloatRect(l, t, columnWidth, columnHeight);
    }
};

int main(int argc, char const *argv[])
{
    try {
        Engine::makeRoot<SyntactsGui>();
        Engine::init(900, 540, WindowStyle::Close);
        Engine::window->setTitle("Syntacts");
        Engine::setBackgroundColor(Grays::Gray5);
        Engine::run(); 
    }
    catch(...) {
        print("Exception Thrown");
        return -1;
    }
    print("Program Terminated Normally");
    return 0;
}
