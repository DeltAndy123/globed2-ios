#include "player_list_cell.hpp"

#include <UIBuilder.hpp>

#include "room_popup.hpp"

using namespace geode::prelude;

bool PlayerListCell::init(const PlayerRoomPreviewAccountData& data) {
    if (!CCLayer::init()) return false;
    this->data = data;

    Build<CCMenu>::create()
        .pos(RoomPopup::LIST_WIDTH, CELL_HEIGHT)
        .parent(this)
        .store(menu);

    auto* label = Build<CCLabelBMFont>::create(data.name.c_str(), "bigFont.fnt")
        .limitLabelWidth(170.f, 0.8f, 0.1f)
        .collect();

    label->setScale(label->getScale() * 0.9f);

    auto* btn = Build<CCMenuItemSpriteExtra>::create(label, this, menu_selector(PlayerListCell::onOpenProfile))
        .pos(-RoomPopup::LIST_WIDTH + 57.f, -22.f)
        .parent(menu)
        .collect();

    // this achieves the same thing as setting anchor point X to 0.0f, but with the correct bounce
    btn->setPositionX(btn->getPositionX() + btn->getScaledContentSize().width / 2);

    auto* gm = GameManager::get();

    Build<SimplePlayer>::create(data.cube)
        .playerFrame(data.cube, IconType::Cube)
        .color(gm->colorForIdx(data.color1))
        .secondColor(gm->colorForIdx(data.color2))
        .parent(this)
        .pos(25.f, CELL_HEIGHT - 22.f)
        .store(simplePlayer);

    if (data.glowColor == -1) {
        simplePlayer->disableGlowOutline(); // TODO fix this and remove later
    } else {
        simplePlayer->setGlowOutline(gm->colorForIdx(data.glowColor));
    }

    if (this->data.levelId != 0) {
        auto* cbs = CircleButtonSprite::createWithSpriteFrameName("d_skull01_001.png", 1.f, CircleBaseColor::Green, CircleBaseSize::Medium);
        Build<CircleButtonSprite>(cbs)
            .scale(0.75f)
            .intoMenuItem([this](auto) {
                // TODO OPen the level
                log::debug("here you open the level ID {}", this->data.levelId);
            })
            .pos(-30.f, -23.f)
            .parent(menu);
    }

    return true;
}

void PlayerListCell::onOpenProfile(cocos2d::CCObject*) {
    ProfilePage::create(data.id, false)->show();
}

PlayerListCell* PlayerListCell::create(const PlayerRoomPreviewAccountData& data) {
    auto ret = new PlayerListCell;
    if (ret->init(data)) {
        return ret;
    }

    delete ret;
    return nullptr;
}