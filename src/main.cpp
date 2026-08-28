#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/SupportLayer.hpp>
#include <Geode/modify/RateStarsLayer.hpp>
#include <Geode/modify/RateDemonLayer.hpp>

using namespace geode::prelude;

// Devlopper's Set Featured button

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

        auto closeSprite =
            CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");

        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(SetFeaturedPopup::onClose)
        );

        auto closeMenu = CCMenu::create();
        closeMenu->addChild(closeButton);
        closeMenu->setPosition({ 378.f, 182.f });
        m_mainLayer->addChild(closeMenu);

        auto title =
            CCLabelBMFont::create("Set Featured", "bigFont.fnt");

        title->setScale(0.9f);
        title->setPosition({ 190.f, 162.f });
        m_mainLayer->addChild(title);

        m_valueLabel = CCLabelBMFont::create(
            std::to_string(m_value).c_str(),
            "bigFont.fnt"
        );

        m_valueLabel->setScale(0.85f);
        m_valueLabel->setPosition({ 190.f, 100.f });
        m_mainLayer->addChild(m_valueLabel);

        auto arrows = CCMenu::create();
        arrows->setPosition({ 0.f, 0.f });

        auto addArrow = [&](
            char const* frame,
            float rotation,
            SEL_MenuHandler callback,
            float x
        ) {
            auto sprite =
                CCSprite::createWithSpriteFrameName(frame);

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
            m_valueLabel->setString(
                std::to_string(m_value).c_str()
            );
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

        auto starSprite =
            CCSprite::create("sf_star_icon.png"_spr);

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
        auto popup = SetFeaturedPopup::create(m_fields->level);

        if (popup)
            popup->show();
    }
};

template <typename Base, typename T>
inline bool instanceof(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

// Support Layer

class modCheck : public CCObject {
public:
    void DelayMod() {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj : CCArrayExt<CCObject*>(scene->getChildren())) {
            if (auto Check = typeinfo_cast<UploadActionPopup*>(pObj)) {
                if (Mod::get()->getSettingValue<int64_t>("modType") == 2) {
                    Check->showSuccessMessage(
                        "Success! Developer access granted."
                    );
                }
            }
        }
    }

    void DelayRate() {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj : CCArrayExt<CCObject*>(scene->getChildren())) {
            if (auto Check = typeinfo_cast<UploadActionPopup*>(pObj)) {
                Check->showSuccessMessage("Rating submitted!");
            }
        }
    }
};

// Moderator's Suggest Stars Layer

class $modify(MyRateStarsLayerMod, RateStarsLayer) {
    void onRate(CCObject* sender) {
        auto layer = static_cast<CCLayer*>(
            this->getChildren()->objectAtIndex(0)
        );

        if (layer && layer->getChildrenCount() == 3) {
            auto popup = UploadActionPopup::create(
                nullptr,
                "Sending rating..."
            );

            popup->show();

            popup->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.5f),
                    CCCallFunc::create(
                        new modCheck(),
                        callfunc_selector(modCheck::DelayRate)
                    ),
                    nullptr
                )
            );
        }
        else {
            RateStarsLayer::onRate(sender);
        }
    }
};

// Support Layer

class $modify(MySupportLayer, SupportLayer) {
    void DelayMod() {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj : CCArrayExt<CCObject*>(scene->getChildren())) {
            if (auto Check = typeinfo_cast<UploadActionPopup*>(pObj)) {
                if (Mod::get()->getSettingValue<int64_t>("modType") == 2) {
                    Check->showSuccessMessage(
                        "Success! Developer access granted."
                    );
                }
            }
        }
    }

    void onRequestAccess(CCObject* sender) {
        auto GM = GameManager::sharedState();

        if (Mod::get()->getSettingValue<int64_t>("modType") == 3) {
            SupportLayer::onRequestAccess(sender);
            GM->m_hasRP = 0;
        }
        else {
            auto popup = UploadActionPopup::create(
                nullptr,
                "Loading..."
            );

            popup->show();

            popup->runAction(
                CCSequence::create(
                    CCDelayTime::create(0.5f),
                    CCCallFunc::create(
                        this,
                        callfunc_selector(MySupportLayer::DelayMod)
                    ),
                    nullptr
                )
            );

            GM->m_hasRP =
                Mod::get()->getSettingValue<int64_t>("modType");
        }
    }
};

// Elder Moderator's Delete Button

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        if (
            Mod::get()->getSettingValue<int64_t>("modType") != 1 ||
            Mod::get()->getSettingValue<int64_t>("modType") != 2
        ) {
            return true;
        }

        auto menu = getChildByID("left-side-menu");

        if (!menu)
            return true;

        if (menu->getChildByID("elder-delete-button"))
            return true;

        auto deleteSprite =
            CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");

        if (!deleteSprite)
            return true;

        deleteSprite->setScale(0.85f);

        auto deleteButton = CCMenuItemSpriteExtra::create(
            deleteSprite,
            this,
            menu_selector(MyLevelInfoLayer::onElderDelete)
        );

        if (!deleteButton)
            return true;

        deleteButton->setID("elder-delete-button");

        menu->addChild(deleteButton);
        menu->updateLayout();

        return true;
    }

        void onElderDelete(CCObject*) {
           FLAlertLayer::create(
           "Level Deleted",
           "The level has been removed from the server",
           "OK"
       )->show();
   }

    void levelDeleteFailed(int a1) {
        if (
            Mod::get()->getSettingValue<int64_t>("modType") == 1 ||
            Mod::get()->getSettingValue<int64_t>("modType") == 2
        ) {
            FLAlertLayer::create(
                "Level Deleted",
                "The level has been removed from the server",
                "OK"
            )->show();

            for (auto pObj :
                 CCArrayExt<CCObject*>(this->getChildren())) {

                if (auto loadingCircle =
                        typeinfo_cast<LoadingCircle*>(pObj)) {

                    loadingCircle->setVisible(false);
                }
            }
        }
        else {
            LevelInfoLayer::levelDeleteFailed(a1);
        }
    }
};

// Devlopper's Rate Stars Layer

class $modify(MyRateStarsLayer, RateStarsLayer) {
    bool init(
        int levelID,
        bool platformer,
        bool moderator
    ) {
        if (!RateStarsLayer::init(
            levelID,
            platformer,
            moderator
        )) {
            return false;
        }

        for (auto child :
             CCArrayExt<CCNode*>(this->getChildren())) {

            if (auto label =
                    typeinfo_cast<CCLabelBMFont*>(child)) {

                if (
                    std::string(label->getString()) ==
                    "Rate Stars"
                ) {
                    label->setString("DEV: Set Stars");
                    break;
                }
            }
        }

        return true;
    }

    void onSubmit(CCObject* sender) {
        auto popup = UploadActionPopup::create(
            nullptr,
            "Sending rating..."
        );

        if (!popup) {
            RateStarsLayer::onSubmit(sender);
            return;
        }

        popup->show();

        popup->runAction(
            CCSequence::create(
                CCDelayTime::create(0.5f),
                CCCallFunc::create(
                    this,
                    callfunc_selector(MyRateStarsLayer::delayRate)
                ),
                nullptr
            )
        );
    }

    void delayRate() {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj :
             CCArrayExt<CCObject*>(scene->getChildren())) {

            if (auto popup =
                    typeinfo_cast<UploadActionPopup*>(pObj)) {

                popup->showSuccessMessage(
                    "Rating Submitted!"
                );

                break;
            }
        }
    }
};
