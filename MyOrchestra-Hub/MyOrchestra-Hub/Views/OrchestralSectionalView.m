//
//  OrchestralSectionalView.m
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-20.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import "OrchestralSectionalView.h"

@implementation OrchestralSectionalView {
	NSBezierPath *leftSectionPath;
	NSBezierPath *middleSectionPath;
	NSBezierPath *rightSectionPath;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
	// Custom View Configuration
	NSPoint topLeftCorner = NSMakePoint(0, self.frame.size.height);
	NSPoint bottomCenter = NSMakePoint(self.frame.size.width / 2, 0);
	NSPoint topRightCorner = NSMakePoint(self.frame.size.width, self.frame.size.height);
	
	[[NSColor blueColor] setStroke];
	
	leftSectionPath = [NSBezierPath bezierPath];
	[leftSectionPath moveToPoint:topLeftCorner];
	[leftSectionPath lineToPoint:bottomCenter];
	[leftSectionPath lineToPoint:NSMakePoint(0, 0)];
	[leftSectionPath lineToPoint:topLeftCorner];
	[leftSectionPath closePath];
	
	middleSectionPath = [NSBezierPath bezierPath];
	[middleSectionPath moveToPoint:topLeftCorner];
	[middleSectionPath lineToPoint:bottomCenter];
	[middleSectionPath lineToPoint:topRightCorner];
	[middleSectionPath lineToPoint:topLeftCorner];
	[middleSectionPath closePath];
	
	rightSectionPath = [NSBezierPath bezierPath];
	[rightSectionPath moveToPoint:topRightCorner];
	[rightSectionPath lineToPoint:bottomCenter];
	[rightSectionPath lineToPoint:NSMakePoint(self.frame.size.width, 0)];
	[rightSectionPath lineToPoint:topRightCorner];
	[rightSectionPath closePath];
	
	[[NSColor clearColor] setFill];
	[leftSectionPath fill];
	[middleSectionPath fill];
	[rightSectionPath fill];
}

- (void)setSelected:(NSUInteger)secIdx
{
	[[NSColor blueColor] setFill];
	if (secIdx == 0) {
		[leftSectionPath fill];
	} else if (secIdx == 1) {
		[middleSectionPath fill];
	} else {
		[rightSectionPath fill];
	}
	
	[self setNeedsDisplay:YES];
}

@end
