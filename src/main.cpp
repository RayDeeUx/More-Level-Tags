#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	struct Fields {
		CCMenu* menu = nullptr;
		CCNode* legacyShip = nullptr;
		CCNode* legacyRobot = nullptr;
		CCNode* startFlipped = nullptr;
		CCNode* dynamicHeight = nullptr;
		CCNode* multiRotate = nullptr;
		CCNode* negativeScale = nullptr;
		CCNode* twoPointTwo = nullptr;
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void hideTags() {
		if (m_fields->buttons.empty()) return;
		std::string decomp = ZipUtils::decompressString(m_level->m_levelString, true, 0);
		if (decomp.size() > 1) {
			if ((m_fields->legacyShip != nullptr) && decomp.find("kA32,0")) {
				m_fields->legacyShip->setVisible(decomp.find("kA32,0") != std::string::npos);
				if (!m_fields->legacyShip->isVisible())
				{
					m_fields->legacyShip->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->legacyShip), m_fields->buttons.end());
				}
			}
			if ((m_fields->legacyRobot != nullptr) && decomp.find("kA34,0")) {
				m_fields->legacyRobot->setVisible(decomp.find("kA34,0") != std::string::npos);
				if (!m_fields->legacyRobot->isVisible())
				{
					m_fields->legacyRobot->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->legacyRobot), m_fields->buttons.end());
				}
			}
			if ((m_fields->startFlipped != nullptr) && decomp.find("kA11,1")) {
				m_fields->startFlipped->setVisible(decomp.find("kA11,1") != std::string::npos);
				if (!m_fields->startFlipped->isVisible())
				{
					m_fields->startFlipped->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->startFlipped), m_fields->buttons.end());
				}
			}
			if ((m_fields->dynamicHeight != nullptr) && decomp.find("kA37,0")) {
				m_fields->dynamicHeight->setVisible(decomp.find("kA37,0") != std::string::npos);
				if (!m_fields->dynamicHeight->isVisible())
				{
					m_fields->dynamicHeight->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->dynamicHeight), m_fields->buttons.end());
				}
			}
			if ((m_fields->multiRotate != nullptr) && decomp.find("kA27,0")) {
				m_fields->multiRotate->setVisible(decomp.find("kA27,0") != std::string::npos);
				if (!m_fields->multiRotate->isVisible())
				{
					m_fields->multiRotate->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->multiRotate), m_fields->buttons.end());
				}
			}
			if ((m_fields->twoPointTwo != nullptr) && decomp.find("kA40,0")) {
				m_fields->twoPointTwo->setVisible(decomp.find("kA40,0") != std::string::npos);
				if (!m_fields->twoPointTwo->isVisible())
				{
					m_fields->twoPointTwo->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->twoPointTwo), m_fields->buttons.end());
				}
			}
			if ((m_fields->negativeScale != nullptr) && decomp.find("kA33,0")) {
				m_fields->negativeScale->setVisible(decomp.find("kA33,0") != std::string::npos);
				if (!m_fields->negativeScale->isVisible())
				{
					m_fields->negativeScale->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->negativeScale), m_fields->buttons.end());
				}
			}
		}
	}
	void setupLevelInfo() {
		LevelInfoLayer::setupLevelInfo();

		if (!m_fields->buttons.empty()) { m_fields->buttons.clear(); }

		if (m_level->m_accountID.value() == 71 && !getChildByIDRecursive("right-side-menu")->isVisible()) { return; } // avoid false positives with robtop's levels

		auto creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) { return; }

		m_fields->menu = CCMenu::create();
		m_fields->menu->setContentSize(ccp(0, 0));
		m_fields->menu->setID("more-level-tags-menu"_spr);
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(as<CCMenu*>(creatorInfoMenu)->getPosition() + ccp(as<CCMenu*>(creatorInfoMenu)->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(false);
		addChild(m_fields->menu);

		auto ship = CCSprite::createWithSpriteFrameName("portal_04_extra_2_001.png");
		ship->setScale(3.5f);
		m_fields->legacyShip = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		m_fields->legacyShip->setID("legacy-ship"_spr);
		m_fields->buttons.push_back(m_fields->legacyShip);
		m_fields->menu->addChild(m_fields->legacyShip);

		auto robot = CCSprite::createWithSpriteFrameName("portal_14_extra_2_001.png");
		robot->setScale(3.5f);
		m_fields->legacyRobot = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->legacyRobot->setID("legacy-robot"_spr);
		m_fields->buttons.push_back(m_fields->legacyRobot);
		m_fields->menu->addChild(m_fields->legacyRobot);

		auto flip = CCSprite::createWithSpriteFrameName("portal_02_extra_2_001.png");
		flip->setScale(3.5f);
		m_fields->startFlipped = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->startFlipped->setID("flip-gravity"_spr);
		m_fields->buttons.push_back(m_fields->startFlipped);
		m_fields->menu->addChild(m_fields->startFlipped);

		auto height = CCSprite::createWithSpriteFrameName("portal_19_extra_2_001.png");
		height->setScale(3.5f);
		m_fields->dynamicHeight = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->dynamicHeight->setID("dynamic-height"_spr);
		m_fields->buttons.push_back(m_fields->dynamicHeight);
		m_fields->menu->addChild(m_fields->dynamicHeight);

		auto rotate = CCSprite::createWithSpriteFrameName("edit_eRotateComBtn_001.png");
		rotate->setScale(1.5f);
		m_fields->multiRotate = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->multiRotate->setID("multi-rotate"_spr);
		m_fields->buttons.push_back(m_fields->multiRotate);
		m_fields->menu->addChild(m_fields->multiRotate);

		auto tpt = CCSprite::createWithSpriteFrameName("portal_18_extra_2_001.png");
		tpt->setScale(3.5f);
		m_fields->twoPointTwo = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->twoPointTwo->setID("two-point-two"_spr);
		m_fields->buttons.push_back(m_fields->twoPointTwo);
		m_fields->menu->addChild(m_fields->twoPointTwo);

		auto negScale = CCSprite::createWithSpriteFrameName("edit_eScaleComBtn_001.png");
		negScale->setScale(1.5f);
		m_fields->negativeScale = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->negativeScale->setID("negative-scale"_spr);
		m_fields->buttons.push_back(m_fields->negativeScale);
		m_fields->menu->addChild(m_fields->negativeScale);

		if (m_level->m_twoPlayerMode)
		{
			auto cube = CCSprite::createWithSpriteFrameName("portal_03_extra_2_001.png");
			cube->setScale(3.5f);
			auto twoPlayer = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
			twoPlayer->setID("two-player"_spr);
			m_fields->menu->addChild(twoPlayer);
			m_fields->buttons.push_back(twoPlayer);
		}

		if (m_level->m_levelString.size() > 1) {
			MyLevelInfoLayer::hideTags();
		} else {
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
		}

		for (size_t i = 0; i < m_fields->buttons.size(); i++) {
			m_fields->buttons[i]->setPositionX((18 / 0.15f) * i);
		}

		m_fields->menu->setVisible(true);
	}

	virtual void levelDownloadFinished(GJGameLevel* p0) {
		LevelInfoLayer::levelDownloadFinished(p0);

		if (m_fields->menu) {
			m_fields->menu->setVisible(false);
			if (p0->m_levelString.size() > 1) {
				std::string decomp = ZipUtils::decompressString(p0->m_levelString, true, 0);
				MyLevelInfoLayer::hideTags();
			}

			for (size_t i = 0; i < m_fields->buttons.size(); i++) {
				m_fields->buttons[i]->setPositionX((18 / 0.15f) * i);
			}

			if (const auto creatorInfoMenu = getChildByID("creator-info-menu")) {
				m_fields->menu->setScale(0.15f);
				m_fields->menu->setPosition(as<CCMenu*>(creatorInfoMenu)->getPosition() + ccp(as<CCMenu*>(creatorInfoMenu)->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
				m_fields->menu->setVisible(true);
			}
		}
	}
};