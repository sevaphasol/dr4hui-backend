#include "backend.hpp"
#include "window.hpp"

namespace gfx {

const std::string&
DR4Backend::Name() const
{
    static const std::string& name = "Хуй";

    return name;
}

dr4::Window*
DR4Backend::CreateWindow()
{
    return new Window();
}

extern "C" dr4::DR4Backend*
CreateDR4Backend()
{
    return new DR4Backend();
}

} // namespace gfx
