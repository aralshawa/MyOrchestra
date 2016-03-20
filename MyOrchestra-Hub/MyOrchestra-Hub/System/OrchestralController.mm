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
	BOOL calibrationDispatched;
	id<OrchestralControllerDelegate> _delegate;
}

@synthesize minYaw = _minYaw, maxYaw = _maxYaw, minPitch = _minPitch, maxPitch = _maxPitch;

- (instancetype)initWithDelegate:(id<OrchestralControllerDelegate>)delegate
{
	if (self = [super init])
	{
		_orchestra = [[Orchestra alloc] init];
		_delegate = delegate;
	}
	
	return self;
}

- (void)resumeUpdates
{
	DataCollector::updateContextOfChanges = true;
	
	if ((!yawCalComplete || !pitchCalComplete) && !calibrationDispatched) {
		calibrationDispatched = YES;
		[_delegate updateSystemState:SystemStateIndicatorStateSetup];
		
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
			// Call myo logic async
			DataCollector::beginCalibrationAndBeginLoop((__bridge void *)self);
		});
	}
	
	if (yawCalComplete && pitchCalComplete) {
		[self.orchestra beginLooping];
		[_delegate updateSystemState:SystemStateIndicatorStateRunning];
	}
}

- (void)pauseUpdates
{
	DataCollector::updateContextOfChanges = false;
	[self.orchestra pauseLooping];
	[_delegate updateSystemState:SystemStateIndicatorStateStopped];
}

- (void)yawCalibrationComplete:(MaxMinCalibrationTuple)result
{
	_minYaw = result.min + 360;
	_maxYaw = result.max + 360;
	yawCalComplete = YES;
	
	[self initAudioIfCalComplete];
}

- (void)pitchCalibrationComplete:(MaxMinCalibrationTuple)result
{
	_minPitch = result.min + 360;
	_maxPitch = result.max + 360;
	pitchCalComplete = YES;
	
	[self initAudioIfCalComplete];
}

- (void)initAudioIfCalComplete;
{
	if (yawCalComplete && pitchCalComplete) {
		[self.orchestra beginLooping];
		[_delegate updateSystemState:SystemStateIndicatorStateRunning];
	}
}

- (void)onUpdateSectionSelectYaw:(double)degrees
{
	double normalizedYaw = [self normalizedYaw:degrees + 360];
	
	NSUInteger sectionIdx = [self sectionForNormalizedYaw:normalizedYaw];
	
	if (sectionIdx != NSUIntegerMax)
	{
		[self.orchestra setSelectedSectionIdx:sectionIdx];
		
		[_delegate updateSelectedSectionWithIndex:sectionIdx];
	}
	
	NSLog(@"%@", self.orchestra);
}

- (void)onUpdateVolumeSelectPitch:(double)degrees
{
	double normalizedPitch = [self normalizedPitch:degrees + 360];
	
	[self.orchestra updateCurrentSectionWithNormalizedVolume:normalizedPitch];
	
	NSLog(@"%@", self.orchestra);
}

- (void)onUpdateMode:(SystemMode)mode
{
	switch (mode) {
		case SystemModeRegular:
		{
			for (OrchestralSection *section in self.orchestra.sections) {
				[section updateAudioResourceWithData:nil];
			}
			
			break;
		}
		case SystemModeSocial:
		{
			// pariwak.com/voice/recordings
			
			NSURLRequest *urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:@"https://pariwak.com/voice/recordings"]];
			NSOperationQueue *queue = [[NSOperationQueue alloc] init];
			[NSURLConnection sendAsynchronousRequest:urlRequest queue:queue completionHandler:^(NSURLResponse *response, NSData *data, NSError *error)
			 {
				 if (error)
				 {
					 NSLog(@"Error with recording request: %@", [error localizedDescription]);
				 }
				 else
				 {
					 NSLog(@"%@", [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding]);
					 
					 NSArray *jsonArray = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
					 
					 for (int secIdx = 0; secIdx < self.orchestra.sections.count; secIdx ++) {
						 OrchestralSection *section = self.orchestra.sections[secIdx];
						 
						 
						 NSURLRequest *urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:[@"https://pariwak.com/public/" stringByAppendingString:jsonArray[jsonArray.count - secIdx - 1]]]];
						 NSOperationQueue *queue = [[NSOperationQueue alloc] init];
						 
						 [NSURLConnection sendAsynchronousRequest:urlRequest queue:queue completionHandler:^(NSURLResponse *response, NSData *data, NSError *error)
						  {
							  [section updateAudioResourceWithData:data];

						  }];
					 }
				 }
			 }];
			
			break;
		}
	}
}

#pragma mark - Utilities
- (NSUInteger)sectionForNormalizedYaw:(double)yaw
{
	double yawThreshold = (double)1 / self.orchestra.numberOfSections;
	
	for (int sec = 0; sec < self.orchestra.numberOfSections; sec ++)
	{
		if (yaw > (sec * yawThreshold) && yaw < ((sec + 1) * yawThreshold)) {
			return 2 - sec;
		}
	}
	
	return NSUIntegerMax;
}

- (double)normalizedYaw:(double)yaw
{
	return fBoundValueToRange(yaw, self.minYaw, self.maxYaw) / (self.maxYaw - self.minYaw);
}

- (double)normalizedPitch:(double)pitch
{
	return fBoundValueToRange(pitch, self.minPitch, self.maxPitch) / (self.maxPitch - self.minPitch);
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
