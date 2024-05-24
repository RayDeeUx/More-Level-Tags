#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

class $modify(LevelInfoLayer) {
	struct Fields {
		CCMenu* menu = nullptr;
		CCNode* l = nullptr;
		CCNode* r = nullptr;
		CCNode* f = nullptr;
		CCNode* h = nullptr;
		CCNode* m = nullptr;
		CCNode* n = nullptr;
		CCNode* t = nullptr;
		std::vector<CCNode*> buttons = {};
		bool isRefreshing = false;
	};
	void hideTags(std::string decomp) {
		if (m_fields->buttons.size() < 1) return;
		if (decomp.size() > 1) {
			if ((m_fields->l != nullptr) && decomp.find("kA32,0")) {
				m_fields->l->setVisible(decomp.find("kA32,0") != std::string::npos);
				if (!m_fields->l->isVisible())
				{
					m_fields->l->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->l), m_fields->buttons.end());
				}
			}
			if ((m_fields->r != nullptr) && decomp.find("kA34,0")) {
				m_fields->r->setVisible(decomp.find("kA34,0") != std::string::npos);
				if (!m_fields->r->isVisible())
				{
					m_fields->r->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->r), m_fields->buttons.end());
				}
			}
			if ((m_fields->f != nullptr) && decomp.find("kA11,1")) {
				m_fields->f->setVisible(decomp.find("kA11,1") != std::string::npos);
				if (!m_fields->f->isVisible())
				{
					m_fields->f->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->f), m_fields->buttons.end());
				}
			}
			if ((m_fields->h != nullptr) && decomp.find("kA37,0")) {
				m_fields->h->setVisible(decomp.find("kA37,0") != std::string::npos);
				if (!m_fields->h->isVisible())
				{
					m_fields->h->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->h), m_fields->buttons.end());
				}
			}
			if ((m_fields->m != nullptr) && decomp.find("kA27,0")) {
				m_fields->m->setVisible(decomp.find("kA27,0") != std::string::npos);
				if (!m_fields->m->isVisible())
				{
					m_fields->m->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->m), m_fields->buttons.end());
				}
			}
			if ((m_fields->t != nullptr) && decomp.find("kA40,0")) {
				m_fields->t->setVisible(decomp.find("kA40,0") != std::string::npos);
				if (!m_fields->t->isVisible())
				{
					m_fields->t->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->t), m_fields->buttons.end());
				}
			}
			if ((m_fields->n != nullptr) && decomp.find("kA33,0")) {
				m_fields->n->setVisible(decomp.find("kA33,0") != std::string::npos);
				if (!m_fields->n->isVisible())
				{
					m_fields->n->removeFromParent();
					m_fields->buttons.erase(std::remove(m_fields->buttons.begin(), m_fields->buttons.end(), m_fields->n), m_fields->buttons.end());
				}
			}
		}
	}
	void onUpdate(cocos2d::CCObject* sender) {
		m_fields->isRefreshing = true;
		LevelInfoLayer::onUpdate(sender);
	}
	void onPlay(cocos2d::CCObject* sender) {
		m_fields->isRefreshing = true;
		LevelInfoLayer::onPlay(sender);
	}
	bool init(GJGameLevel* p0, bool p1)
	{
		m_fields->isRefreshing = false;
		m_fields->buttons.clear();

		if (!LevelInfoLayer::init(p0, p1)) { return false; }

		if (p0->m_accountID.value() == 71 && !getChildByIDRecursive("right-side-menu")->isVisible()) { return true; } // avoid false positives with robtop's levels

		auto creatorInfoMenu = getChildByID("creator-info-menu");
		if (!creatorInfoMenu) { return true; }

		m_fields->menu = CCMenu::create();
		m_fields->menu->setContentSize(ccp(0, 0));
		m_fields->menu->setID("more-level-tags-menu"_spr);
		m_fields->menu->setScale(0.15f);
		m_fields->menu->setPosition(as<CCMenu*>(creatorInfoMenu)->getPosition() + ccp(as<CCMenu*>(creatorInfoMenu)->getScaledContentSize().width / 2 + 7 + (getChildByID("copy-indicator") ? 18 : 0) + (getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		m_fields->menu->setVisible(false);
		addChild(m_fields->menu);

		auto ship = CCSprite::createWithSpriteFrameName("portal_04_extra_2_001.png");
		ship->setScale(3.5f);
		m_fields->l = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		m_fields->l->setID("legacy-ship"_spr);
		m_fields->buttons.push_back(m_fields->l);
		m_fields->menu->addChild(m_fields->l);

		auto robot = CCSprite::createWithSpriteFrameName("portal_14_extra_2_001.png");
		robot->setScale(3.5f);
		m_fields->r = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->r->setID("legacy-robot"_spr);
		m_fields->buttons.push_back(m_fields->r);
		m_fields->menu->addChild(m_fields->r);

		auto flip = CCSprite::createWithSpriteFrameName("portal_02_extra_2_001.png");
		flip->setScale(3.5f);
		m_fields->f = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		m_fields->f->setID("flip-gravity"_spr);
		m_fields->buttons.push_back(m_fields->f);
		m_fields->menu->addChild(m_fields->f);

		auto height = CCSprite::createWithSpriteFrameName("portal_19_extra_2_001.png");
		height->setScale(3.5f);
		m_fields->h = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->h->setID("dynamic-height"_spr);
		m_fields->buttons.push_back(m_fields->h);
		m_fields->menu->addChild(m_fields->h);

		auto rotate = CCSprite::createWithSpriteFrameName("edit_eRotateComBtn_001.png");
		rotate->setScale(1.5f);
		m_fields->m = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->m->setID("multi-rotate"_spr);
		m_fields->buttons.push_back(m_fields->m);
		m_fields->menu->addChild(m_fields->m);

		auto tpt = CCSprite::createWithSpriteFrameName("portal_18_extra_2_001.png");
		tpt->setScale(3.5f);
		m_fields->t = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->t->setID("two-point-two"_spr);
		m_fields->buttons.push_back(m_fields->t);
		m_fields->menu->addChild(m_fields->t);

		auto negScale = CCSprite::createWithSpriteFrameName("edit_eScaleComBtn_001.png");
		negScale->setScale(1.5f);
		m_fields->n = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		m_fields->n->setID("negative-scale"_spr);
		m_fields->buttons.push_back(m_fields->n);
		m_fields->menu->addChild(m_fields->n);

		if (p0->m_twoPlayerMode)
		{
			auto cube = CCSprite::createWithSpriteFrameName("portal_03_extra_2_001.png");
			cube->setScale(3.5f);
			auto p2 = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
			p2->setID("two-player"_spr);
			m_fields->menu->addChild(p2);
			m_fields->buttons.push_back(p2);
		}

		if (p0->m_levelString.size() > 1) {
			std::string decomp = ZipUtils::decompressString(p0->m_levelString.c_str(), true, 0);
			hideTags(decomp);
		} else {
			if (m_fields->l) {
				m_fields->buttons.push_back(m_fields->l);
				m_fields->l->setVisible(false);
			}
			if (m_fields->r) {
				m_fields->buttons.push_back(m_fields->r);
				m_fields->r->setVisible(false);
			}
			if (m_fields->f) {
				m_fields->buttons.push_back(m_fields->f);
				m_fields->r->setVisible(false);
			}
			if (m_fields->h) {
				m_fields->buttons.push_back(m_fields->h);
				m_fields->r->setVisible(false);
			}
			if (m_fields->m) {
				m_fields->buttons.push_back(m_fields->m);
				m_fields->r->setVisible(false);
			}
			if (m_fields->t) {
				m_fields->buttons.push_back(m_fields->t);
				m_fields->r->setVisible(false);
			}
		}

		for (size_t i = 0; i < m_fields->buttons.size(); i++) {
			m_fields->buttons[i]->setPositionX((18 / 0.15f) * i);
		}

		m_fields->menu->setVisible(true);

		return true;
	}

	virtual void levelDownloadFinished(GJGameLevel* p0)
	{
		LevelInfoLayer::levelDownloadFinished(p0);

		if (m_fields->menu) {
			if (p0->m_levelString.size() > 1) {
				if (!m_fields->isRefreshing) {
					m_fields->menu->setVisible(false);
					std::string decomp = ZipUtils::decompressString(p0->m_levelString.c_str(), true, 0);
					hideTags(decomp);
				}
			} else {
				if (m_fields->l) { m_fields->l->setVisible(false); }
				if (m_fields->r) { m_fields->r->setVisible(false); }
				if (m_fields->f) { m_fields->f->setVisible(false); }
				if (m_fields->h) { m_fields->h->setVisible(false); }
				if (m_fields->m) { m_fields->m->setVisible(false); }
				if (m_fields->t) { m_fields->t->setVisible(false); }
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
		isRefreshing = false;
	}
};