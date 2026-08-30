#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/binding/UploadActionPopup.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/SupportLayer.hpp>
#include <Geode/modify/RateStarsLayer.hpp>
#include <Geode/modify/RateDemonLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Developer's Set Featured button

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

        m_valueLabel->setScale(0.9f);
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
            sprite->setScale(0.9f);

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

// Support Layer Success Message

class modCheck : public CCObject {
public:
    void DelayMod(CCObject* sender) {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj : CCArrayExt<CCObject*>(
            static_cast<CCScene*>(scene)->getChildren()
        )) {
            if (instanceof<UploadActionPopup>(pObj)) {
                auto Check = static_cast<UploadActionPopup*>(pObj);

                if (Mod::get()->getSettingValue<int64_t>("modType") == 2) {
                    Check->showSuccessMessage(
                        "Success! Developer access granted."
                    );
                }
            }
        }
    }

// Rating Submitted! Fake Message

    void DelayRate(CCObject* sender) {
        auto scene = CCDirector::get()->getRunningScene();

        for (auto pObj : CCArrayExt<CCObject*>(
            static_cast<CCScene*>(scene)->getChildren()
        )) {
            if (instanceof<UploadActionPopup>(pObj)) {
                auto Check = static_cast<UploadActionPopup*>(pObj);
                Check->showSuccessMessage("Rating submitted!");
            }
        }
    }
};

// Support Layer Loading Message

class $modify(SupportLayer) {
    void onRequestAccess(CCObject* sender) {
        auto GM = GameManager::sharedState();

        if (Mod::get()->getSettingValue<int64_t>("modType") == 3) {
            SupportLayer::onRequestAccess(sender);
            GM->m_hasRP = 0;
        }
        else {
            auto popup = UploadActionPopup::create(nullptr, "Loading...");
            popup->show();

            auto checker = new modCheck();
            checker->autorelease();

            popup->runAction(CCSequence::create(
                CCDelayTime::create(0.5f),
                CCCallFunc::create(
                    checker,
                    callfunc_selector(modCheck::DelayMod)
                ),
                nullptr
            ));

            GM->m_hasRP =
                Mod::get()->getSettingValue<int64_t>("modType");
        }
    }
};


// Moderator's Suggest Stars Layer

class $modify(RateStarsLayer) {
    void onRate(CCObject* sender) {
        auto layer = static_cast<CCLayer*>(
            this->getChildren()->objectAtIndex(0)
        );

        if (layer->getChildrenCount() == 3) {
            auto popup = UploadActionPopup::create(
                nullptr,
                "Sending rating..."
            );

            popup->show();

            auto checker = new modCheck();
            checker->autorelease();

            popup->runAction(CCSequence::create(
                CCDelayTime::create(0.5f),
                CCCallFunc::create(
                    checker,
                    callfunc_selector(modCheck::DelayRate)
                ),
                nullptr
            ));
        }
        else {
            RateStarsLayer::onRate(sender);
        }
    }
};

// Moderator's Rate Demon Layer

class $modify(RateDemonLayer) {
    void onRate(CCObject* sender) {
        auto popup = UploadActionPopup::create(nullptr, "Sending rating...");
        popup->show();
        popup->runAction(CCSequence::create(
            CCDelayTime::create(0.5),
            CCCallFunc::create(this, callfunc_selector(modCheck::DelayRate)),
            nullptr
        ));
    }
};

// Elder Moderator's Delete Button

class $modify(LevelInfoLayer) {
    void levelDeleteFailed(int a1) {
        auto scene = CCDirector::get()->getRunningScene();

        if (Mod::get()->getSettingValue<int64_t>("modType") == 1 ||
            Mod::get()->getSettingValue<int64_t>("modType") == 2) {

            FLAlertLayer::create("Level Deleted", "The level has been removed from the server", "OK")->show();

            for (auto pObj : CCArrayExt<CCObject*>(this->getChildren())) {
                if (instanceof<LoadingCircle>(pObj)) {
                    auto loadingCircle = static_cast<LoadingCircle*>(pObj);
                    loadingCircle->setVisible(false);
                }
            }
        }
        else {
            LevelInfoLayer::levelDeleteFailed(a1);
        }
    }
};

// One-time popup

