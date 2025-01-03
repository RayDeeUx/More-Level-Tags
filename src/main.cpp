#include <Geode/modify/LevelInfoLayer.hpp>
#include <regex>

#define getInt Mod::get()->getSettingValue<int64_t>

static const std::regex shaderAbuseRegex(R"(;1,(?:2904|2905|2907|2909|2910|2911|2912|2913|2914|2915|2916|2917|2919|2920|2921|2922|2923|2924),)");
static const std::regex cameraAbuseRegex(R"(;1,(?:1913|1914|1916|2015|2016|2062|2901|2925),)");

using namespace geode::prelude;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	struct Fields {
		CCNode* menu = nullptr;
		CCNode* twoPlayer = nullptr;
		CCNode* shaderAbuse = nullptr;
		CCNode* cameraAbuse = nullptr;
		CCNode* legacyShip = nullptr;
		CCNode* legacyRobot = nullptr;
		CCNode* startFlipped = nullptr;
		CCNode* dynamicHeight = nullptr;
		CCNode* multiRotate = nullptr;
		CCNode* negativeScale = nullptr;
		CCNode* twoPointTwo = nullptr;
		// TODO: STACKOVERFLOW TO COUNT OCCURRENCES
		// `;1,<OBJECT_ID>,` fmt::format
		// TODO: VIEW FUNGAL SHIFT TO FIND OCCURRENCES OF SUBSTRING
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void hideTags(const GJGameLevel* theLevel) {
		std::string decomp = ZipUtils::decompressString(theLevel->m_levelString, true, 0);
		if (decomp.empty() || m_fields->buttons.empty()) return;
		if (m_fields->twoPlayer)
			setNodeVisibleBasedOnCondition(m_fields->twoPlayer, theLevel->m_twoPlayerMode);
		if (m_fields->twoPlayer)
			setNodeVisibleBasedOnCondition(m_fields->shaderAbuse, getInt("shaderAbuseTolerance") > -1 && frequencyOfRegexPatternInString(shaderAbuseRegex, decomp) > getInt("shaderAbuseTolerance"));
		if (m_fields->twoPlayer)
			setNodeVisibleBasedOnCondition(m_fields->cameraAbuse, getInt("cameraAbuseTolerance") > -1 && frequencyOfRegexPatternInString(cameraAbuseRegex, decomp) > getInt("cameraAbuseTolerance"));
		if (m_fields->legacyShip)
			setNodeVisibleBasedOnCondition(m_fields->legacyShip, utils::string::contains(decomp, "kA32,0"));
		if (m_fields->legacyRobot)
			setNodeVisibleBasedOnCondition(m_fields->legacyRobot, utils::string::contains(decomp, "kA34,0"));
		if (m_fields->startFlipped)
			setNodeVisibleBasedOnCondition(m_fields->startFlipped, utils::string::contains(decomp, "kA11,1"));
		if (m_fields->dynamicHeight)
			setNodeVisibleBasedOnCondition(m_fields->dynamicHeight, utils::string::contains(decomp, "kA37,0"));
		if (m_fields->multiRotate)
			setNodeVisibleBasedOnCondition(m_fields->multiRotate, utils::string::contains(decomp, "kA27,0"));
		if (m_fields->twoPointTwo)
			setNodeVisibleBasedOnCondition(m_fields->twoPointTwo, utils::string::contains(decomp, "kA40,0"));
		if (m_fields->negativeScale)
			setNodeVisibleBasedOnCondition(m_fields->negativeScale, utils::string::contains(decomp, "kA33,0"));
	}
	void setNodeVisibleBasedOnCondition(cocos2d::CCNode* node, bool condition) {
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
	static long frequencyOfRegexPatternInString(const std::regex& pattern, const std::string& fullString) {
		const auto start = std::sregex_iterator(fullString.begin(), fullString.end(), pattern);
		const auto finish = std::sregex_iterator();
		return std::distance(start, finish);
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

		const auto cube = CCSprite::createWithSpriteFrameName("2p.png"_spr);
		m_fields->twoPlayer = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->twoPlayer, "two-player"_spr);

		const auto shaderAbuse = createTagSprite("edit_eShaderBtn_001.png");
		m_fields->shaderAbuse = CircleButtonSprite::create(shaderAbuse, CircleBaseColor::DarkPurple, CircleBaseSize::Large);
		pushBackAddChild(m_fields->shaderAbuse, "shader-abuse"_spr);

		const auto cameraAbuse = CCSprite::createWithSpriteFrameName("cameraAbuse.png"_spr);
		m_fields->cameraAbuse = CircleButtonSprite::create(cameraAbuse, CircleBaseColor::DarkPurple, CircleBaseSize::Large);
		pushBackAddChild(m_fields->cameraAbuse, "camera-abuse"_spr);

		const auto ship = createTagSprite("portal_04_extra_2_001.png");
		m_fields->legacyShip = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		pushBackAddChild(m_fields->legacyShip, "legacy-ship"_spr);

		const auto robot = createTagSprite("portal_14_extra_2_001.png");
		m_fields->legacyRobot = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->legacyRobot, "legacy-robot"_spr);

		const auto flip = createTagSprite("portal_02_extra_2_001.png");
		m_fields->startFlipped = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		pushBackAddChild(m_fields->startFlipped, "flip-gravity"_spr);

		const auto height = createTagSprite("portal_19_extra_2_001.png");
		m_fields->dynamicHeight = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->dynamicHeight, "dynamic-height"_spr);

		const auto rotate = createTagSprite("edit_eRotateComBtn_001.png");
		m_fields->multiRotate = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->multiRotate, "multi-rotate"_spr);

		const auto tpt = createTagSprite("portal_18_extra_2_001.png");
		m_fields->twoPointTwo = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->twoPointTwo, "two-point-two"_spr);

		const auto negScale = createTagSprite("edit_eScaleComBtn_001.png");
		m_fields->negativeScale = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		pushBackAddChild(m_fields->negativeScale, "negative-scale"_spr);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::hideTags(theLevel);
		else {
			if (m_fields->twoPlayer) pushBackMakeInvis(m_fields->twoPlayer);
			if (m_fields->shaderAbuse) pushBackMakeInvis(m_fields->shaderAbuse);
			if (m_fields->cameraAbuse) pushBackMakeInvis(m_fields->cameraAbuse);
			if (m_fields->legacyShip) pushBackMakeInvis(m_fields->legacyShip);
			if (m_fields->legacyRobot) pushBackMakeInvis(m_fields->legacyRobot);
			if (m_fields->startFlipped) pushBackMakeInvis(m_fields->startFlipped);
			if (m_fields->dynamicHeight) pushBackMakeInvis(m_fields->dynamicHeight);
			if (m_fields->multiRotate) pushBackMakeInvis(m_fields->multiRotate);
			if (m_fields->twoPointTwo) pushBackMakeInvis(m_fields->twoPointTwo);
			if (m_fields->negativeScale) pushBackMakeInvis(m_fields->negativeScale);
		}

		int i = 0;
		for (auto node : CCArrayExt<CCNode*>(m_fields->menu->getChildren())) {
			node->setPositionX((18 / 0.15f) * static_cast<float>(i));
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
			if (m_fields->twoPlayer) m_fields->twoPlayer->setVisible(false);
			if (m_fields->shaderAbuse) m_fields->shaderAbuse->setVisible(false);
			if (m_fields->cameraAbuse) m_fields->cameraAbuse->setVisible(false);
			if (m_fields->legacyShip) m_fields->legacyShip->setVisible(false);
			if (m_fields->legacyRobot) m_fields->legacyRobot->setVisible(false);
			if (m_fields->startFlipped) m_fields->startFlipped->setVisible(false);
			if (m_fields->dynamicHeight) m_fields->dynamicHeight->setVisible(false);
			if (m_fields->multiRotate) m_fields->multiRotate->setVisible(false);
			if (m_fields->twoPointTwo) m_fields->twoPointTwo->setVisible(false);
			if (m_fields->negativeScale) m_fields->negativeScale->setVisible(false);
		}

		int i = 0;
		for (auto node : CCArrayExt<CCNode*>(m_fields->menu->getChildren())) {
			node->setPositionX((18 / 0.15f) * static_cast<float>(i));
			i++;
		}

		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(true);

		m_fields->isRefreshing = false;
	}
};