#pragma once
#include <memory>
#include <imgui.h>

namespace ImGui {

struct PolyBezier {
    PolyBezier(ImVec4 color = ImVec4(0,1,1,1), ImVec2 min = ImVec2(0,0), ImVec2 max = ImVec2(1,1));
    int pointCount();  
    int bezierCount();
    void clearPoints();
    void addPoint(ImVec2 cpL, ImVec2 pos, ImVec2 cpR);
    void getPoint(int index, ImVec2* cpL, ImVec2* pos, ImVec2* cpR);
    void getBezier(int index, ImVec2* pos0, ImVec2* cp0, ImVec2* cp1, ImVec2* pos1);

    ImVec2 min;
    ImVec2 max;
    ImVec4 color;
    float thickness;
    int segments;
    bool constrainEndpoints;
    float grabRadius;

private:
    friend void PolyBezierEdit(const char*,PolyBezier*,int,int,ImVec2);
    struct Point;
    struct Group;
    std::shared_ptr<Group> root;
    std::shared_ptr<Group> head;
};

void PolyBezierEdit(const char* label, PolyBezier* polyBezier, int gridX = 10, int gridY = 10, ImVec2 size = ImVec2(-1,0));

} // namespace ImGui