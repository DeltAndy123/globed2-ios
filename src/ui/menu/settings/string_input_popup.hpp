#pragma once
#include <defs/all.hpp>

class StringInputPopup : public geode::Popup<std::function<void(std::string_view)>> {
public:
    static constexpr float POPUP_WIDTH = 240.f;
    static constexpr float POPUP_HEIGHT = 80.f;

    static StringInputPopup* create(std::function<void(std::string_view)>);

private:
    geode::TextInput* inputNode;

    bool setup(std::function<void(std::string_view)>) override;
};
