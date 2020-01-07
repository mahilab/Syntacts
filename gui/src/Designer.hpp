#pragma once

#include "Widget.hpp"
#include "Nodes.hpp"

class Designer : public Widget {
public:
    using Widget::Widget;
    tact::Signal buildSignal();
    void render() override;
private:
    ProductNode m_root;
};