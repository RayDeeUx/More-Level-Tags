#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

CCMenu* menu;
LoadingCircle* loading;
CCNode* l;
CCNode* r;
CCNode* f;
CCNode* h;
CCNode* m;
CCNode* n;
CCNode* t;
std::vector<CCNode*> buttons;
bool isRefreshing = false;

/*
	consent to continue TheSillyDoggo's More Level Tags mod is below:
	https://discord.com/channels/911701438269386882/911702535373475870/1228540436277366794
*/

void hideTags(std::string decomp) {
	if (buttons.size() < 1) return; 
	if (decomp.size() > 1) {
		if ((l != nullptr) && decomp.find("kA32,0")) {
			l->setVisible(decomp.find("kA32,0") != std::string::npos);
			if (!l->isVisible())
			{
				l->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), l), buttons.end());
			}
		}
		if ((r != nullptr) && decomp.find("kA34,0")) {
			r->setVisible(decomp.find("kA34,0") != std::string::npos);
			if (!r->isVisible())
			{
				r->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), r), buttons.end());
			}
		}
		if ((f != nullptr) && decomp.find("kA11,1")) {
			f->setVisible(decomp.find("kA11,1") != std::string::npos);
			if (!f->isVisible())
			{
				f->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), f), buttons.end());
			}
		}
		if ((h != nullptr) && decomp.find("kA37,0")) {
			h->setVisible(decomp.find("kA37,0") != std::string::npos);
			if (!h->isVisible())
			{
				h->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), h), buttons.end());
			}
		}
		if ((m != nullptr) && decomp.find("kA27,0")) {
			m->setVisible(decomp.find("kA27,0") != std::string::npos);
			if (!m->isVisible())
			{
				m->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), m), buttons.end());
			}
		}
		if ((t != nullptr) && decomp.find("kA40,0")) {
			t->setVisible(decomp.find("kA40,0") != std::string::npos);
			if (!t->isVisible())
			{
				t->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), t), buttons.end());
			}
		}
		if ((n != nullptr) && decomp.find("kA33,0")) {
			n->setVisible(decomp.find("kA33,0") != std::string::npos);
			if (!n->isVisible())
			{
				n->removeFromParent();
				buttons.erase(std::remove(buttons.begin(), buttons.end(), n), buttons.end());
			}
		}
	}
}

