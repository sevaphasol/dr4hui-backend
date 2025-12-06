#pragma once

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"
#include "pp/canvas.hpp"
#include "pp/shape.hpp"
#include <vector>

namespace pp {
namespace impl {

class BrushShape final : public ::pp::Shape {
  public:
    BrushShape( dr4::Window* window, const pp::ControlsTheme& theme, pp::Canvas* cvs );

    void
    DrawOn( dr4::Texture& texture ) const override final;

    void
    SetPos( dr4::Vec2f pos ) override final;
    dr4::Vec2f
    GetPos() const override final;

    // Интерфейс для инструмента
    void
    AddPoint( dr4::Vec2f point );
    const std::vector<dr4::Vec2f>&
    GetPoints() const
    {
        return points_;
    }

    static constexpr float BrushRadius = 3.0f;

  private:
    pp::Canvas*             cvs_ = nullptr;
    std::vector<dr4::Vec2f> points_;
    dr4::Color              brush_color_;
    dr4::Window*            window_ = nullptr;
};

} // namespace impl
} // namespace pp
