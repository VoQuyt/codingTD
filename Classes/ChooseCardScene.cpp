#include"ChooseCardScene.h"
#include "json\document.h"
#include "json\rapidjson.h"
#include <network/SocketIO.h>
//#include "Object/SocketClient.h"
#include "Global Class/Tool.h"
USING_NS_CC;



Scene* ChooseCardScene::createScene()
{
	return ChooseCardScene::create();
}

bool ChooseCardScene::init()
{
	visibleSize = Director::getInstance()->getVisibleSize();

	SetupGUI();
	
	//_client = SocketIO::connect("http://127.0.0.1:3000", *this);
 	Tool::Socket_Client->_client->on("_Prohibit_Card_", CC_CALLBACK_2(ChooseCardScene::onReceiveEvent, this));

	return true;
}

void ChooseCardScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}  

void ChooseCardScene::onReceiveEvent(SIOClient* client, const std::string& data)
{
	rapidjson::Document document;
	document.Parse<0>(data.c_str());
	CCLOG("%s", document["cardName"].GetString());
	for (int i = 0; i < vecButton.size(); i++)
	{
		if (vecButton[i]->getName() == document["cardName"].GetString())
		{
			vecButton[i]->setEnabled(false);
		}
	}
}
void ChooseCardScene::btn_Click(Ref *pSender, cocos2d::ui::Button::Widget::TouchEventType type) {
	Button* term;
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
	{
		term = (Button*)pSender;
		//CCLOG("%s" , term->getName().c_str());
		//term->setEnabled(false);
		string card = term->getName();
		Tool::Socket_Client->_client->emit("_Prohibit_Card_", "{\"cardName\":\"" + card + "\"}");
	}
	default: break;
	}
}

void ChooseCardScene::SetupGUI()
{
	auto sp_SceneName = Sprite::create("UI/LoginScene/Login.png");
	sp_SceneName->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height*0.9));
	this->addChild(sp_SceneName);

	auto sp_Background = Sprite::create("UI/BG/background1.png");
	sp_Background->setPosition(visibleSize / 2);
	this->addChild(sp_Background, -1);

	auto frame1 = Sprite::create("UI/BG/frame1.png");
	frame1->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height*0.5));
	this->addChild(frame1);

	auto frame2 = Sprite::create("UI/BG/frame2.png");
	frame2->setPosition(Vec2(visibleSize.width * 0.15f, visibleSize.height*0.5));
	this->addChild(frame2);

	auto frame3 = Sprite::create("UI/BG/frame2.png");
	frame3->setPosition(Vec2(visibleSize.width * 0.85f, visibleSize.height*0.5));
	this->addChild(frame3);

	float x = 0.35f, y = 0.95f;
	for (int i = 0; i < 9; i++)
	{
		auto btn_Card = Button::create("Sprites/Card/Card"+to_string(i)+".png", "Sprites/Card/Card"+to_string(i)+".png");
		btn_Card->setName("btn_Card"+to_string(i));
		btn_Card->addTouchEventListener(CC_CALLBACK_2(ChooseCardScene::btn_Click, this));
		if (i % 3 == 0)
		{
			x = 0.35f;
			y -= 0.25f;
		}
		btn_Card->setPosition(Vec2(visibleSize.width*x, visibleSize.height*y));
		x += 0.15f;
		this->addChild(btn_Card);
		vecButton.push_back(btn_Card);
	}
	
}



void ChooseCardScene::RunActionNotify(string content)
{
	lbl_Notify->setString(content);
	lbl_Notify->runAction(Sequence::create(FadeIn::create(0.75), DelayTime::create(0.75), FadeOut::create(1), nullptr));
}
