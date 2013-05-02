//
//  AnimatedSprite.h
//  ActionPlataformer
//
//  Created by Lucas Bittencourt on 4/4/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __ActionPlataformer__AnimatedSprite__
#define __ActionPlataformer__AnimatedSprite__

#include "cocos2d.h"

class AnimatedSprite : public cocos2d::CCSprite
{
	public:
		AnimatedSprite(std::string frame_name);
		static cocos2d::CCAnimation* animationWithFile(const std::string& name, int frame_count, float delay);
		void addAnimation(cocos2d::CCAnimation* animation, const std::string& name);
		void runAnimation(const std::string& name, bool loop = false, bool start_again = false);
		~AnimatedSprite();
	private:
		std::map<std::string,cocos2d::CCAnimation*> _animations;
		std::string _last_animation;
};

#endif /* defined(__ActionPlataformer__AnimatedSprite__) */
