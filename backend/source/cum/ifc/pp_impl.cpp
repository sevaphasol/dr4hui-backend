#include "cum/ifc/pp_impl.hpp"

std::vector<pp::Tool*>
dr4::impl::PPToolPlugin::CreateTools( pp::Canvas* cvs )
{
    // return { new pp::impl::Tool::Rect() };
    return {};
}

std::string_view
dr4::impl::PPToolPlugin::GetIdentifier() const
{
    return "pp";
}

std::string_view
dr4::impl::PPToolPlugin::GetName() const
{
    return "pp";
}

std::string_view
dr4::impl::PPToolPlugin::GetDescription() const
{
    return "pp";
}

std::vector<std::string_view>
dr4::impl::PPToolPlugin::GetDependencies() const
{
    return {};
}

std::vector<std::string_view>
dr4::impl::PPToolPlugin::GetConflicts() const
{
    return {};
}

void
dr4::impl::PPToolPlugin::AfterLoad()
{
}
