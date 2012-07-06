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

- (void)viewDidLoad
{
    [scrollView setScrollEnabled:YES];
}

- (void)viewDidUnload 
{
    [self setScrollView:nil];
    [super viewDidUnload];
}
@end
