#pragma once
#include "cocos2d.h"
#include"BaseUnitClass.h"

USING_NS_CC;
using namespace std;

class DeadWalker_1 : public BaseUnitClass {
public:
	DeadWalker_1(int line, bool isOwned, int unitId, int playerId);
	~DeadWalker_1();

	//Rotten Aura: Decrease enemies Attack and Defense by 5 / 7.5 / 10% in 5 seconds in 500 range around.
	void Regeneration() override;
	
	
};