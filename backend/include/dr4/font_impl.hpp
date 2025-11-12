#pragma once

#include "dr4/texture.hpp"
#include <SFML/Graphics/Font.hpp>

namespace dr4 {
namespace impl {

class Font : public dr4::Font {
  public:
    virtual void
    LoadFromFile( const std::string& path ) override final;
    virtual void
    LoadFromBuffer( const void* buffer, size_t size ) override final;

    virtual float
    GetAscent( float fontSize ) const override final;
    virtual float
    GetDescent( float fontSize ) const override final;

    auto&
    GetImpl()
    {
        return impl_;
    }

    const auto&
    GetImpl() const
    {
        return impl_;
    }

  private:
    sf::Font impl_;
};

} // namespace impl
} // namespace dr4
