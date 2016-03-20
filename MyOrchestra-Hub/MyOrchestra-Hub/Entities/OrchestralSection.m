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
		_volume = 0;
		
		NSURL *audioURL;
		switch (self.type) {
			case OrchestralSectionTypeStrings:
				audioURL = [[NSBundle mainBundle] URLForResource:@"low-freq" withExtension:@"wav"];
				break;
			case OrchestralSectionTypePrecusion:
				audioURL = [[NSBundle mainBundle] URLForResource:@"mid-freq" withExtension:@"wav"];
				break;
			case OrchestralSectionTypeWoodwind:
				audioURL = [[NSBundle mainBundle] URLForResource:@"hi-freq" withExtension:@"wav"];
				break;
		}
		
		// TODO: Assume no import errors
		self.audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:audioURL error:NULL];
		self.audioPlayer.numberOfLoops = -1;
		[self.audioPlayer prepareToPlay];
	}
	
	return self;
}

- (void)setEnabled:(BOOL)enabled
{
	_enabled = enabled;
	
	// Update audio
	[self.audioPlayer play];
}

/// Between -1 and 1
- (void)setAudioPanRatio:(double)pan
{
	self.audioPlayer.pan = pan;
}

- (void)setVolume:(double)volume
{
	_volume = volume < 0 ? 0 : MIN(volume, 1);
	
	// Update audio
	[self.audioPlayer setVolume:volume];
}

@end
