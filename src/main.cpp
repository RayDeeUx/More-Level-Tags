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

		CCNode* creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) return true;

		const float copyIndicatorOffset = getChildByID("copy-indicator") ? 18.f : 0.f;
		const float highObjIndictorOffset = getChildByID("high-object-indicator") ? 18.f : 0.f;
		const float creatorInfoMenuHalfWidth = creatorInfoMenu->getScaledContentSize().width / 2.f;

		Fields* fields = m_fields.self();

		fields->menu = CCMenu::create();
		fields->menu->setScale(0.15f);
		fields->menu->setContentWidth(5000.f);
		fields->menu->setAnchorPoint({0.f, .5f});
		fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenuHalfWidth + 7.f + copyIndicatorOffset + highObjIndictorOffset, 8.6f));
		fields->menu->setID("more-level-tags-menu"_spr);
		fields->menu->setVisible(false);
		addChild(fields->menu);

		fields->twoPlayer = createTagSprite("2p.png"_spr, CircleBaseColor::Pink);
		addChildToFieldsMenu(fields->twoPlayer, "two-player"_spr);

		fields->shaderIntolerance = createTagSprite("edit_eShaderBtn_001.png", CircleBaseColor::DarkPurple);
		addChildToFieldsMenu(fields->shaderIntolerance, "shader-intolerance"_spr);

		fields->cameraIntolerance = createTagSprite("cameraIntolerance.png"_spr, CircleBaseColor::DarkPurple);
		addChildToFieldsMenu(fields->cameraIntolerance, "camera-intolerance"_spr);

		fields->legacyShip = createTagSprite("portal_04_extra_2_001.png", CircleBaseColor::Cyan);
		addChildToFieldsMenu(fields->legacyShip, "legacy-ship"_spr);

		fields->legacyRobot = createTagSprite("portal_14_extra_2_001.png", CircleBaseColor::Pink);
		addChildToFieldsMenu(fields->legacyRobot, "legacy-robot"_spr);

		fields->startFlipped = createTagSprite("portal_02_extra_2_001.png", CircleBaseColor::Pink);
		addChildToFieldsMenu(fields->startFlipped, "flip-gravity"_spr);

		fields->dynamicHeight = createTagSprite("portal_19_extra_2_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(fields->dynamicHeight, "dynamic-height"_spr);

		fields->multiRotate = createTagSprite("edit_eRotateComBtn_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(fields->multiRotate, "multi-rotate"_spr);

		fields->twoPointTwo = createTagSprite("portal_18_extra_2_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(fields->twoPointTwo, "two-point-two"_spr);

		fields->negativeScale = createTagSprite("edit_eScaleComBtn_001.png", CircleBaseColor::Gray);
		addChildToFieldsMenu(fields->negativeScale, "negative-scale"_spr);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::displayTags(theLevel);
		else {
			makeInvis(fields->twoPlayer);
			makeInvis(fields->shaderIntolerance);
			makeInvis(fields->cameraIntolerance);
			makeInvis(fields->legacyShip);
			makeInvis(fields->legacyRobot);
			makeInvis(fields->startFlipped);
			makeInvis(fields->dynamicHeight);
			makeInvis(fields->multiRotate);
			makeInvis(fields->twoPointTwo);
			makeInvis(fields->negativeScale);
		}

		geode::AxisLayout* layout = RowLayout::create()
			->setGap(36.f)
			->setAutoScale(true)
			->setDefaultScaleLimits(.0001f, 1.f)
			->setAxisAlignment(AxisAlignment::Start);
		layout->ignoreInvisibleChildren(true);
		fields->menu->setLayout(layout);
		fields->menu->setAnchorPoint({0.f, .5f});
		fields->menu->setContentWidth(5000.f);
		fields->menu->setVisible(true);

		return true;
	}

	virtual void levelDownloadFinished(GJGameLevel* theLevel) {
		LevelInfoLayer::levelDownloadFinished(theLevel);

		const auto creatorInfoMenu = getChildByID("creator-info-menu");
		Fields* fields = m_fields.self();

		if (!fields->menu || !creatorInfoMenu) {
			fields->isRefreshing = false;
			return;
		}

		if (!theLevel->m_levelString.empty()) {
			if (!fields->isRefreshing) {
				fields->menu->setVisible(false);
				MyLevelInfoLayer::displayTags(theLevel);
			}
		} else {
			if (CCNode* node = fields->twoPlayer) node->setVisible(false);
			if (CCNode* node = fields->shaderIntolerance) node->setVisible(false);
			if (CCNode* node = fields->cameraIntolerance) node->setVisible(false);
			if (CCNode* node = fields->legacyShip) node->setVisible(false);
			if (CCNode* node = fields->legacyRobot) node->setVisible(false);
			if (CCNode* node = fields->startFlipped) node->setVisible(false);
			if (CCNode* node = fields->dynamicHeight) node->setVisible(false);
			if (CCNode* node = fields->multiRotate) node->setVisible(false);
			if (CCNode* node = fields->twoPointTwo) node->setVisible(false);
			if (CCNode* node = fields->negativeScale) node->setVisible(false);
		}

		const float copyIndicatorOffset = getChildByID("copy-indicator") ? 18.f : 0.f;
		const float highObjIndictorOffset = getChildByID("high-object-indicator") ? 18.f : 0.f;
		const float creatorInfoMenuHalfWidth = creatorInfoMenu->getScaledContentSize().width / 2.f;
		geode::AxisLayout* layout = RowLayout::create()
			->setGap(36.f)
			->setAutoScale(true)
			->setDefaultScaleLimits(.0001f, 1.f)
			->setAxisAlignment(AxisAlignment::Start);
		layout->ignoreInvisibleChildren(true);
		fields->menu->setScale(0.15f);
		fields->menu->setLayout(layout);
		fields->menu->setAnchorPoint({0.f, .5f});
		fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenuHalfWidth + 7.f + copyIndicatorOffset + highObjIndictorOffset, 8.6f));
		fields->menu->setVisible(true);

		fields->isRefreshing = false;
	}
};