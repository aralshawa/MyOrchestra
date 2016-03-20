//
//  ViewController.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "ViewController.h"

#import <QuartzCore/QuartzCore.h>

@implementation ViewController  {
	BOOL _playState;
	SystemMode _mode;
	
	NSBezierPath *leftSectionPath;
}

- (void)resetState
{
	_playState = NO;
	_mode = SystemModeRegular;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[self.view setWantsLayer:YES];
	self.view.layer.backgroundColor = [NSColor whiteColor].CGColor;
	
	// Initialization
	[self resetState];
	self.orchController = [[OrchestralController alloc] initWithDelegate:self];
	
//	[self.orchController resumeUpdates];
}

- (void)setRepresentedObject:(id)representedObject {
	[super setRepresentedObject:representedObject];
}

- (IBAction)onPlayButtonPress:(NSButton *)sender {
	_playState = !_playState;
	
	// Start/stop playing ...
	if (_playState) {
		[self.orchController resumeUpdates];
		sender.title = @"Stop";
	} else {
		[self.orchController pauseUpdates];
		sender.title = @"Play!";
	}
	
	NSLog(@"Play State = %zd", _playState);
}

- (IBAction)onModeSelect:(NSPopUpButton *)sender {
	_mode = sender.indexOfSelectedItem;
	
	// Stop playing...
	NSLog(@"Mode = %zd", _mode);
	
	if (_playState) {
		_playState = !_playState;
		[self.orchController pauseUpdates];
		self.playButton.title = @"Play!";
	}
	
	// Changing the mode
	[self.orchController onUpdateMode:_mode];
}

- (void)updateSelectedSectionWithIndex:(NSUInteger)secIdx
{
	[self.orchestraSectionViews setSelected:secIdx];
}

- (void)updateSystemState:(SystemStateIndicatorState)state
{
	[self.stateIndicator setState:state];
}

@end
