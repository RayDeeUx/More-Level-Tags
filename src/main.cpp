#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	struct Fields {
		CCNode* menu = nullptr;
		CCNode* legacyShip = nullptr;
		CCNode* legacyRobot = nullptr;
		CCNode* startFlipped = nullptr;
		CCNode* dynamicHeight = nullptr;
		CCNode* multiRotate = nullptr;
		CCNode* negativeScale = nullptr;
		CCNode* twoPointTwo = nullptr;
		CCNode* twoPlayer = nullptr;
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void hideTags(GJGameLevel* theLevel) {
		std::string decomp = ZipUtils::decompressString(theLevel->m_levelString, true, 0);
		if (decomp.empty() || m_fields->buttons.empty()) return;
		if (m_fields->legacyShip)
			judgementDay(m_fields->legacyShip, utils::string::contains(decomp, "kA32,0"));
		if (m_fields->legacyRobot)
			judgementDay(m_fields->legacyRobot, utils::string::contains(decomp, "kA34,0"));
		if (m_fields->startFlipped)
			judgementDay(m_fields->startFlipped, utils::string::contains(decomp, "kA11,1"));
		if (m_fields->dynamicHeight)
			judgementDay(m_fields->dynamicHeight, utils::string::contains(decomp, "kA37,0"));
		if (m_fields->multiRotate)
			judgementDay(m_fields->multiRotate, utils::string::contains(decomp, "kA27,0"));
		if (m_fields->twoPointTwo)
			judgementDay(m_fields->twoPointTwo, utils::string::contains(decomp, "kA40,0"));
		if (m_fields->negativeScale)
			judgementDay(m_fields->negativeScale, utils::string::contains(decomp, "kA33,0"));
		if (m_fields->twoPlayer)
			judgementDay(m_fields->twoPlayer, theLevel->m_twoPlayerMode);
	}
	void judgementDay(cocos2d::CCNode* node, bool condition) {
		node->setVisible(condition);
		if (node->isVisible()) return;
		node->removeFromParent();
		if (!m_fields->buttons.empty()) std::erase(m_fields->buttons, node);
	}
	void onUpdate(cocos2d::CCObject* sender) {
		m_fields->isRefreshing = true;
		LevelInfoLayer::onUpdate(sender);
	}
	void onPlay(cocos2d::CCObject* sender) {
		m_fields->isRefreshing = true;
		LevelInfoLayer::onPlay(sender);
	}
	bool init(GJGameLevel* theLevel, bool p1) {
		m_fields->isRefreshing = false;
		if (!m_fields->buttons.empty()) m_fields->buttons.clear();

		if (!LevelInfoLayer::init(theLevel, p1)) return false;
		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;

		int authorID = theLevel->m_accountID.value();

		if (authorID == 71 && !getChildByIDRecursive("right-side-menu")->isVisible()) return true; // avoid false positives with robtop's levels
		if (authorID == 19293579 && !Mod::get()->getSettingValue<bool>("robsVault")) return true;

		auto creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) return true;

		m_fields->menu = CCMenu::create();
		m_fields->menu->setContentSize(ccp(0, 0));
		m_fields->menu->setID("more-level-tags-menu"_spr);
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(false);
		addChild(m_fields->menu);

		auto ship = CCSprite::createWithSpriteFrameName("portal_04_extra_2_001.png");
		ship->setScale(.5f);
		m_fields->legacyShip = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		m_fields->legacyShip->setID("legacy-ship"_spr);
		m_fields->buttons.push_back(m_fields->legacyShip);
		m_fields->menu->addChild(m_fields->legacyShip);

		auto robot = CCSprite::createWithSpriteFrameName("portal_14_extra_2_001.png");
		robot->setScale(.5f);
		m_fields->legacyRobot = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->legacyRobot->setID("legacy-robot"_spr);
		m_fields->buttons.push_back(m_fields->legacyRobot);
		m_fields->menu->addChild(m_fields->legacyRobot);

		auto flip = CCSprite::createWithSpriteFrameName("portal_02_extra_2_001.png");
		flip->setScale(.5f);
		m_fields->startFlipped = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->startFlipped->setID("flip-gravity"_spr);
		m_fields->buttons.push_back(m_fields->startFlipped);
		m_fields->menu->addChild(m_fields->startFlipped);

		auto height = CCSprite::createWithSpriteFrameName("portal_19_extra_2_001.png");
		height->setScale(.5f);
		m_fields->dynamicHeight = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->dynamicHeight->setID("dynamic-height"_spr);
		m_fields->buttons.push_back(m_fields->dynamicHeight);
		m_fields->menu->addChild(m_fields->dynamicHeight);

		auto rotate = CCSprite::createWithSpriteFrameName("edit_eRotateComBtn_001.png");
		rotate->setScale(.5f);
		m_fields->multiRotate = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->multiRotate->setID("multi-rotate"_spr);
		m_fields->buttons.push_back(m_fields->multiRotate);
		m_fields->menu->addChild(m_fields->multiRotate);

		auto tpt = CCSprite::createWithSpriteFrameName("portal_18_extra_2_001.png");
		tpt->setScale(.5f);
		m_fields->twoPointTwo = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->twoPointTwo->setID("two-point-two"_spr);
		m_fields->buttons.push_back(m_fields->twoPointTwo);
		m_fields->menu->addChild(m_fields->twoPointTwo);

		auto negScale = CCSprite::createWithSpriteFrameName("edit_eScaleComBtn_001.png");
		negScale->setScale(.5f);
		m_fields->negativeScale = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->negativeScale->setID("negative-scale"_spr);
		m_fields->buttons.push_back(m_fields->negativeScale);
		m_fields->menu->addChild(m_fields->negativeScale);

		auto cube = CCSprite::createWithSpriteFrameName("2p.png"_spr);
		m_fields->twoPlayer = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->twoPlayer->setID("two-player"_spr);
		m_fields->buttons.push_back(m_fields->twoPlayer);
		m_fields->menu->addChild(m_fields->twoPlayer);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::hideTags(theLevel);
		else {
			if (m_fields->legacyShip) {
				m_fields->buttons.push_back(m_fields->legacyShip);
				m_fields->legacyShip->setVisible(false);
			}
			if (m_fields->legacyRobot) {
				m_fields->buttons.push_back(m_fields->legacyRobot);
				m_fields->legacyRobot->setVisible(false);
			}
			if (m_fields->startFlipped) {
				m_fields->buttons.push_back(m_fields->startFlipped);
				m_fields->startFlipped->setVisible(false);
			}
			if (m_fields->dynamicHeight) {
				m_fields->buttons.push_back(m_fields->dynamicHeight);
				m_fields->dynamicHeight->setVisible(false);
			}
			if (m_fields->multiRotate) {
				m_fields->buttons.push_back(m_fields->multiRotate);
				m_fields->multiRotate->setVisible(false);
			}
			if (m_fields->twoPointTwo) {
				m_fields->buttons.push_back(m_fields->twoPointTwo);
				m_fields->twoPointTwo->setVisible(false);
			}
			if (m_fields->negativeScale) {
				m_fields->buttons.push_back(m_fields->negativeScale);
				m_fields->negativeScale->setVisible(false);
			}
			if (m_fields->twoPlayer) {
				m_fields->buttons.push_back(m_fields->twoPlayer);
				m_fields->twoPlayer->setVisible(false);
			}
		}

		int i = 0;
		for (auto node : CCArrayExt<CCNode*>(m_fields->menu->getChildren())) {
			node->setPositionX((18 / 0.15f) * i);
			i++;
		}

		m_fields->menu->setVisible(true);

		return true;
	}

	virtual void levelDownloadFinished(GJGameLevel* theLevel) {
		LevelInfoLayer::levelDownloadFinished(theLevel);

		if (m_fields->menu) {
			if (theLevel->m_levelString.size() > 1) {
				if (!m_fields->isRefreshing) {
					m_fields->menu->setVisible(false);
					MyLevelInfoLayer::hideTags(theLevel);
				}
			} else {
				if (m_fields->legacyShip) m_fields->legacyShip->setVisible(false);
				if (m_fields->legacyRobot) m_fields->legacyRobot->setVisible(false);
				if (m_fields->startFlipped) m_fields->startFlipped->setVisible(false);
				if (m_fields->dynamicHeight) m_fields->dynamicHeight->setVisible(false);
				if (m_fields->multiRotate) m_fields->multiRotate->setVisible(false);
				if (m_fields->twoPointTwo) m_fields->twoPointTwo->setVisible(false);
				if (m_fields->negativeScale) m_fields->negativeScale->setVisible(false);
				if (m_fields->twoPlayer) m_fields->twoPlayer->setVisible(false);
			}

			int i = 0;
			for (auto node : CCArrayExt<CCNode*>(m_fields->menu->getChildren())) {
				node->setPositionX((18 / 0.15f) * i);
				i++;
			}

			if (const auto creatorInfoMenu = getChildByID("creator-info-menu")) {
				m_fields->menu->setScale(0.15f);
				m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
				m_fields->menu->setVisible(true);
			}
		}
		m_fields->isRefreshing = false;
	}
};