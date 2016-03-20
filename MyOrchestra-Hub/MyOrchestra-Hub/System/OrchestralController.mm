//
//  OrchestralController.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "OrchestralController.h"

#import "hello-myo.h"

#define fBoundValueToRange(value, min, max) fmin(fmax(value - min, 0), max)

@implementation OrchestralController {
	BOOL yawCalComplete;
	BOOL pitchCalComplete;
}

@synthesize minYaw = _minYaw, maxYaw = _maxYaw, minPitch = _minPitch, maxPitch = _maxPitch;

- (instancetype)init
{
	if (self = [super init])
	{
		_orchestra = [[Orchestra alloc] init];
	}
	
	return self;
}

- (void)beginCalibrationAndUpdates
{
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		// Call myo logic async
		DataCollector::beginCalibrationAndBeginLoop((__bridge void *)self);
	});
	
	NSLog(@"What up!?");
}

- (void)yawCalibrationComplete:(MaxMinCalibrationTuple)result
{
	_minYaw = result.min;
	_maxYaw = result.max;
	yawCalComplete = YES;
	
	[self initAudioIfCalComplete];
}

- (void)pitchCalibrationComplete:(MaxMinCalibrationTuple)result
{
	_minPitch = result.min;
	_maxPitch = result.max;
	pitchCalComplete = YES;
	
	[self initAudioIfCalComplete];
}

- (void)initAudioIfCalComplete;
{
	if (yawCalComplete && pitchCalComplete)
		[self.orchestra beginLooping];
}

- (void)onUpdateSectionSelectYaw:(double)degrees
{
	double normalizedYaw = [self normalizedYaw:degrees];
	
	NSUInteger sectionIdx = [self sectionForNormalizedYaw:normalizedYaw];
	
	if (sectionIdx != NSUIntegerMax)
	{
		[self.orchestra setSelectedSectionIdx:sectionIdx];
	}
	
	NSLog(@"%@", self.orchestra);
}

- (void)onUpdateVolumeSelectPitch:(double)degrees
{
	double normalizedPitch = [self normalizedPitch:degrees];
	
	[self.orchestra updateCurrentSectionWithNormalizedVolume:normalizedPitch];
	
	NSLog(@"%@", self.orchestra);
}

#pragma mark - Utilities
- (NSUInteger)sectionForNormalizedYaw:(double)yaw
{
	double yawThreshold = (double)1 / self.orchestra.numberOfSections;
	
	for (int sec = 0; sec < self.orchestra.numberOfSections; sec ++)
	{
		if (yaw > (sec * yawThreshold) && yaw < ((sec + 1) * yawThreshold)) {
			return sec;
		}
	}
	
	return NSUIntegerMax;
}

- (double)normalizedYaw:(double)yaw
{
	return fBoundValueToRange(yaw, self.minYaw, self.maxYaw) / self.maxYaw;
}

- (double)normalizedPitch:(double)pitch
{
	return fBoundValueToRange(pitch, self.minPitch, self.maxPitch) / self.maxPitch;
}

#pragma mark - C Trampoline Functions

void yawCalibrationComplete(void *context, struct MaxMinCalibrationTuple result)
{
	return [(__bridge id)context yawCalibrationComplete:result];
}

void pitchCalibrationComplete(void *context, struct MaxMinCalibrationTuple result)
{
	return [(__bridge id)context pitchCalibrationComplete:result];
}

void onUpdateSectionSelectYaw(void *context, double degrees)
{
	return [(__bridge id)context onUpdateSectionSelectYaw:degrees];
}

void onUpdateVolumeSelectPitch(void *context, double degrees)
{
	return [(__bridge id)context onUpdateVolumeSelectPitch:degrees];
}

@end
