//
//  Orchestra.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "Orchestra.h"

@implementation Orchestra {
	NSUInteger _numberOfSections;
	NSMutableArray <OrchestralSection *> *_sections;
}

- (instancetype)init
{
	if (self = [super init])
	{
		_sections = [[NSMutableArray alloc] initWithCapacity:3];
		[_sections addObject:[[OrchestralSection alloc] initSectionWithType:OrchestralSectionTypeStrings]];
		[_sections addObject:[[OrchestralSection alloc] initSectionWithType:OrchestralSectionTypePrecusion]];
		[_sections addObject:[[OrchestralSection alloc] initSectionWithType:OrchestralSectionTypeWoodwind]];
	}
	
	return self;
}

- (NSArray *)sections
{
	return _sections;
}

- (NSUInteger)numberOfSections
{
	return _sections.count;
}

- (void)beginLooping
{
	for (int secIdx = 0; secIdx < self.sections.count; secIdx ++) {
		OrchestralSection *section = self.sections[secIdx];
		
		// Set up pan ratio
		// Note: -1 = perfect L channel, 1 = perfect R channel
		double leftRightPanRatio = -1.f + (2 * (double)secIdx/(self.sections.count - 1)) + (secIdx == 0 ? 0.1 : ((secIdx == self.sections.count - 1) ? -0.1: 0));
		[section setAudioPanRatio:leftRightPanRatio];
		
		// Set enabled
		[section setEnabled:YES];
	}
}

- (void)pauseLooping
{
	for (OrchestralSection *section in self.sections) {
		[section setEnabled:NO];
	}
}

- (BOOL)audioPlaying
{
	for (OrchestralSection *section in self.sections) {
		if (section.enabled) {
			return YES;
		}
	}
	
	return  NO;
}

- (void)updateCurrentSectionWithNormalizedVolume:(double)vol
{
	_sections[self.selectedSectionIdx].volume = vol;
}

- (NSString *)description
{
	NSString *sectionSummaries = @"";
	
	for (OrchestralSection * section in _sections) {
		sectionSummaries = [sectionSummaries stringByAppendingString:[NSString stringWithFormat:@"\tSection type: %zd Volume: %f\n", section.type, section.volume]];
	}
	
	return [NSString stringWithFormat:@"Orchestra with %zd sections.\n%@", _sections.count, sectionSummaries];
}

@end
