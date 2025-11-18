#pragma once

#include "cum/ifc/dr4.hpp"

namespace cum {
namespace impl {

class DR4Backend : public cum::DR4BackendPlugin {
  public:
    ~DR4Backend() = default;

    virtual dr4::Window*
    CreateWindow() override final;

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
