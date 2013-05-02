//
//  AnimatedSprite.cpp
//  ActionPlataformer
//
//  Created by Lucas Bittencourt on 4/4/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(std::string frame_name)
{
	initWithSpriteFrameName(frame_name.c_str());
	this->autorelease();
}

cocos2d::CCAnimation* AnimatedSprite::animationWithFile(const std::string& name, int frame_count, float delay)
{
	cocos2d::CCArray* frames = cocos2d::CCArray::create(frame_count);
	std::stringstream ss;
	for(int i = 0; i < frame_count; i++)
	{
		ss.str("");
		ss << (i + 1);
		std::string file = name + ss.str() + ".png";
		
		if(frame_count == 1)
			file = name + ".png";
		
		cocos2d::CCSpriteFrame* frame = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.c_str());
		frames->addObject(frame);
	}
	return cocos2d::CCAnimation::create(frames, delay);
}
void AnimatedSprite::addAnimation(cocos2d::CCAnimation* animation, const std::string& name)
{
	_animations[name] = animation;
	animation->retain();
}
void AnimatedSprite::runAnimation(const std::string& name, bool loop, bool start_again)
{
 
	
	if(!start_again && _last_animation == name) return;
	
	_last_animation = name;
	
	cocos2d::CCAnimate* animate = cocos2d::CCAnimate::create(_animations[name]);
	
	if(loop)
	{
		cocos2d::CCRepeatForever* repeat = cocos2d::CCRepeatForever::create(animate);
		runAction(repeat);
	}
	else
	{
		runAction(animate);
	}
}
AnimatedSprite::~AnimatedSprite()
{
	for(auto it : _animations)
	{
		it.second->release();
	}
}