#include "dr4/backend_impl.hpp"
#include "dr4/window_impl.hpp"

const std::string&
dr4::impl::DR4Backend::Name() const
{
    static const std::string& name = "BackendSFML";

    return name;
}

dr4::Window*
dr4::impl::DR4Backend::CreateWindow()
{
    return new dr4::impl::Window();
}

extern "C" dr4::DR4Backend*
CreateDR4Backend()
{
    return new dr4::impl::DR4Backend();
}
