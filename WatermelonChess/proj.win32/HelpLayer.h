#ifndef __HELP_LAYER_H__
#define __HELP_LAYER_H__


#include "cocos2d.h"
USING_NS_CC;

class HelpLayer : public CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(HelpLayer);

	void quit(CCObject* pSender);
};

#endif