#pragma once

#include "misc/dr4_ifc.hpp"

namespace gfx {

class DR4Backend : public dr4::DR4Backend {
  public:
    ~DR4Backend() = default;

    virtual const std::string&
    Name() const override;
    virtual dr4::Window*
    CreateWindow() override;
};

extern "C" dr4::DR4Backend*
CreateDR4Backend();

} // namespace gfx
