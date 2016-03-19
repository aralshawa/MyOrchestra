//
//  OrchestralSection.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "OrchestralSection.h"

@implementation OrchestralSection {
	OrchestralSectionType _type;
}

- (instancetype)initSectionWithType:(OrchestralSectionType)type
{
	if (self = [super init])
	{
		_type = type;
		self.volume = 0;
		self.speed = 0.5;
	}
	
	return self;
}

- (void)setEnabled:(BOOL)enabled
{
	_enabled = enabled;
	// Update audo
}

- (void)setVolume:(double)volume
{
	_volume = volume < 0 ? 0 : MIN(volume, 1);
	// Update audio
}

- (void)setSpeed:(double)speed
{
	_speed = speed < 0 ? 0 : MIN(speed, 1);
	// Update audio
}

@end
