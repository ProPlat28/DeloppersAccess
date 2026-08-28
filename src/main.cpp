#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class SetFeaturedPopup : public Popup {
protected:
GJGameLevel* m_level = nullptr;
int m_value = 0;
CCLabelBMFont* m_valueLabel = nullptr;

bool init(GJGameLevel* level) {
    if (!Popup::init(380.f, 180.f))
        return false;

    m_level = level;
    m_value = level ? level->m_featured : 0;

    if (m_closeBtn)
        m_closeBtn->setVisible(false);

    auto bg = CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize({ 380.f, 180.f });
    bg->setPosition({ 190.f, 90.f });
    m_mainLayer->addChild(bg, -1);

    auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
    auto closeButton = CCMenuItemSpriteExtra::create(
        closeSprite,
        this,
        menu_selector(SetFeaturedPopup::onClose)
    );

    auto closeMenu = CCMenu::create();
    closeMenu->addChild(closeButton);
    closeMenu->setPosition({ 378.f, 182.f });
    m_mainLayer->addChild(closeMenu);

    auto title = CCLabelBMFont::create("Set Featured", "bigFont.fnt");
    title->setScale(0.9f);
    title->setPosition({ 190.f, 162.f });
    m_mainLayer->addChild(title);

    m_valueLabel = CCLabelBMFont::create(
        std::to_string(m_value).c_str(),
        "bigFont.fnt"
    );

    m_valueLabel->setScale(0.9f);
    m_valueLabel->setPosition({ 190.f, 100.f });
    m_mainLayer->addChild(m_valueLabel);

    auto arrows = CCMenu::create();
    arrows->setPosition({ 0.f, 0.f });

    auto addArrow = [&](char const* frame, float rotation, SEL_MenuHandler callback, float x) {
        auto sprite = CCSprite::createWithSpriteFrameName(frame);

        if (!sprite)
            return;

        sprite->setRotation(rotation);
        sprite->setScale(0.95f);

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            callback
        );

        if (!button)
            return;

        button->setPosition({ x, 100.f });
        arrows->addChild(button);
    };

    addArrow(
        "GJ_arrow_01_001.png",
        -90.f,
        menu_selector(SetFeaturedPopup::onMinus10),
        32.f
    );

    addArrow(
        "GJ_arrow_02_001.png",
        -90.f,
        menu_selector(SetFeaturedPopup::onMinus1),
        84.f
    );

    addArrow(
        "GJ_arrow_02_001.png",
        90.f,
        menu_selector(SetFeaturedPopup::onPlus1),
        296.f
    );

    addArrow(
        "GJ_arrow_01_001.png",
        90.f,
        menu_selector(SetFeaturedPopup::onPlus10),
        348.f
    );

    m_mainLayer->addChild(arrows);

    auto buttons = CCMenu::create();
    buttons->setPosition({ 190.f, 25.f });

    auto cancelSprite = ButtonSprite::create(
        "Cancel",
        "goldFont.fnt",
        "GJ_button_01.png"
    );

    auto cancelButton = CCMenuItemSpriteExtra::create(
        cancelSprite,
        this,
        menu_selector(SetFeaturedPopup::onCancel)
    );

    cancelButton->setPosition({ -60.f, 0.f });
    buttons->addChild(cancelButton);

    auto submitSprite = ButtonSprite::create(
        "Submit",
        "goldFont.fnt",
        "GJ_button_01.png"
    );

    auto submitButton = CCMenuItemSpriteExtra::create(
        submitSprite,
        this,
        menu_selector(SetFeaturedPopup::onSubmit)
    );

    submitButton->setPosition({ 60.f, 0.f });
    buttons->addChild(submitButton);

    m_mainLayer->addChild(buttons);

    auto epicMenu = CCMenu::create();
    epicMenu->setPosition({ 350.f, 25.f });

    auto epicSprite = ButtonSprite::create(
        "Epic\nOnly",
        "goldFont.fnt",
        "GJ_button_01.png"
    );

    epicSprite->setScale(0.45f);

    auto epicButton = CCMenuItemSpriteExtra::create(
        epicSprite,
        this,
        menu_selector(SetFeaturedPopup::onEpicOnly)
    );

    epicMenu->addChild(epicButton);
    m_mainLayer->addChild(epicMenu);

    return true;
}

void updateValue() {
    if (m_valueLabel)
        m_valueLabel->setString(std::to_string(m_value).c_str());
}

void onMinus10(CCObject*) {
    m_value -= 10;
    updateValue();
}

void onMinus1(CCObject*) {
    --m_value;
    updateValue();
}

void onPlus1(CCObject*) {
    ++m_value;
    updateValue();
}

void onPlus10(CCObject*) {
    m_value += 10;
    updateValue();
}

void onCancel(CCObject* sender) {
    onClose(sender);
}

void onEpicOnly(CCObject* sender) {
    onClose(sender);
}

void onSubmit(CCObject* sender) {
    if (m_level)
        m_level->m_featured = m_value;

    onClose(sender);
}

public:
static SetFeaturedPopup* create(GJGameLevel* level) {
auto popup = new SetFeaturedPopup();

    if (!popup->init(level)) {
        delete popup;
        return nullptr;
    }

    popup->autorelease();
    return popup;
}

};

class $modify(SFLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        GJGameLevel* level = nullptr;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        m_fields->level = level;

        auto leftMenu = getChildByID("left-side-menu");

        if (!leftMenu)
            return true;

        if (leftMenu->getChildByID("set-featured-button"))
            return true;

        auto star = CCSprite::create("sf_star_icon.png"_spr);

        if (!star)
            return true;

        star->setScale(0.6f);

        auto button = CCMenuItemSpriteExtra::create(
            star,
            this,
            menu_selector(SFLevelInfoLayer::onSetFeatured)
        );

        if (!button)
            return true;

        button->setID("set-featured-button");

        button->setLayoutOptions(
            AxisLayoutOptions::create()
                ->setPrevGap(5.f)
        );

        leftMenu->addChild(button);
        leftMenu->updateLayout();

        return true;
    }

    void onSetFeatured(CCObject*) {
        if (!m_fields->level)
            return;

        auto popup = SetFeaturedPopup::create(m_fields->level);

        if (popup)
            popup->show();
    }
};
