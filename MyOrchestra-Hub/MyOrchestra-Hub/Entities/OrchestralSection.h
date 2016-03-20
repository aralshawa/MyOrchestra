//
//  OrchestralSection.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AppKit/NSSound.h>

typedef NS_ENUM(NSUInteger, OrchestralSectionType) {
	OrchestralSectionTypeStrings,
	OrchestralSectionTypePrecusion,
	OrchestralSectionTypeWoodwind
};

@interface OrchestralSection : NSObject

- (instancetype)initSectionWithType:(OrchestralSectionType)type;
- (void)updateAudioResourceWithData:(NSData *)data;

@property (nonatomic, readonly) OrchestralSectionType type;
@property (nonatomic) BOOL enabled;
@property (nonatomic) double volume;

@property (nonatomic, strong) AVAudioPlayer *audioPlayer;

- (void)setAudioPanRatio:(double)pan;

@end