class $modify(CongratsMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        log::info("CongratsPopup: MenuLayer::init reached");
        
        bool alreadyShown = Mod::get()->getSavedValue<bool>("has-shown-congrats-debug", false);
        log::info("CongratsPopup: alreadyShown = {}", alreadyShown);

        if (!alreadyShown) {
            log::info("CongratsPopup: queuing popup for next frame");

            Loader::get()->queueInMainThread([]() {
                log::info("CongratsPopup: inside queueInMainThread, about to create popup");

                auto popup = createQuickPopup(
                    "Congrats",
                    "You just got the RobTop access! Please go to the Settings, after click Help and then Req.",
                    "OK",
                    nullptr,
                    [](auto, bool) {
                        log::info("CongratsPopup: OK pressed");
                    }
                );

                log::info("CongratsPopup: createQuickPopup returned {}", fmt::ptr(popup));

                Mod::get()->setSavedValue<bool>("has-shown-congrats-debug", true);
            });
        }

        return true;
    }
};

// Developer's Rate Stars Layer

class $modify(RSLHook, RateStarsLayer) {

    struct Fields {
        bool m_coinColored = false;
        CCMenuItemSpriteExtra* m_coinBtn = nullptr;
        CCSprite* m_coinSprite = nullptr;
    };

    static CCScale9Sprite* findPanelBg(CCNode* node) {
        auto children = node->getChildren();
        if (!children) return nullptr;

        for (auto child : CCArrayExt<CCObject*>(children)) {
            auto childNode = static_cast<CCNode*>(child);
            if (auto s9 = typeinfo_cast<CCScale9Sprite*>(childNode)) {
                return s9;
            }
        }
        for (auto child : CCArrayExt<CCObject*>(children)) {
            auto childNode = static_cast<CCNode*>(child);
            if (auto found = findPanelBg(childNode)) {
                return found;
            }
        }
        return nullptr;
    }

    bool init(int levelID, bool platformer, bool moderator) {
        if (!RateStarsLayer::init(levelID, platformer, moderator)) return false;

        auto root = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
        for (auto child : CCArrayExt<CCObject*>(root->getChildren())) {
            if (auto label = typeinfo_cast<CCLabelBMFont*>(child)) {
                label->setString("DEV: Set Stars");
                break;
            }
        }

        auto panelBg = findPanelBg(this);
        auto attachTarget = panelBg ? static_cast<CCNode*>(panelBg) : static_cast<CCNode*>(root);
        auto targetSize = attachTarget->getContentSize();

        auto coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png");
        coinSprite->setScale(1.6f);
        coinSprite->setColor({150, 150, 150});

        auto coinBtn = CCMenuItemSpriteExtra::create(
            coinSprite,
            this,
            menu_selector(RSLHook::onToggleDevCoin)
        );

        auto coinSize = coinSprite->getContentSize();
        coinBtn->setContentSize(coinSize);
        coinSprite->setPosition({coinSize.width / 2.f, coinSize.height / 2.f});
        coinBtn->setPosition({0.f, targetSize.height});

        auto coinMenu = CCMenu::create();
        coinMenu->addChild(coinBtn);
        coinMenu->setPosition({0.f, 0.f});
        coinMenu->setZOrder(100);
        attachTarget->addChild(coinMenu, 100);

        m_fields->m_coinBtn = coinBtn;
        m_fields->m_coinSprite = coinSprite;

        auto zeroSprite = ButtonSprite::create("0", 36, false, "bigFont.fnt", "GJ_button_04.png", 30.f, 0.5.f);

        auto zeroBtn = CCMenuItemSpriteExtra::create(
            zeroSprite,
            this,
            menu_selector(RSLHook::onDevZeroClicked)
        );
        zeroBtn->setPosition({targetSize.width, targetSize.height});

        coinMenu->addChild(zeroBtn);

        return true;
    }

    void onToggleDevCoin(CCObject*) {
        m_fields->m_coinColored = !m_fields->m_coinColored;

        m_fields->m_coinSprite->setColor(
            m_fields->m_coinColored
                ? ccColor3B{255, 255, 255}
                : ccColor3B{150, 150, 150}
        );
    }

    void onDevZeroClicked(CCObject*) {
        this->onClose(nullptr);
    }

    void onRate(CCObject* sender) {
        auto popup = UploadActionPopup::create(nullptr, "Sending rating...");
        popup->show();

        auto checker = new modCheck();
        checker->autorelease();

        popup->runAction(CCSequence::create(
            CCDelayTime::create(0.5f),
            CCCallFunc::create(checker, callfunc_selector(modCheck::DelayRate)),
            nullptr
        ));
    }
};
