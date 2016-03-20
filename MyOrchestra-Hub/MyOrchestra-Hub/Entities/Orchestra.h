//
//  Orchestra.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OrchestralSection.h"

@interface Orchestra : NSObject

@property (nonatomic, readonly) NSUInteger numberOfSections;
@property (nonatomic, readonly) BOOL audioPlaying;
@property (nonatomic, readonly) NSArray<OrchestralSection *> *sections;

@property (nonatomic) NSUInteger selectedSectionIdx;

- (void)beginLooping;
- (void)pauseLooping;
- (void)updateCurrentSectionWithNormalizedVolume:(double)vol;

@end
