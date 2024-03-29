﻿#include "FrozenKingdom.h"

FrozenKingdom::FrozenKingdom(int line, bool isOwned, int unitId, int playerId)
{
	name = "Frozen Kingdom";
	description = "Kingdom";
	maxHealth = 1000;
	currentHealth = maxHealth;
	baseAttack = 80;
	baseDefense = 100;
	baseAttackSpeed = 30;
	range = 400;
	baseRegeneration = 1;

	upgradeName = "Frozen Kingdom 2";
	upgradeGoldCost = 200;
	upgradeKnowledgeCost = 3;

	this->UpdateIngameInfo("Sprites/Frozen Kingdom/default.png", unitId, playerId, isOwned, "Frozen Kingdom", line);
	if (this->isOwned) this->root->setPosition(Vec2(50, 50));
	else this->root->setPosition(Vec2(3050,50));
}

FrozenKingdom::~FrozenKingdom()
{
}

void FrozenKingdom::Die()
{
	this->StotpAction("All");
	this->isAlive = false;
	this->root->runAction(RemoveSelf::create());
	this->action = "Idle";
}


void FrozenKingdom::Update()
{
	//Đẩy đồng đội nếu đi ra khỏi map
	for (auto unit : BaseUnitClass::AllIngameUnit_Vector) {
		if (unit->isOwned == this->isOwned
			&& unit->isAlive && unit->action != "Die") {
			if (unit->root->getPositionX() < 0)
				unit->root->runAction(MoveTo::create(0.2, Vec2(50, unit->root->getPositionY())));
			if (unit->root->getPositionX() > 3000)
				unit->root->runAction(MoveTo::create(0.2, Vec2(2950, unit->root->getPositionY())));
		}
	}


	//Nếu chết rồi thì hủy đối tượng
	if (!this->isAlive) {
		//Xử lý victory...
		delete this;
		return;
	}

	//Kiểm tra trigger damage
	for (int i = 0; i < damageReceive.size(); i++) {
		if (damageReceive[i].triggerTime <= Tool::currentIngameTime) {
			this->onDamageReceive(damageReceive[i]);
			damageReceive.erase(damageReceive.begin() + i);
		}
	}

	//Kiểm tra release status
	for (int i = 0; i < statusReceive.size(); i++) {
		if (statusReceive[i].releaseStatusTime <= Tool::currentIngameTime) {
			string influence = statusReceive[i].statusInfluence;
			statusReceive.erase(statusReceive.begin() + i);
			this->ReprocessAllStatus(influence);
		}
	}

	//Regeneration
	if (this->nextRegenerationTriggerTime < Tool::currentIngameTime) {
		this->Regeneration();
		this->nextRegenerationTriggerTime++;
	}
	
	//Action Die
	if (this->currentHealth < 1 && this->action != "Die") {
		this->Die();
		return;
	}

	//Action Attack
	if (this->action == "Idle") {
		vector<BaseUnitClass*> targetList = this->FindTargets();
		if (targetList.size() > 0) {
			this->Attack(targetList);
			return;
		}

	}

}

void FrozenKingdom::Attack(vector<BaseUnitClass*>& targets)
{
	this->action = "Attack";
	this->sprite->runAction(Sequence::create(
		DelayTime::create(60.0 / this->attackSpeed),
		CallFunc::create([&]() {	this->action = "Idle"; }),
		nullptr
	))->setFlags(1);
	for (auto target : targets) {
		float damage = this->attack;
		//string animateName = this->animateName + "_explosion";
		string animateName = "_explosion"; // Không có sprite nên ai bắn cũng nổ như nhau
		float distance = abs(this->root->getPositionX() - target->root->getPositionX());
		float triggerTime = Tool::currentIngameTime + distance * 0.002;
		target->damageReceive.push_back(DamageReceive(this->unitId, damage, triggerTime, animateName, ""));
	}
}

//Special: Frozen Aura: Whoever entered this tower range without permission will be cold, decrease Attack Speed by 10 / 15 / 20%.
vector<BaseUnitClass*> FrozenKingdom::FindTargets()
{
	vector<BaseUnitClass*> result;
	vector<BaseUnitClass*> shotableTargets;
	//Tìm danh sách địch có thể bắn tới
	for (auto target : BaseUnitClass::AllIngameUnit_Vector) {
		if (
			(this->isOwned != target->isOwned)
			&& (target->isAlive && target->action != "Die")
			&& (abs(target->root->getPositionX() - this->root->getPositionX()) < this->range)
			)
			shotableTargets.push_back(target);
	}
	//Sắp xếp theo máu tăng dần
	std::sort(shotableTargets.begin(), shotableTargets.end(), BaseUnitClass::SortByHealth);

	for (auto target : shotableTargets) {
		bool isAffected = false;
		for (int i = 0; i < target->statusReceive.size(); i++) {
			if (target->statusReceive[i].statusName == "Frozen Aura") {
				target->statusReceive[i].releaseStatusTime = Tool::currentIngameTime + 5;
				isAffected = true;
				break;
			}
		}
		float slowAttackSpeed = 0.9;
		if (level == 2) slowAttackSpeed = 0.85;
		if (level == 3) slowAttackSpeed = 0.8;
		if (!isAffected) {
			target->ApplyStatus(StatusReceive("Frozen Aura", "AttackSpeed", slowAttackSpeed, Tool::currentIngameTime + 5, 2));
		}
	}

	//Lấy 1 thằng máu thấp nhất
	if (shotableTargets.size() > 0) result.push_back(shotableTargets[0]);
	return result;
}



void FrozenKingdom::Upgrade()
{
	level++;
	if (level == 2) {
		float currentHealthPercent = (float)currentHealth / maxHealth;
		name = "Frozen Kingdom 2";
		description = "Kingdom";
		maxHealth = 1750;
		currentHealth = maxHealth * currentHealthPercent;
		baseAttack = 96;
		baseDefense = 100;
		baseAttackSpeed = 30;
		range = 500;
		baseRegeneration = 2;

		upgradeName = "Frozen Kingdom 3";
		upgradeGoldCost = 500;
		upgradeKnowledgeCost = 5;
		this->root->setScale(1);
		this->ReprocessAllStatus("Attack");
		this->ReprocessAllStatus("Defense");
		this->ReprocessAllStatus("MoveSpeed");
		this->ReprocessAllStatus("AttackSpeed");
		this->ReprocessAllStatus("Regeneration");
	}
	else if (level == 3) {
		float currentHealthPercent = (float)currentHealth / maxHealth;
		name = "Frozen Kingdom 3";
		description = "Kingdom";
		maxHealth = 1750;
		currentHealth = maxHealth * currentHealthPercent;
		baseAttack = 120;
		baseDefense = 150;
		baseAttackSpeed = 30;
		range = 575;
		baseRegeneration = 3;

		upgradeName = "";
		this->root->setScale(1.1);
		this->ReprocessAllStatus("Attack");
		this->ReprocessAllStatus("Defense");
		this->ReprocessAllStatus("MoveSpeed");
		this->ReprocessAllStatus("AttackSpeed");
		this->ReprocessAllStatus("Regeneration");
	}
}
