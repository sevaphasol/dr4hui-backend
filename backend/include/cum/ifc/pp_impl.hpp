#pragma once

#include "cum/ifc/pp.hpp"

namespace dr4 {
namespace impl {

class PPToolPlugin : public pp::PPToolPlugin {
  public:
    ~PPToolPlugin() = default;

    virtual std::vector<pp::Tool*>
    CreateTools( pp::Canvas* cvs ) override final;

    virtual std::string_view
    GetIdentifier() const override final;

    virtual std::string_view
    GetName() const override final;

    virtual std::string_view
    GetDescription() const override final;

    virtual std::vector<std::string_view>
    GetDependencies() const override final;

    virtual std::vector<std::string_view>
    GetConflicts() const override final;

    virtual void
    AfterLoad() override final;
};

} // namespace impl
} // namespace dr4
