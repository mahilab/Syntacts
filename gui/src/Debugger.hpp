#pragma once

#include "Widget.hpp"

class Debugger : public Widget {
public:
    using Widget::Widget;
    void update() override;
public:
    bool show = false;
};