#define IMGUI_DEFINE_MATH_OPERATORS

#include "PolyBezier.hpp"
#include "Custom.hpp"
#include <imgui_internal.h>

///////////////////////////////////////////////////////////////////////////////

namespace ImGui {

struct PolyBezier::Point {
    ImVec2 pos;    
    bool grabbed = false;
    ImVec2 constrain(float xMin, float xMax, float yMin, float yMax) {       
        ImVec2 constrained = pos;
        pos.x = ImClamp(pos.x, ImMax(xMin,constraints.Min.x), ImMin(xMax,constraints.Max.x));
        pos.y = ImClamp(pos.y, ImMax(yMin,constraints.Min.y), ImMin(yMax,constraints.Max.y));
        return pos - constrained;
    }
    ImRect constraints = ImRect(-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX);
};

struct PolyBezier::Group : std::enable_shared_from_this<PolyBezier::Group> {

    Group(ImVec2 pos, float cpLength) {
        p.pos = pos;
        cpl.pos = pos - ImVec2(cpLength, 0);
        cpr.pos = pos + ImVec2(cpLength, 0);
    }

    void constrain(const ImVec2& min, const ImVec2& max) {
        if (forceTanget) {
            if (cpr.grabbed) {
                auto v = cpr.pos - p.pos;
                cpl.pos = p.pos - v;
            }
            else {
                auto v = cpl.pos - p.pos;
                cpr.pos = p.pos - v;
            }
        } 
        constrainP(min,max);
        cpl.constrain(prev ? prev->p.pos.x : -FLT_MAX, p.pos.x, -FLT_MAX, FLT_MAX);
        cpr.constrain(p.pos.x, next ? next->p.pos.x : FLT_MAX, -FLT_MAX, FLT_MAX);   
    }

    ImVec2 constrainP(const ImVec2& min, const ImVec2& max) {
        return p.constrain(prev ? prev->p.pos.x : min.x, next ? (next->p.grabbed ? p.pos.x : next->p.pos.x) : max.x, min.y, max.y);
    }

    void insert(std::shared_ptr<PolyBezier::Group> g) {
        if (prev) {
            prev->next = g;
            g->prev = prev;
        }
        g->next = shared_from_this();
        prev = g;
    }

    void push(std::shared_ptr<PolyBezier::Group> g) {
        next = g;
        g->prev = shared_from_this();
    }

    void remove() {
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;
    }

    PolyBezier::Point p, cpl, cpr;
    bool active = false;
    bool forceTanget = true;

