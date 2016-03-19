//
//  OrchestralSection.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, OrchestralSectionType) {
	OrchestralSectionTypeStrings,
	OrchestralSectionTypePrecusion,
	OrchestralSectionTypeWoodwind
};

@interface OrchestralSection : NSObject

- (instancetype)initSectionWithType:(OrchestralSectionType)type;

@property (nonatomic, readonly) OrchestralSectionType type;
//@property (nonatomic, readonly) NSUInteger sequenceNumber;
@property (nonatomic) BOOL enabled;
@property (nonatomic) double volume;

@property (nonatomic) double speed;

@end
