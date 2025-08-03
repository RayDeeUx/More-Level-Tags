#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <regex>

#define getInt Mod::get()->getSettingValue<int64_t>
#define getBool Mod::get()->getSettingValue<bool>

static const std::regex shaderIntoleranceRegex(R"(;1,(?:2904|2905|2907|2909|2910|2911|2912|2913|2914|2915|2916|2917|2919|2920|2921|2922|2923|2924),)");
static const std::regex cameraIntoleranceRegex(R"(;1,(?:1913|1914|1916|2015|2016|2062|2901|2925),)");

static const std::vector<int> shaderIntoleranceVector = {2904, 2905, 2907, 2909, 2910, 2911, 2912, 2913, 2914, 2915, 2916, 2917, 2919, 2920, 2921, 2922, 2923, 2924};
static const std::vector<int> cameraIntoleranceVector = {1913, 1914, 1916, 2015, 2016, 2062, 2901, 2925};

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
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void displayTags(const GJGameLevel* theLevel) {
		const std::string& fullLevelString = ZipUtils::decompressString(theLevel->m_levelString, true, 0);
		if (fullLevelString.empty() || m_fields->buttons.empty()) return;
		const size_t locationOfFirstSemicolon = fullLevelString.find_first_of(';');
		const size_t locationOfBackgroundKey = fullLevelString.find_first_of("kA6,");
		if (locationOfFirstSemicolon == std::string::npos || locationOfBackgroundKey == std::string::npos) return;
		const std::string& levelStringKeys = fullLevelString.substr(locationOfBackgroundKey, locationOfFirstSemicolon);
		if (levelStringKeys.empty()) return;
		log::info("levelStringKeys: {}", levelStringKeys);
		displayTagIf(m_fields->twoPlayer, theLevel->m_twoPlayerMode);
		if (getBool("advancedShaderAbuseTolerance"))
			displayTagIf(m_fields->shaderIntolerance, getInt("shaderAbuseTolerance") > -1 && frequencyOfRegexPatternInString(shaderIntoleranceRegex, fullLevelString) > getInt("shaderAbuseTolerance"));
		else displayTagIf(m_fields->shaderIntolerance, getInt("shaderAbuseTolerance") > -1 && useVectorToFindFrequency(shaderIntoleranceVector, fullLevelString) > getInt("shaderAbuseTolerance"));
		if (getBool("advancedCameraAbuseTolerance"))
			displayTagIf(m_fields->cameraIntolerance, getInt("cameraAbuseTolerance") > -1 && frequencyOfRegexPatternInString(cameraIntoleranceRegex, fullLevelString) > getInt("cameraAbuseTolerance"));
		else displayTagIf(m_fields->cameraIntolerance, getInt("cameraAbuseTolerance") > -1 && useVectorToFindFrequency(cameraIntoleranceVector, fullLevelString) > getInt("cameraAbuseTolerance"));
		displayTagIf(m_fields->legacyShip, utils::string::contains(fullLevelString, "kA32,0"));
		displayTagIf(m_fields->legacyRobot, utils::string::contains(fullLevelString, "kA34,0"));
		displayTagIf(m_fields->startFlipped, utils::string::contains(fullLevelString, "kA11,1"));
		displayTagIf(m_fields->dynamicHeight, utils::string::contains(fullLevelString, "kA37,0"));
		displayTagIf(m_fields->multiRotate, utils::string::contains(fullLevelString, "kA27,0"));
		displayTagIf(m_fields->twoPointTwo, utils::string::contains(fullLevelString, "kA40,0"));
		displayTagIf(m_fields->negativeScale, utils::string::contains(fullLevelString, "kA33,0"));
	}
	void displayTagIf(cocos2d::CCNode* node, const bool condition) {
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
	static cocos2d::CCSprite* createTagSprite(std::string&& frameName, const CircleBaseColor color) {
		cocos2d::CCSprite* sprite = CCSprite::createWithSpriteFrameName(frameName.c_str());
		if (!utils::string::contains(frameName, Mod::get()->getID())) sprite->setScale(.5f);
		return CircleButtonSprite::create(sprite, color, CircleBaseSize::Large);
	}
	static long frequencyOfRegexPatternInString(const std::regex& pattern, const std::string& levelString) {
		return std::distance(std::sregex_iterator(levelString.begin(), levelString.end(), pattern), std::sregex_iterator());
	}
	static int useVectorToFindFrequency(const std::vector<int>& vector, const std::string& levelString) {
		int result = 0;
		for (int i : vector) if (utils::string::contains(levelString, fmt::format(";1,{},", i))) result += 1;
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
		if (!getBool("enabled")) return true;

		if (theLevel->m_levelID.value() < 129) return true; // avoid false positives with robtop's levels
		if (theLevel->m_accountID.value() == 19293579 && !getBool("robsVault")) return true;

		auto creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) return true;

		m_fields->menu = CCMenu::create();
		m_fields->menu->setContentSize(ccp(0, 0));
		m_fields->menu->setID("more-level-tags-menu"_spr);
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(creatorInfoMenu->getPosition() + ccp(creatorInfoMenu->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(false);
		addChild(m_fields->menu);

		m_fields->twoPlayer = createTagSprite("2p.png"_spr, CircleBaseColor::Pink);
		pushBackAddChild(m_fields->twoPlayer, "two-player"_spr);

		m_fields->shaderIntolerance = createTagSprite("edit_eShaderBtn_001.png", CircleBaseColor::DarkPurple);
		pushBackAddChild(m_fields->shaderIntolerance, "shader-intolerance"_spr);

		m_fields->cameraIntolerance = createTagSprite("cameraIntolerance.png"_spr, CircleBaseColor::DarkPurple);
		pushBackAddChild(m_fields->cameraIntolerance, "camera-intolerance"_spr);

		m_fields->legacyShip = createTagSprite("portal_04_extra_2_001.png", CircleBaseColor::Cyan);
		pushBackAddChild(m_fields->legacyShip, "legacy-ship"_spr);

		m_fields->legacyRobot = createTagSprite("portal_14_extra_2_001.png", CircleBaseColor::Pink);
		pushBackAddChild(m_fields->legacyRobot, "legacy-robot"_spr);

		m_fields->startFlipped = createTagSprite("portal_02_extra_2_001.png", CircleBaseColor::Pink);
		pushBackAddChild(m_fields->startFlipped, "flip-gravity"_spr);

		m_fields->dynamicHeight = createTagSprite("portal_19_extra_2_001.png", CircleBaseColor::Gray);
		pushBackAddChild(m_fields->dynamicHeight, "dynamic-height"_spr);

		m_fields->multiRotate = createTagSprite("edit_eRotateComBtn_001.png", CircleBaseColor::Gray);
		pushBackAddChild(m_fields->multiRotate, "multi-rotate"_spr);

		m_fields->twoPointTwo = createTagSprite("portal_18_extra_2_001.png", CircleBaseColor::Gray);
		pushBackAddChild(m_fields->twoPointTwo, "two-point-two"_spr);

		m_fields->negativeScale = createTagSprite("edit_eScaleComBtn_001.png", CircleBaseColor::Gray);
		pushBackAddChild(m_fields->negativeScale, "negative-scale"_spr);

		if (!theLevel->m_levelString.empty()) MyLevelInfoLayer::displayTags(theLevel);
		else {
			pushBackMakeInvis(m_fields->twoPlayer);
			pushBackMakeInvis(m_fields->shaderIntolerance);
			pushBackMakeInvis(m_fields->cameraIntolerance);
			pushBackMakeInvis(m_fields->legacyShip);
			pushBackMakeInvis(m_fields->legacyRobot);
			pushBackMakeInvis(m_fields->startFlipped);
			pushBackMakeInvis(m_fields->dynamicHeight);
			pushBackMakeInvis(m_fields->multiRotate);
			pushBackMakeInvis(m_fields->twoPointTwo);
			pushBackMakeInvis(m_fields->negativeScale);
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
				MyLevelInfoLayer::displayTags(theLevel);
			}
		} else {
			if (auto node = m_fields->twoPlayer) node->setVisible(false);
			if (auto node = m_fields->shaderIntolerance) node->setVisible(false);
			if (auto node = m_fields->cameraIntolerance) node->setVisible(false);
			if (auto node = m_fields->legacyShip) node->setVisible(false);
			if (auto node = m_fields->legacyRobot) node->setVisible(false);
			if (auto node = m_fields->startFlipped) node->setVisible(false);
			if (auto node = m_fields->dynamicHeight) node->setVisible(false);
			if (auto node = m_fields->multiRotate) node->setVisible(false);
			if (auto node = m_fields->twoPointTwo) node->setVisible(false);
			if (auto node = m_fields->negativeScale) node->setVisible(false);
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

class $modify(MyFLAlertLayer, FLAlertLayer) {
	static void onModify(auto & self) {
		(void) self.setHookPriority("FLAlertLayer::init", -2133456789);
	}
	bool init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
		const std::string& titleAsString = title;
		const std::string& descAsString = desc;
		if (!utils::string::contains(titleAsString, " Trigger Intolerance") && !utils::string::contains(titleAsString, "Camera ") && !utils::string::contains(titleAsString, "Shader ") && !utils::string::startsWith(descAsString, "When determining if a level should display the ") && !utils::string::contains(descAsString, "EverAfter by Galofuf") && !utils::string::contains(descAsString, "HIGHLY DISCOURAGED") && !utils::string::contains(descAsString, "and should be reserved for the likes of NASA supercomputers."))
			return FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
		return FLAlertLayer::init(delegate, title, desc, btn1, btn2, 420.f, true, 320.f, 1.0f);
	}
};