﻿#include "HeavenBless.h"

HeavenBless::HeavenBless(int playerId)
{
	this->playerId = playerId;
	this->name = "Heaven Bless";
	this->description = "encourage all allies with a bless from heaven, increase current Health and max Health by 250.";
	for (auto ally : BaseUnitClass::AllIngameUnit_Vector) {
		if (ally->ownerPlayerId == this->playerId
			&& ally->isAlive && ally->action != "Die" && ally->description != "Kingdom"
			) {
			ally->maxHealth += 250;
			ally->currentHealth += 250;
			auto sp = Sprite::create("Sprites/Heaven Bless/heal/heal (1).png");
			ally->root->addChild(sp,1);
			sp->setPosition(Vec2(ally->sprite->getBoundingBox().size.width / 2 * ally->isOwned,0));
			sp->runAction(Sequence::create(IngameObject::animate["Heaven Bless_heal"]->clone(), RemoveSelf::create(), nullptr));
		}
	}

}

HeavenBless::~HeavenBless()
{
}



