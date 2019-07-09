#pragma once
#include "cocos2d.h"
#include"BaseUnitClass.h"

USING_NS_CC;
using namespace std;

class UFODriver_2 : public BaseUnitClass {
public:
	UFODriver_2(int line, bool isOwned, int unitId, int playerId);
	~UFODriver_2();


	void Attack(vector<BaseUnitClass*>& targets) override;
	int numOfAttack = 0;
	
};