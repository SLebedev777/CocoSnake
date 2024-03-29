#pragma once

#include <functional>
#include "cocos/2d/CCNode.h"

namespace MyCocos2dHelpers
{

	void CallCCNodeMethodRecursively(cocos2d::Node* node, std::function<void(cocos2d::Node*)> func)
	{
		if (!node)
			return;

		func(node);

		for (auto obj : node->getChildren())
		{
			CallCCNodeMethodRecursively(obj, func);
		}
	}

}