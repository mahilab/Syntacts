#include "Debugger.hpp"
#include "Gui.hpp"
#include "portaudio.h"

using namespace mahi::gui;

void Debugger::update()
{
    static double nxtTime = -1;
    static double cpuTotal = 0;
    static double cpuSession = 0;
    static std::size_t ram = 0;
    static auto glVer = glGetString(GL_VERSION);
    static auto vendor = glGetString(GL_VENDOR); 
    static auto renderer = glGetString(GL_RENDERER); 
    static float fps = 0;


    if (ImGui::GetTime() > nxtTime) {
        fps = ImGui::GetIO().Framerate;
        cpuTotal = mahi::util::cpu_usage_process();
        cpuSession = gui.device.session ? gui.device.session->getCpuLoad() : 0;
        ram = mahi::util::ram_used_process();
        nxtTime += 1.0;
    }

    if (show)
    {
        ImGui::Begin("Debug Info", &show, ImGuiWindowFlags_AlwaysAutoResize);
        // auto info = Debug::getInfo();
        ImGui::Text("Signal Count:        ");
        ImGui::SameLine();
        ImGui::Text("%d", tact::Signal::count());
        ImGui::Text("Max Voices:          ");
        ImGui::SameLine();
        ImGui::Text("%d", SYNTACTS_MAX_VOICES);
#ifdef SYNTACTS_USE_POOL
        ImGui::Text("Pool Capacity:       ");
        ImGui::SameLine();
        ImGui::Text("%d", tact::Signal::pool().blocksTotal());
        ImGui::Text("Pool Block Size      ");
        ImGui::SameLine();
        ImGui::Text("%d (%d total)", SYNTACTS_POOL_BLOCK_SIZE, SYNTACTS_POOL_BLOCK_SIZE * SYNTACTS_POOL_MAX_BLOCKS);
#endif
        ImGui::Text("ASIO Support:        ");
        ImGui::SameLine();
#ifdef PA_USE_ASIO
        ImGui::Text("True");
#else
        ImGui::Text("False");
#endif
        ImGui::Text("Pointer Type:        ");
        ImGui::SameLine();
#ifdef SYNTACTS_USE_SHARED_PTR
        ImGui::Text("std::shared_ptr");
#else
        ImGui::Text("std::unique_ptr");
#endif
        ImGui::Text("Library Directory:   ");
        ImGui::SameLine();        
        if (ImGui::TintedButton(tact::Library::getLibraryDirectory().c_str(), {0,0,0,0}))
            mahi::gui::open_folder(tact::Library::getLibraryDirectory());
        ImGui::Text("GUI Directory:       ");
        ImGui::SameLine();        
        if (ImGui::TintedButton(gui.saveDir().c_str(), {0,0,0,0}))
            mahi::gui::open_folder(gui.saveDir());

        ImGui::Separator();

        ImGui::Text("Framerate:           ");
        ImGui::SameLine();
        ImGui::Text("%.2f FPS", fps);
        ImGui::Text("Verts / Tris:        ");
        ImGui::SameLine();
        ImGui::Text("%d / %d", ImGui::GetIO().MetricsRenderVertices, ImGui::GetIO().MetricsRenderIndices / 3);
        ImGui::Text("RAM Usage:           "); 
        ImGui::SameLine(); ImGui::Text("%d MB", ram / 1000000);
        ImGui::Text("CPU Load:            "); ImGui::SameLine(); ImGui::Text("%.2f %%", cpuTotal);
        ImGui::Text("Session Load:        "); ImGui::SameLine(); ImGui::Text("%.2f %%", cpuSession * 100);        
        ImGui::Separator();
        ImGui::Text("Operating System:    ");
        ImGui::SameLine();
        ImGui::Text("%s %s", mahi::util::os_name().c_str(), mahi::util::os_version().c_str());
        ImGui::Text("Syntacts Version:    ");
        ImGui::SameLine();
        ImGui::Text(tact::syntactsVersion().c_str());
        ImGui::Text("PortAudio Version:   ");
        ImGui::SameLine();
        ImGui::Text(Pa_GetVersionInfo()->versionText);
        ImGui::Text("ImGui Version:       ");
        ImGui::SameLine();
        ImGui::Text(ImGui::GetVersion());
        ImGui::Text("GLFW Version:        ");
        ImGui::SameLine();
        ImGui::Text(glfwGetVersionString());
        ImGui::Text("OpenGL Version:      ");
        ImGui::SameLine();
        ImGui::Text("%s\n",glVer);
        ImGui::Text("GPU:                 ");
        ImGui::SameLine();
        ImGui::Text("%s\n",renderer);
        ImGui::PushStyleColor(ImGuiCol_Button, Reds::FireBrick);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Reds::Salmon);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Reds::LightSalmon);
        if (ImGui::Button("Report Issue", {-1, 0}))
            mahi::gui::open_url("https://github.com/mahilab/Syntacts/issues");
        ImGui::PopStyleColor(3);
        ImGui::End();
    }
}