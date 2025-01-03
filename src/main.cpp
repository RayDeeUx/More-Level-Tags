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
		// TODO: STACKOVERFLOW TO COUNT OCCURRENCES
		// `;1,<OBJECT_ID>,` fmt::format
		// TODO: VIEW FUNGAL SHIFT TO FIND OCCURRENCES OF SUBSTRING
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void hideTags(GJGameLevel* theLevel) {
		std::string decomp = ZipUtils::decompressString(theLevel->m_levelString, true, 0);
		if (decomp.empty() || m_fields->buttons.empty()) return;
		if (m_fields->legacyShip)
			setXVisibleBasedOnY(m_fields->legacyShip, utils::string::contains(decomp, "kA32,0"));
		if (m_fields->legacyRobot)
			setXVisibleBasedOnY(m_fields->legacyRobot, utils::string::contains(decomp, "kA34,0"));
		if (m_fields->startFlipped)
			setXVisibleBasedOnY(m_fields->startFlipped, utils::string::contains(decomp, "kA11,1"));
		if (m_fields->dynamicHeight)
			setXVisibleBasedOnY(m_fields->dynamicHeight, utils::string::contains(decomp, "kA37,0"));
		if (m_fields->multiRotate)
			setXVisibleBasedOnY(m_fields->multiRotate, utils::string::contains(decomp, "kA27,0"));
		if (m_fields->twoPointTwo)
			setXVisibleBasedOnY(m_fields->twoPointTwo, utils::string::contains(decomp, "kA40,0"));
		if (m_fields->negativeScale)
			setXVisibleBasedOnY(m_fields->negativeScale, utils::string::contains(decomp, "kA33,0"));
		if (m_fields->twoPlayer)
			setXVisibleBasedOnY(m_fields->twoPlayer, theLevel->m_twoPlayerMode);
	}
	void setXVisibleBasedOnY(cocos2d::CCNode* node, bool condition) {
		if (!node) return;
		node->setVisible(condition);
		if (node->isVisible()) return;
		node->removeFromParent();
		if (!m_fields->buttons.empty()) std::erase(m_fields->buttons, node);
	}
	void pushBackMakeInvis(cocos2d::CCNode* node) {
		if (!node) return;
		m_fields->buttons.push_back(node);
		node->setVisible(false);
	}
	void pushBackAddChild(cocos2d::CCNode* node, std::string&& id) {
		if (!node || !m_fields->menu) return;
		node->setID(id);
		m_fields->buttons.push_back(node);
		m_fields->menu->addChild(node);
	}
	static cocos2d::CCSprite* createTagSprite(const char* frameName) {
		cocos2d::CCSprite* sprite = CCSprite::createWithSpriteFrameName(frameName);
		sprite->setScale(.5f);
		return sprite;
	}
	int frequencyOfXInY(std::string&& pattern, const std::string& fullString) {
		const auto patternSize = pattern.length();
		const auto fullStringSize = fullString.length();
		int result = 0;

		for (int i = 0; i <= fullStringSize - patternSize; i++) {
			int j;
			for (j = 0; j < patternSize; j++) if (fullString[i + j] != pattern[j]) break;
			if (j == patternSize) result++;
		}

		return result;
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

		auto ship = createTagSprite("portal_04_extra_2_001.png");
		m_fields->legacyShip = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		pushBackAddChild(m_fields->legacyShip, "legacy-ship"_spr);

		auto robot = createTagSprite("portal_14_extra_2_001.png");
		m_fields->legacyRobot = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->legacyRobot, "legacy-robot"_spr);

		auto flip = createTagSprite("portal_02_extra_2_001.png");
		m_fields->startFlipped = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->startFlipped, "flip-gravity"_spr);

		auto height = createTagSprite("portal_19_extra_2_001.png");
		m_fields->dynamicHeight = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->dynamicHeight, "dynamic-height"_spr);

		auto rotate = createTagSprite("edit_eRotateComBtn_001.png");
		m_fields->multiRotate = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->multiRotate, "multi-rotate"_spr);

		auto tpt = createTagSprite("portal_18_extra_2_001.png");
		m_fields->twoPointTwo = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->twoPointTwo, "two-point-two"_spr);

		auto negScale = createTagSprite("edit_eScaleComBtn_001.png");
		m_fields->negativeScale = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->negativeScale, "negative-scale"_spr);

		auto cube = CCSprite::createWithSpriteFrameName("2p.png"_spr);
		m_fields->twoPlayer = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->twoPlayer, "two-player"_spr);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::hideTags(theLevel);
		else {
			if (m_fields->legacyShip) pushBackMakeInvis(m_fields->legacyShip);
			if (m_fields->legacyRobot) pushBackMakeInvis(m_fields->legacyRobot);
			if (m_fields->startFlipped) pushBackMakeInvis(m_fields->startFlipped);
			if (m_fields->dynamicHeight) pushBackMakeInvis(m_fields->dynamicHeight);
			if (m_fields->multiRotate) pushBackMakeInvis(m_fields->multiRotate);
			if (m_fields->twoPointTwo) pushBackMakeInvis(m_fields->twoPointTwo);
			if (m_fields->negativeScale) pushBackMakeInvis(m_fields->negativeScale);
			if (m_fields->twoPlayer) pushBackMakeInvis(m_fields->twoPlayer);
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

		const auto creatorInfoMenu = getChildByID("creator-info-menu");

		if (!m_fields->menu || !creatorInfoMenu) {
			m_fields->isRefreshing = false;
			return;
		}

		if (!theLevel->m_levelString.empty()) {
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

		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(true);

		m_fields->isRefreshing = false;
	}
};