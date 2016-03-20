//
//  OrchestralController.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Orchestra.h"
#import "OrchestralController-C-Interface.h"

@protocol OrchestralControllerDelegate;

@interface OrchestralController : NSObject

- (instancetype)initWithDelegate:(id <OrchestralControllerDelegate>)delegate;

@property (nonatomic, strong) Orchestra *orchestra;

@property (nonatomic, readonly) double minYaw;
@property (nonatomic, readonly) double maxYaw;

@property (nonatomic, readonly) double minPitch;
@property (nonatomic, readonly) double maxPitch;

- (void)resumeUpdates;
- (void)pauseUpdates;

- (void)yawCalibrationComplete:(struct MaxMinCalibrationTuple)result;
- (void)pitchCalibrationComplete:(struct MaxMinCalibrationTuple)result;

// Updates should not be called until calibration is complete
- (void)onUpdateSectionSelectYaw:(double)degrees;
- (void)onUpdateVolumeSelectPitch:(double)degrees;

@end


@protocol OrchestralControllerDelegate <NSObject>

- (void)updateSelectedSectionWithIndex:(NSUInteger)secIdx;

@end