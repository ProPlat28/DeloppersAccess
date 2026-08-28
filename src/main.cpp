#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

// Devlopper's Set Featured Popup

class SetFeaturedPopup : public Popup {
protected:
GJGameLevel* m_level = nullptr;
int m_value = 0;
CCLabelBMFont* m_valueLabel = nullptr;

```
bool init(GJGameLevel* level) {
    if (!Popup::init(380.f, 180.f))
        return false;

    m_level = level;

    if (level)
        m_value = level->m_featured;

    if (m_closeBtn)
        m_closeBtn->setVisible(false);

    auto bg = CCScale9Sprite::create("GJ_square01.png");

    if (bg) {
        bg->setContentSize({ 380.f, 180.f });
        bg->setPosition({ 190.f, 90.f });
        m_mainLayer->addChild(bg, -1);
    }

    auto closeSprite = CCSprite::createWithSpriteFrameName(
        "GJ_deleteBtn_001.png"
    );

    if (closeSprite) {
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(SetFeaturedPopup::onClose)
        );

        if (closeButton) {
            auto closeMenu = CCMenu::create();
            closeMenu->addChild(closeButton);
            closeMenu->setPosition({ 378.f, 182.f });
            m_mainLayer->addChild(closeMenu);
        }
    }

    auto title = CCLabelBMFont::create(
        "Set Featured",
        "bigFont.fnt"
    );

    if (title) {
        title->setScale(0.9f);
        title->setPosition({ 190.f, 162.f });
        m_mainLayer->addChild(title);
    }

    m_valueLabel = CCLabelBMFont::create(
        std::to_string(m_value).c_str(),
        "bigFont.fnt"
    );

    if (m_valueLabel) {
        m_valueLabel->setScale(0.9f);
        m_valueLabel->setPosition({ 190.f, 100.f });
        m_valueLabel->setID("featured-value");
        m_mainLayer->addChild(m_valueLabel);
    }

    auto stepperMenu = CCMenu::create();

    if (stepperMenu) {
        stepperMenu->setPosition({ 0.f, 0.f });

        auto minus10Sprite = CCSprite::createWithSpriteFrameName(
            "GJ_arrow_01_001.png"
        );

        if (minus10Sprite) {
            minus10Sprite->setRotation(-90.f);
            minus10Sprite->setScale(0.95f);

            auto minus10Button = CCMenuItemSpriteExtra::create(
                minus10Sprite,
                this,
                menu_selector(SetFeaturedPopup::onMinus10)
            );

            if (minus10Button) {
                minus10Button->setPosition({ 32.f, 100.f });
                stepperMenu->addChild(minus10Button);
            }
        }

        auto minus1Sprite = CCSprite::createWithSpriteFrameName(
            "GJ_arrow_02_001.png"
        );

        if (minus1Sprite) {
            minus1Sprite->setRotation(-90.f);
            minus1Sprite->setScale(0.95f);

            auto minus1Button = CCMenuItemSpriteExtra::create(
                minus1Sprite,
                this,
                menu_selector(SetFeaturedPopup::onMinus1)
            );

            if (minus1Button) {
                minus1Button->setPosition({ 84.f, 100.f });
                stepperMenu->addChild(minus1Button);
            }
        }

        auto plus1Sprite = CCSprite::createWithSpriteFrameName(
            "GJ_arrow_02_001.png"
        );

        if (plus1Sprite) {
            plus1Sprite->setRotation(90.f);
            plus1Sprite->setScale(0.95f);

            auto plus1Button = CCMenuItemSpriteExtra::create(
                plus1Sprite,
                this,
                menu_selector(SetFeaturedPopup::onPlus1)
            );

            if (plus1Button) {
                plus1Button->setPosition({ 296.f, 100.f });
                stepperMenu->addChild(plus1Button);
            }
        }

        auto plus10Sprite = CCSprite::createWithSpriteFrameName(
            "GJ_arrow_01_001.png"
        );

        if (plus10Sprite) {
            plus10Sprite->setRotation(90.f);
            plus10Sprite->setScale(0.95f);

            auto plus10Button = CCMenuItemSpriteExtra::create(
                plus10Sprite,
                this,
                menu_selector(SetFeaturedPopup::onPlus10)
            );

            if (plus10Button) {
                plus10Button->setPosition({ 348.f, 100.f });
                stepperMenu->addChild(plus10Button);
            }
        }

        m_mainLayer->addChild(stepperMenu);
    }

    auto bottomMenu = CCMenu::create();

    if (bottomMenu) {
        bottomMenu->setPosition({ 190.f, 25.f });

        auto cancelSprite = ButtonSprite::create(
            "Cancel",
            "goldFont.fnt",
            "GJ_button_01.png"
        );

        if (cancelSprite) {
            auto cancelButton = CCMenuItemSpriteExtra::create(
                cancelSprite,
                this,
                menu_selector(SetFeaturedPopup::onCancel)
            );

            if (cancelButton) {
                cancelButton->setPosition({ -60.f, 0.f });
                bottomMenu->addChild(cancelButton);
            }
        }

        auto submitSprite = ButtonSprite::create(
            "Submit",
            "goldFont.fnt",
            "GJ_button_01.png"
        );

        if (submitSprite) {
            auto submitButton = CCMenuItemSpriteExtra::create(
                submitSprite,
                this,
                menu_selector(SetFeaturedPopup::onSubmit)
            );

            if (submitButton) {
                submitButton->setPosition({ 60.f, 0.f });
                bottomMenu->addChild(submitButton);
            }
        }

        m_mainLayer->addChild(bottomMenu);
    }

    auto epicMenu = CCMenu::create();

    if (epicMenu) {
        epicMenu->setPosition({ 350.f, 25.f });

        auto epicSprite = ButtonSprite::create(
            "Epic\nOnly",
            "goldFont.fnt",
            "GJ_button_01.png"
        );

        if (epicSprite) {
            epicSprite->setScale(0.45f);

            auto epicButton = CCMenuItemSpriteExtra::create(
                epicSprite,
                this,
                menu_selector(SetFeaturedPopup::onEpicOnly)
            );

            if (epicButton)
                epicMenu->addChild(epicButton);
        }

        m_mainLayer->addChild(epicMenu);
    }

    return true;
}

void updateValue() {
    if (m_valueLabel)
        m_valueLabel->setString(std::to_string(m_value).c_str());
}

void onMinus10(CCObject*) {
    m_value - = 10;
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
    m_value + = 10;
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
```

public:
static SetFeaturedPopup* create(GJGameLevel* level) {
auto popup = new SetFeaturedPopup();

```
    if (popup->init(level)) {
        popup->autorelease();
        return popup;
    }

    delete popup;
    return nullptr;
}
```

};

class $modify(SFLevelInfoLayer, LevelInfoLayer) {
struct Fields {
GJGameLevel* level = nullptr;
};

```
bool init(GJGameLevel* level, bool challenge) {
    if (!LevelInfoLayer::init(level, challenge))
        return false;

    m_fields->level = level;

    auto leftMenu = getChildByID("left-side-menu");

    if (!leftMenu)
        return true;

    auto starSprite = CCSprite::create(
        "sf_star_icon.png"_spr
    );

    if (!starSprite)
        return true;

    starSprite->setScale(0.6f);

    auto starButton = CCMenuItemSpriteExtra::create(
        starSprite,
        this,
        menu_selector(SFLevelInfoLayer::onSetFeatured)
    );

    if (!starButton)
        return true;

    starButton->setID("set-featured-button");

    leftMenu->addChild(starButton);
    leftMenu->updateLayout();

    return true;
}

void onSetFeatured(CCObject*) {
    if (!m_fields->level)
        return;

    auto popup = SetFeaturedPopup::create(
        m_fields->level
    );

    if (popup)
        popup->show();
}

};

// Devlopper's Rate Stars Layer