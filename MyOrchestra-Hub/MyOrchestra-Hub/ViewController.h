//
//  ViewController.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OrchestralController.h"
#import "OrchestralSectionalView.h"

@interface ViewController : NSViewController <OrchestralControllerDelegate>

@property (nonatomic, strong) OrchestralController *orchController;

@property (weak) IBOutlet OrchestralSectionalView *orchestraSectionViews;
@property (weak) IBOutlet NSButton *playButton;

- (IBAction)onPlayButtonPress:(NSButton *)sender;
- (IBAction)onModeSelect:(NSPopUpButton *)sender;
- (void)updateSelectedSectionWithIndex:(NSUInteger)secIdx;

@end

