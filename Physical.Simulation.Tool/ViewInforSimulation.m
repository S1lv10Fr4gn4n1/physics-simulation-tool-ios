//
//  ViewInforSimulation.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 02/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewInforSimulation.h"

@implementation ViewInforSimulation

@synthesize scrollView;
@synthesize view1;
@synthesize labelConfiguration;

- (void)viewDidLoad
{
    [scrollView addSubview: view1];
	[scrollView setContentSize: CGSizeMake(800, 600)];
    
    labelConfiguration.text = NSLocalizedString(@"configuration", @"");
}

- (void)viewDidUnload 
{
    [self setScrollView:nil];
    [self setView:nil];
    
    [self setLabelConfiguration:nil];
    [super viewDidUnload];
}
@end