class $modify(LevelInfoLayer)
{
	void onUpdate(cocos2d::CCObject* sender) {
		isRefreshing = true;
		LevelInfoLayer::onUpdate(sender);
	}
	bool init(GJGameLevel* p0, bool p1)
	{
		isRefreshing = false;
		buttons.clear();

		if (!LevelInfoLayer::init(p0, p1)) { return false; }

		if (!getChildByIDRecursive("info-button") || !getChildByIDRecursive("refresh-button")) { return true; } // avoid false positives with robtop's levels

		menu = CCMenu::create();
		menu->setContentSize(ccp(0, 0));
		menu->setID("more-level-tags-menu"_spr);
		menu->setScale(0.15f);
		menu->setPosition(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getPosition() + ccp(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getScaledContentSize().width / 2 + 7 + (this->getChildByID("copy-indicator") ? 18 : 0) + (this->getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));

		loading = LoadingCircle::create();
		loading->ignoreAnchorPointForPosition(false);
		loading->m_fade = false;
		loading->show();
		menu->addChild(loading);

		auto ship = CCSprite::createWithSpriteFrameName("portal_04_extra_2_001.png");
		ship->setScale(3.5f);
		l = CircleButtonSprite::create(ship, CircleBaseColor::Cyan, CircleBaseSize::Large);
		l->setID("legacy-ship"_spr);
		buttons.push_back(l);
		menu->addChild(l);

		auto robot = CCSprite::createWithSpriteFrameName("portal_14_extra_2_001.png");
		robot->setScale(3.5f);
		r = CircleButtonSprite::create(robot, CircleBaseColor::Pink, CircleBaseSize::Large);
		r->setID("legacy-robot"_spr);
		buttons.push_back(r);
		menu->addChild(r);

		auto flip = CCSprite::createWithSpriteFrameName("portal_02_extra_2_001.png");
		flip->setScale(3.5f);
		f = CircleButtonSprite::create(flip, CircleBaseColor::Pink, CircleBaseSize::Large);
		f->setID("flip-gravity"_spr);
		buttons.push_back(f);
		menu->addChild(f);

		auto height = CCSprite::createWithSpriteFrameName("portal_19_extra_2_001.png");
		height->setScale(3.5f);
		h = CircleButtonSprite::create(height, CircleBaseColor::Gray, CircleBaseSize::Large);
		h->setID("dynamic-height"_spr);
		buttons.push_back(h);
		menu->addChild(h);

		auto rotate = CCSprite::createWithSpriteFrameName("edit_eRotateComBtn_001.png");
		rotate->setScale(1.5f);
		m = CircleButtonSprite::create(rotate, CircleBaseColor::Gray, CircleBaseSize::Large);
		m->setID("multi-rotate"_spr);
		buttons.push_back(m);
		menu->addChild(m);

		auto tpt = CCSprite::createWithSpriteFrameName("portal_18_extra_2_001.png");
		tpt->setScale(3.5f);
		t = CircleButtonSprite::create(tpt, CircleBaseColor::Gray, CircleBaseSize::Large);
		t->setID("two-point-two"_spr);
		buttons.push_back(t);
		menu->addChild(t);

		auto negScale = CCSprite::createWithSpriteFrameName("edit_eScaleComBtn_001.png");
		negScale->setScale(1.5f);
		n = CircleButtonSprite::create(negScale, CircleBaseColor::Gray, CircleBaseSize::Large);
		n->setID("negative-scale"_spr);
		buttons.push_back(n);
		menu->addChild(n);

		if (p0->m_twoPlayerMode)
		{
			auto cube = CCSprite::createWithSpriteFrameName("portal_03_extra_2_001.png");
			cube->setScale(3.5f);
			auto p2 = CircleButtonSprite::create(cube, CircleBaseColor::Pink, CircleBaseSize::Large);
			p2->setID("two-player"_spr);
			menu->addChild(p2);
			buttons.push_back(p2);
		}

		this->addChild(menu);

		if (p0->m_levelString.size() > 1)
		{

			if (loading) { loading->removeFromParent(); }
			std::string decomp = ZipUtils::decompressString(p0->m_levelString.c_str(), true, 0);
			hideTags(decomp);
		} else {
			if (l) {
				buttons.push_back(l);
				l->setVisible(false);
			}
			if (r) {
				buttons.push_back(r);
				r->setVisible(false);
			}
			if (f) {
				buttons.push_back(f);
				r->setVisible(false);
			}
			if (h) {
				buttons.push_back(h);
				r->setVisible(false);
			}
			if (m) {
				buttons.push_back(m);
				r->setVisible(false);
			}
			if (t) {
				buttons.push_back(t);
				r->setVisible(false);
			}
			if (loading) { loading->setVisible(true); }
		}

		for (size_t i = 0; i < buttons.size(); i++)
		{
			buttons[i]->setPositionX((18 / 0.15f) * i);
		}
		

		return true;
	}

	virtual void levelDownloadFinished(GJGameLevel* p0)
	{
		LevelInfoLayer::levelDownloadFinished(p0);

		if (menu)
		{
			if (p0->m_levelString.size() > 1)
			{
				if (loading) { loading->setVisible(false); }
				if (!isRefreshing) {
					std::string decomp = ZipUtils::decompressString(p0->m_levelString.c_str(), true, 0);
					hideTags(decomp);
				}
			}
			else
			{
				if (l) { l->setVisible(false); }
				if (r) { r->setVisible(false); }
				if (f) { f->setVisible(false); }
				if (h) { h->setVisible(false); }
				if (m) { m->setVisible(false); }
				if (t) { t->setVisible(false); }
				if (loading) { loading->setVisible(true); }
			}

			for (size_t i = 0; i < buttons.size(); i++)
			{
				buttons[i]->setPositionX((18 / 0.15f) * i);
			}
			
			menu->setScale(0.15f);
			menu->setPosition(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getPosition() + ccp(as<CCMenu*>(this->getChildByID("creator-info-menu"))->getScaledContentSize().width / 2 + 7 + (this->getChildByID("copy-indicator") ? 18 : 0) + (this->getChildByID("high-object-indicator") ? 18 : 0), 7 + 1.6f) + ccp(5, 0));
		}
		isRefreshing = false;
	}
};