    std::shared_ptr<PolyBezier::Group> prev = nullptr;
    std::shared_ptr<PolyBezier::Group> next = nullptr;
};

PolyBezier::PolyBezier(ImVec4 color, ImVec2 min, ImVec2 max) : 
    color(color), min(min), max(max), thickness(1), segments(64), constrainEndpoints(true), grabRadius(3)
{
    root = std::make_shared<PolyBezier::Group>(min, 0.1f * (max.x - min.x));
    head = std::make_shared<PolyBezier::Group>(max, 0.1f * (max.y - min.y));
    root->push(head);
}

int PolyBezier::pointCount() {
    int points = 0;
    for (auto g = root; g; g = g->next) 
        points++;
    return points;     
}    

int PolyBezier::bezierCount() {
    int beziers = -1;
    for (auto g = root; g; g = g->next) 
        beziers++;
    return ImClamp(beziers,0,INT_MAX);
}

void PolyBezier::clearPoints() {
    root = nullptr;
    head = nullptr;
}

void PolyBezier::addPoint(ImVec2 cpL, ImVec2 pos, ImVec2 cpR) {
    auto newG = std::make_shared<PolyBezier::Group>(pos, 0);
    newG->p.pos   = pos;
    newG->cpl.pos = cpL;
    newG->cpr.pos = cpR;
    if (!root) {
        root = newG;
        head = root;
    }
    else {
        bool inserted = false;
        for (auto g = root; g; g = g->next)
        {
            if (g->p.pos.x > newG->p.pos.x) {
                g->insert(newG);
                if (g == root)
                    root = newG;
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            head->push(newG);
            head = newG;
        }
    }
}

void PolyBezier::getPoint(int index, ImVec2* cpL, ImVec2* pos, ImVec2* cpR) {
    IM_ASSERT(index < pointCount());
    auto g = root;
    for (int i = 0; i < index; ++i)
        g = g->next;
    *cpL = g->cpl.pos; *pos = g->p.pos; *cpR = g->cpr.pos;
}

void PolyBezier::getBezier(int index, ImVec2* pos0, ImVec2* cp0, ImVec2* cp1, ImVec2* pos1) {
    IM_ASSERT(index < bezierCount());
    auto g = root;
    for (int i = 0; i < index; ++i) 
        g = g->next;
    *pos0 = g->p.pos; *cp0 = g->cpr.pos; *cp1 = g->next->cpl.pos; *pos1 = g->next->p.pos;
}

void PolyBezierEdit(const char* label, PolyBezier* polyBezier, int gridX, int gridY, ImVec2 size) {
    PolyBezier& pb = *polyBezier;
    // skip render if SkipItems on
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    // get ImGuiContext and Style
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImGuiIO& IO = GetIO();
    ImDrawList* DrawList = GetWindowDrawList();
    // convert color
    auto color32 = ColorConvertFloat4ToU32(pb.color);
    // sizing
    size = CalcItemSize(size, 200, 200);
    const ImRect bb_frame(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImVec2 padding(10,10);
    const ImRect bb_px(bb_frame.Min + padding, bb_frame.Max - padding);

    const float bb_px_width     = bb_px.GetWidth();
    const float bb_px_height    = bb_px.GetHeight();
    const float bb_px_invWidth  = 1.0f / bb_px_width;
    const float bb_px_invHeight = 1.0f / bb_px_height;

    const float bb_pb_width     = pb.max.x - pb.min.x;
    const float bb_pb_height    = pb.max.y - pb.min.y;
    const float bb_pb_invWidth  = 1.0f / bb_pb_width;
    const float bb_pb_invHeight = 1.0f / bb_pb_height;    

    // render background
    ItemSize(bb_frame, style.FramePadding.y);
    if (!ItemAdd(bb_frame, 0, &bb_frame))
        return;
    const bool hovered = ItemHoverable(bb_px, id);
    RenderFrame(bb_frame.Min, bb_frame.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    RenderGrid(bb_px, gridX, gridY, GetColorU32(ImGuiCol_WindowBg), GetColorU32(ImGuiCol_FrameBgHovered, 0.1f));
    // clear active lambda
    auto clearActivations = [&](std::shared_ptr<PolyBezier::Group> except = nullptr) {
        for (auto g = pb.root; g; g = g->next){
            if (g != except)
                g->active = false;
        }
    };
    // transformation lambdas
    auto toPx = [&](const ImVec2& pbV) -> ImVec2 {
        float pxX = bb_px.Min.x + ( pbV.x - pb.min.x ) * bb_px_width  * bb_pb_invWidth;
        float pxY = bb_px.Max.y - ( pbV.y - pb.min.y ) * bb_px_height * bb_pb_invHeight;
        return ImVec2(pxX,pxY);
    };
    auto toPb = [&](const ImVec2& pxV) -> ImVec2 {
        float pbX =  (pxV.x - bb_px.Min.x) * bb_px_invWidth  * bb_pb_width  + pb.min.x;
        float pbY = -(pxV.y - bb_px.Max.y) * bb_px_invHeight * bb_pb_height + pb.min.y;
        return ImVec2(pbX,pbY);
    };
    auto toPbD = [&](const ImVec2& pxV) -> ImVec2 {
        float pbDX =  (pxV.x) * bb_px_invWidth  * bb_pb_width;
        float pbDY = -(pxV.y) * bb_px_invHeight * bb_pb_height;
        return ImVec2(pbDX,pbDY);
    };    
    // check for additions
    if (IO.MouseClicked[1] && hovered) {
        auto newG = std::make_shared<PolyBezier::Group>(toPb(IO.MousePos), 0.1f * bb_pb_width);
        if (!pb.root) {
            pb.root = newG;
            pb.head = pb.root;
        }
        else
        {
            bool inserted = false;
            for (auto g = pb.root; g; g = g->next)
            {
                if (g->p.pos.x > newG->p.pos.x) {
                    g->insert(newG);
                    if (g == pb.root)
                        pb.root = newG;
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                pb.head->push(newG);
                pb.head = newG;
            }
        }  
        clearActivations();
        newG->active = true;
        newG->p.grabbed = true;
    }
    // check for deletions
    if (ImGui::IsKeyPressedMap(ImGuiKey_Delete)) {
        for (auto g = pb.root; g; g = g->next) {
            if (g->active && g != pb.root && g != pb.head) {
                g->remove();
            }
        }
    }
    // drag 
    bool clearActive = true;
    for (auto g = pb.root; g; g = g->next) {
        ImVec2 v;
        float d;
        if (g->active) {
            // drag left cp
            v = (IO.MousePos - toPx(g->cpl.pos));
            d = v.x * v.x + v.y * v.y;  
            if (d < 25) {
                if (IO.MouseClicked[0]) {
                    if (IO.KeyCtrl)
                        g->forceTanget = !g->forceTanget;
                    g->cpl.grabbed = true;
                    clearActive = false;
                }   
            }
            if (g->cpl.grabbed) {
                g->cpl.pos += toPbD(IO.MouseDelta);
                if (IO.MouseReleased[0]) 
                    g->cpl.grabbed = false;
                break;
            }
            // drag right cp
            v = (IO.MousePos - toPx(g->cpr.pos));
            d = v.x * v.x + v.y * v.y;  
            if (d < 25) {
                if (IO.MouseClicked[0]) {
                    if (IO.KeyCtrl)
                        g->forceTanget = !g->forceTanget;
                    g->cpr.grabbed = true;
                    clearActive = false;
                }   
            }
            if (g->cpr.grabbed) {
                g->cpr.pos += toPbD(IO.MouseDelta);
                if (IO.MouseReleased[0]) 
                    g->cpr.grabbed = false;
                break;
            }
        } 
        // drag point
        v = (IO.MousePos - toPx(g->p.pos));
        d = v.x * v.x + v.y * v.y;
        if (d < 25) {
            if (IO.MouseClicked[0]) {
                if (IO.KeyCtrl)
                    g->forceTanget = !g->forceTanget;
                g->p.grabbed = true;
                g->active = true;
                clearActive = false; 
                clearActivations(g);                
            }
        }
        if (g->p.grabbed) {
            g->p.pos += toPbD(IO.MouseDelta);
            auto constrainedBy = g->constrainP(pb.min, pb.max);
            g->cpr.pos += toPbD(IO.MouseDelta) + constrainedBy;
            g->cpl.pos += toPbD(IO.MouseDelta) + constrainedBy;
            if (IO.MouseReleased[0] || IO.MouseReleased[1]) 
                g->p.grabbed = false;
            break;
        }
    }
    // constrain
    if (pb.constrainEndpoints) {
        if (pb.root) {
            pb.root->p.constraints.Min.x = pb.min.x;
            pb.root->p.constraints.Max.x = pb.min.x;
        }
        if (pb.head && pb.head != pb.root) {
            pb.head->p.constraints.Min.x = pb.max.x;
            pb.head->p.constraints.Max.x = pb.max.x;
        }
    }
    else {
        if (pb.root) {
            pb.root->p.constraints.Min.x = -FLT_MAX;
            pb.root->p.constraints.Max.x = -FLT_MAX;
        }
        if (pb.head && pb.head != pb.root) {
            pb.head->p.constraints.Min.x = FLT_MAX;
            pb.head->p.constraints.Max.x = FLT_MAX;
        }
    }
    for (auto g = pb.root; g; g = g->next) 
        g->constrain(pb.min, pb.max);
    // clear active if clicked
    if (IO.MouseClicked[0] && clearActive)
        clearActivations();
    // check if anything active
    bool active = false;
    for (auto g = pb.root; g; g = g->next)
    {
        if (g->active) {
            active = true;
            break;
        }
    } 
    // bezierCount points
    int beziers = pb.bezierCount();
    // render points
    if (hovered || active) {
        for (auto g = pb.root; g; g = g->next) 
            DrawList->AddCircleFilled(toPx(g->p.pos), pb.grabRadius, color32);
    }
    // render bezier
    if (beziers > 0) {
        DrawList->PushClipRect(bb_px.Min, bb_px.Max, true);
        auto g = pb.root;
        for (int i = 0; i < beziers; ++i) {
            DrawList->AddBezierCurve(toPx(g->p.pos), toPx(g->cpr.pos), toPx(g->next->cpl.pos), toPx(g->next->p.pos), color32, pb.thickness, pb.segments);
            g = g->next;
        }
        DrawList->PopClipRect();
    }
    // render active grabs
    for (auto g = pb.root; g; g = g->next) {
        if (g->active) {
            int segs = 12;
            if (g->prev) {
                    ImGui::GetForegroundDrawList()->AddLine(toPx(g->cpl.pos), toPx(g->p.pos), GetColorU32(ImGuiCol_ButtonActive));

                if (g->forceTanget)
                    ImGui::GetForegroundDrawList()->AddRectFilled(toPx(g->cpl.pos) - ImVec2(pb.grabRadius,pb.grabRadius), toPx(g->cpl.pos) + ImVec2(pb.grabRadius,pb.grabRadius), GetColorU32(ImGuiCol_ButtonActive));
                else 
                    ImGui::GetForegroundDrawList()->AddRect(toPx(g->cpl.pos) - ImVec2(pb.grabRadius,pb.grabRadius), toPx(g->cpl.pos) + ImVec2(pb.grabRadius,pb.grabRadius), GetColorU32(ImGuiCol_ButtonActive));
                
            }
            if (g->next) {
                ImGui::GetForegroundDrawList()->AddLine(toPx(g->cpr.pos), toPx(g->p.pos), GetColorU32(ImGuiCol_ButtonActive));
                if (g->forceTanget)
                    ImGui::GetForegroundDrawList()->AddRectFilled(toPx(g->cpr.pos) - ImVec2(pb.grabRadius,pb.grabRadius), toPx(g->cpr.pos) + ImVec2(pb.grabRadius,pb.grabRadius), GetColorU32(ImGuiCol_ButtonActive));                
                else 
                    ImGui::GetForegroundDrawList()->AddRect(toPx(g->cpr.pos) - ImVec2(pb.grabRadius,pb.grabRadius), toPx(g->cpr.pos) + ImVec2(pb.grabRadius,pb.grabRadius), GetColorU32(ImGuiCol_ButtonActive));                
            }
        }
    }
    // render position label
    if (hovered || active) {
        char posText[16];
        ImVec2 nmMouse = toPb(IO.MousePos);
        sprintf(posText,"%.2f,%.2f",nmMouse.x, nmMouse.y);
        ImVec2 posTextSize = CalcTextSize(posText);
        RenderText(bb_px.Max - posTextSize - style.FramePadding, posText);        
    }
}

};