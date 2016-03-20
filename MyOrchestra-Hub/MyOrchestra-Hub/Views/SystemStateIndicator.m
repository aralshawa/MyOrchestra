//
//  SystemStateIndicator.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-20.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "SystemStateIndicator.h"

@implementation SystemStateIndicator {
	SystemStateIndicatorState _state;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
	NSRect rect = NSMakeRect([self bounds].origin.x, [self bounds].origin.y, [self bounds].size.width, [self bounds].size.height);
	
	NSBezierPath *path = [NSBezierPath bezierPathWithRoundedRect:rect xRadius:5.0 yRadius:5.0];
	[path addClip];
	
	NSColor *stoppedColor = [NSColor colorWithRed:250.f/235.f green:75.f/235.f blue:84.f/235.f alpha:1.f];
	NSColor *setupColor = [NSColor colorWithRed:245.f/235.f green:245.f/235.f blue:105.f/235.f alpha:1.f];
	NSColor *runningColor = [NSColor colorWithRed:142.f/235.f green:235.f/235.f blue:49.f/235.f alpha:1.f];
	
	switch (_state) {
		case SystemStateIndicatorStateStopped:
			[stoppedColor set];
			break;
		case SystemStateIndicatorStateSetup:
			[setupColor set];
			break;
		case SystemStateIndicatorStateRunning:
			[runningColor set];
			break;
	}
	
	NSRectFill(rect);
}

- (void)setState:(SystemStateIndicatorState)state
{
	_state = state;
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[self setNeedsDisplay:YES];
	});
}

@end
