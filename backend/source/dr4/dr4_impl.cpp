#include "cum/ifc/dr4_impl.hpp"
#include "cum/manager.hpp"
#include "dr4/window_impl.hpp"

dr4::Window*
cum::impl::DR4Backend::CreateWindow()
{
    return new dr4::impl::Window();
}

std::string_view
cum::impl::DR4Backend::GetIdentifier() const
{
    return {};
}

std::string_view
cum::impl::DR4Backend::GetName() const
{
    return "dr4";
}

std::string_view
cum::impl::DR4Backend::GetDescription() const
{
    return "dr4";
}

std::vector<std::string_view>
cum::impl::DR4Backend::GetDependencies() const
{
    return {};
}

std::vector<std::string_view>
cum::impl::DR4Backend::GetConflicts() const
{
    return {};
}

void
cum::impl::DR4Backend::AfterLoad()
{
}

extern "C" cum::Plugin*
CREATE_PLUGIN_FUNC_NAME()
{
    return new cum::impl::DR4Backend();
}
