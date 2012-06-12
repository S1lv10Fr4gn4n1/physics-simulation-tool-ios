//
//  ViewControllerViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

@interface ViewController () {
    Shader * shader;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
- (void)loadShaders;
@end


@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
    
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    [self loadShaders];
    
//    self.effect = [[GLKBaseEffect alloc] init];
//    self.effect.light0.enabled = GL_TRUE;
//    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
//    
//    glEnable(GL_DEPTH_TEST);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    // TODO implementation 
    
    self.effect = nil;
    
    delete shader;
}

- (void)loadShaders
{
    NSString *vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString *fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    NSString *geomShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"gsh"];
    const char* vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* geomShaderSource = [[NSString stringWithContentsOfFile:geomShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    shader = new Shader(vertShaderSource, fragShaderSource, geomShaderSource);
    shader->loadShaders();
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
    // TODO - implementation
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the object again with ES2
    glUseProgram(shader->getProgram());
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


@end
