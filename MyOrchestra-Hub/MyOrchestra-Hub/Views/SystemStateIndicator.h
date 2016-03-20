//
//  SystemStateIndicator.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-20.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OrchestralController.h" // Bad form

@interface SystemStateIndicator : NSView

- (void)setState:(SystemStateIndicatorState)state;

@end
