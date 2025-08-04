#include <Geode/modify/LevelInfoLayer.hpp>
#include <regex>

#define getInt Mod::get()->getSettingValue<int64_t>
#define getBool Mod::get()->getSettingValue<bool>

static const std::regex shaderIntoleranceRegex(R"(;1,(?:2904|2905|2907|2909|2910|2911|2912|2913|2914|2915|2916|2917|2919|2920|2921|2922|2923|2924),)");
static const std::regex cameraIntoleranceRegex(R"(;1,(?:1913|1914|1916|2015|2016|2062|2901|2925),)");

static const std::set shaderIntoleranceVector = {2904, 2905, 2907, 2909, 2910, 2911, 2912, 2913, 2914, 2915, 2916, 2917, 2919, 2920, 2921, 2922, 2923, 2924};
static const std::set cameraIntoleranceVector = {1913, 1914, 1916, 2015, 2016, 2062, 2901, 2925};

using namespace geode::prelude;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	struct Fields {
		CCNode* menu = nullptr;
		CCNode* twoPlayer = nullptr;
		CCNode* shaderIntolerance = nullptr;
		CCNode* cameraIntolerance = nullptr;
		CCNode* legacyShip = nullptr;
		CCNode* legacyRobot = nullptr;
		CCNode* startFlipped = nullptr;
		CCNode* dynamicHeight = nullptr;
		CCNode* multiRotate = nullptr;
		CCNode* negativeScale = nullptr;
		CCNode* twoPointTwo = nullptr;
		bool isRefreshing = false;
	};
	void displayTags(const GJGameLevel* theLevel) {
		const std::string& fullLevelString = ZipUtils::decompressString(theLevel->m_levelString, true, 0);
		if (fullLevelString.empty()) return;

		const int locationOfBackgroundKey = static_cast<int>(fullLevelString.find("kA6,"));
		if (locationOfBackgroundKey == std::string::npos) return;
		const int locationOfFirstSemicolon = static_cast<int>(fullLevelString.find_first_of(';', locationOfBackgroundKey));
		if (locationOfFirstSemicolon == std::string::npos) return;
		const std::string& levelStringKeys = fullLevelString.substr(locationOfBackgroundKey, std::abs(locationOfBackgroundKey - locationOfFirstSemicolon));
		if (levelStringKeys.empty()) return;

		displayTagIf(m_fields->twoPlayer, theLevel->m_twoPlayerMode);
		if (getBool("advancedShaderAbuseTolerance")) {
			displayTagIf(m_fields->shaderIntolerance, getInt("shaderAbuseTolerance") > -1 && frequencyOfRegexPatternInString(shaderIntoleranceRegex, fullLevelString) > getInt("shaderAbuseTolerance"));
		} else {
			displayTagIf(m_fields->shaderIntolerance, getInt("shaderAbuseTolerance") > -1 && useVectorToFindFrequency(shaderIntoleranceVector, fullLevelString) > getInt("shaderAbuseTolerance"));
		}
		if (getBool("advancedCameraAbuseTolerance")) {
			displayTagIf(m_fields->cameraIntolerance, getInt("cameraAbuseTolerance") > -1 && frequencyOfRegexPatternInString(cameraIntoleranceRegex, fullLevelString) > getInt("cameraAbuseTolerance"));
		} else {
			displayTagIf(m_fields->cameraIntolerance, getInt("cameraAbuseTolerance") > -1 && useVectorToFindFrequency(cameraIntoleranceVector, fullLevelString) > getInt("cameraAbuseTolerance"));
		}
		displayTagIf(m_fields->legacyShip, utils::string::contains(levelStringKeys, "kA32,0"));
		displayTagIf(m_fields->legacyRobot, utils::string::contains(levelStringKeys, "kA34,0"));
		displayTagIf(m_fields->startFlipped, utils::string::contains(levelStringKeys, "kA11,1"));
		displayTagIf(m_fields->dynamicHeight, utils::string::contains(levelStringKeys, "kA37,0"));
		displayTagIf(m_fields->multiRotate, utils::string::contains(levelStringKeys, "kA27,0"));
		displayTagIf(m_fields->twoPointTwo, utils::string::contains(levelStringKeys, "kA40,0"));
		displayTagIf(m_fields->negativeScale, utils::string::contains(levelStringKeys, "kA33,0"));
	}
	static void displayTagIf(cocos2d::CCNode* node, const bool condition) {
		if (!node) return log::info("node is nullptr");
		node->setVisible(condition);
		if (node->getParent() && node->getParent()->getLayout()) node->getParent()->updateLayout();
	}
	static void makeInvis(cocos2d::CCNode* node) {
		if (!node) return log::info("node is nullptr");
		node->setVisible(false);
		if (node->getParent() && node->getParent()->getLayout()) node->getParent()->updateLayout();
	}
	void addChildToFieldsMenu(cocos2d::CCNode* node, const std::string& id) {
		if (!node || !m_fields->menu) return;
		node->setID(id);
		m_fields->menu->addChild(node);
	}
	static cocos2d::CCSprite* createTagSprite(const std::string& frameName, const CircleBaseColor color) {
		cocos2d::CCSprite* sprite = CCSprite::createWithSpriteFrameName(frameName.c_str());
		if (!utils::string::contains(frameName, Mod::get()->getID())) sprite->setScale(.5f);
		return CircleButtonSprite::create(sprite, color, CircleBaseSize::Large);
	}
	static long frequencyOfRegexPatternInString(const std::regex& pattern, const std::string& levelString) {
		return std::distance(std::sregex_iterator(levelString.begin(), levelString.end(), pattern), std::sregex_iterator());
	}
	static int useVectorToFindFrequency(const std::set<int>& vector, const std::string& levelString) {
		int result = 0;
		for (const int i : vector) if (utils::string::contains(levelString, fmt::format(";1,{},", i))) result += 1;
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

		if (!LevelInfoLayer::init(theLevel, p1)) return false;
		if (!getBool("enabled")) return true;

		if (theLevel->m_levelID.value() < 129) return true; // avoid false positives with robtop's levels
		if (theLevel->m_accountID.value() == 19293579 && !getBool("robsVault")) return true;

		auto creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) return true;

		m_fields->menu = CCMenu::create();
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setContentWidth(5000.f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0) - 5.f, 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setID("more-level-tags-menu"_spr);
		m_fields->menu->setVisible(false);
		addChild(m_fields->menu);

		m_fields->twoPlayer = createTagSprite("2p.png"_spr, CircleBaseColor::Pink);
		addChildToFieldsMenu(m_fields->twoPlayer, "two-player"_spr);

		m_fields->shaderIntolerance = createTagSprite("edit_eShaderBtn_001.png", CircleBaseColor::DarkPurple);
		addChildToFieldsMenu(m_fields->shaderIntolerance, "shader-intolerance"_spr);

		m_fields->cameraIntolerance = createTagSprite("cameraIntolerance.png"_spr, CircleBaseColor::DarkPurple);
		addChildToFieldsMenu(m_fields->cameraIntolerance, "camera-intolerance"_spr);

		m_fields->legacyShip = createTagSprite("portal_04_extra_2_001.png", CircleBaseColor::Cyan);
		addChildToFieldsMenu(m_fields->legacyShip, "legacy-ship"_spr);

		m_fields->legacyRobot = createTagSprite("portal_14_extra_2_001.png", CircleBaseColor::Pink);
		addChildToFieldsMenu(m_fields->legacyRobot, "legacy-robot"_spr);

		m_fields->startFlipped = createTagSprite("portal_02_extra_2_001.png", CircleBaseColor::Pink);
		addChildToFieldsMenu(m_fields->startFlipped, "flip-gravity"_spr);

		m_fields->dynamicHeight = createTagSprite("portal_19_extra_2_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(m_fields->dynamicHeight, "dynamic-height"_spr);

		m_fields->multiRotate = createTagSprite("edit_eRotateComBtn_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(m_fields->multiRotate, "multi-rotate"_spr);

		m_fields->twoPointTwo = createTagSprite("portal_18_extra_2_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(m_fields->twoPointTwo, "two-point-two"_spr);

		m_fields->negativeScale = createTagSprite("edit_eScaleComBtn_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(m_fields->negativeScale, "negative-scale"_spr);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::displayTags(theLevel);
		else {
			makeInvis(m_fields->twoPlayer);
			makeInvis(m_fields->shaderIntolerance);
			makeInvis(m_fields->cameraIntolerance);
			makeInvis(m_fields->legacyShip);
			makeInvis(m_fields->legacyRobot);
			makeInvis(m_fields->startFlipped);
			makeInvis(m_fields->dynamicHeight);
			makeInvis(m_fields->multiRotate);
			makeInvis(m_fields->twoPointTwo);
			makeInvis(m_fields->negativeScale);
		}

		geode::AxisLayout* layout = RowLayout::create()
			->setGap(36.f)
			->setAutoScale(true)
			->setDefaultScaleLimits(.0001f, 1.f)
			->setAxisAlignment(AxisAlignment::Start);
		layout->ignoreInvisibleChildren(true);
		m_fields->menu->setLayout(layout);
		m_fields->menu->setAnchorPoint({0.f, .5f});
		m_fields->menu->setContentWidth(5000.f);
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
				MyLevelInfoLayer::displayTags(theLevel);
			}
		} else {
			if (CCNode* node = m_fields->twoPlayer) node->setVisible(false);
			if (CCNode* node = m_fields->shaderIntolerance) node->setVisible(false);
			if (CCNode* node = m_fields->cameraIntolerance) node->setVisible(false);
			if (CCNode* node = m_fields->legacyShip) node->setVisible(false);
			if (CCNode* node = m_fields->legacyRobot) node->setVisible(false);
			if (CCNode* node = m_fields->startFlipped) node->setVisible(false);
			if (CCNode* node = m_fields->dynamicHeight) node->setVisible(false);
			if (CCNode* node = m_fields->multiRotate) node->setVisible(false);
			if (CCNode* node = m_fields->twoPointTwo) node->setVisible(false);
			if (CCNode* node = m_fields->negativeScale) node->setVisible(false);
		}

		geode::AxisLayout* layout = RowLayout::create()
			->setGap(36.f)
			->setAutoScale(true)
			->setDefaultScaleLimits(.0001f, 1.f)
			->setAxisAlignment(AxisAlignment::Start);
		layout->ignoreInvisibleChildren(true);
		m_fields->menu->setLayout(layout);
		m_fields->menu->setAnchorPoint({0.f, .5f});
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0) - 5.f, 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(true);

		m_fields->isRefreshing = false;
	}
};