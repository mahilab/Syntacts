#pragma once

#include "Widget.hpp"
#include "Nodes.hpp"

class Designer : public Widget {
public:
    Designer(Gui& _gui);
    tact::Signal buildSignal();
    void update() override;
    void edit(const tact::Signal& sig);
private:
    std::shared_ptr<Node> m_root;
};