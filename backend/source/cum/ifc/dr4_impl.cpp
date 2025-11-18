#include "cum/ifc/dr4_impl.hpp"
#include "dr4/window_impl.hpp"

dr4::Window*
dr4::impl::DR4Backend::CreateWindow()
{
    return new dr4::impl::Window();
}

std::string_view
dr4::impl::DR4Backend::GetIdentifier() const
{
    return {};
}

std::string_view
dr4::impl::DR4Backend::GetName() const
{
    return "dr4";
}

std::string_view
dr4::impl::DR4Backend::GetDescription() const
{
    return "dr4";
}

std::vector<std::string_view>
dr4::impl::DR4Backend::GetDependencies() const
{
    return {};
}

std::vector<std::string_view>
dr4::impl::DR4Backend::GetConflicts() const
{
    return {};
}

void
dr4::impl::DR4Backend::AfterLoad()
{
}
