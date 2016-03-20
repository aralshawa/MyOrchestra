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
		
		// Default config
		[self updateAudioResourceWithData:nil];
	}
	
	return self;
}

- (void)updateAudioResourceWithData:(NSData *)data
{
	if (data != nil) {
		NSError *error;
		self.audioPlayer = [[AVAudioPlayer alloc] initWithData:data error:&error];
		
	} else {
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
	}
	
	[self setVolume:0.5];
	self.audioPlayer.numberOfLoops = -1;
	[self.audioPlayer prepareToPlay];
}

- (void)setEnabled:(BOOL)enabled
{
	_enabled = enabled;
	
	// Update audio
	if (enabled) {
		[self.audioPlayer play];
	} else {
		[self.audioPlayer stop];
	}
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
