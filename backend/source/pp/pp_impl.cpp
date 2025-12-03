#include "cum/ifc/pp_impl.hpp"

#include "pp/tools/circle.hpp"
#include "pp/tools/line.hpp"
#include "pp/tools/rect.hpp"
#include "pp/tools/text.hpp"
#include <memory>

std::vector<std::unique_ptr<pp::Tool>>
cum::impl::PPToolPlugin::CreateTools( pp::Canvas* cvs )
{
    std::vector<std::unique_ptr<pp::Tool>> tools;

    tools.emplace_back( std::make_unique<pp::impl::RectTool>( cvs ) );
    tools.emplace_back( std::make_unique<pp::impl::CircleTool>( cvs ) );
    tools.emplace_back( std::make_unique<pp::impl::LineTool>( cvs ) );
    tools.emplace_back( std::make_unique<pp::impl::TextTool>( cvs ) );

    return tools;
}

std::string_view
cum::impl::PPToolPlugin::GetIdentifier() const
{
    return "pp";
}

std::string_view
cum::impl::PPToolPlugin::GetName() const
{
    return "pp";
}

std::string_view
cum::impl::PPToolPlugin::GetDescription() const
{
    return "pp";
}

std::vector<std::string_view>
cum::impl::PPToolPlugin::GetDependencies() const
{
    return {};
}

std::vector<std::string_view>
cum::impl::PPToolPlugin::GetConflicts() const
{
    return {};
}

void
cum::impl::PPToolPlugin::AfterLoad()
{
}

extern "C" cum::Plugin*
CREATE_PLUGIN_FUNC_NAME()
{
    return new cum::impl::PPToolPlugin();
}
