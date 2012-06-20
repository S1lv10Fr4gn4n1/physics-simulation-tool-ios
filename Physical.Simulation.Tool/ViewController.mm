    //
//  ViewControllerViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"


@interface ViewController () {
    MainEngine * mainEngine;
    MainGraphic * mainGraphic;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
- (void)initializeSimulator;
- (void)initializeFrame;
- (void)initializeLibraryGraphic;
- (void)initializeLibraryEngine;
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
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
            
    [self setupGL];

    [self initializeSimulator];
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
    mainEngine->rotatedScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    mainGraphic->rotatedScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
//    self.effect = [[GLKBaseEffect alloc] init];
//    self.effect.light0.enabled = GL_TRUE;
//    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
//    
//    glEnable(GL_DEPTH_TEST);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    /// TODO - implementation 
    
    self.effect = nil;
    
    delete mainGraphic;
    delete mainEngine;
}

- (void)initializeSimulator
{
    [self initializeLibraryGraphic];
    [self initializeLibraryEngine];
    [self initializeFrame];
}

- (void)initializeFrame
{
    SimulatedObject * simulatedObject = new SimulatedObject();
    simulatedObject->setMode(GL_LINE_LOOP);
    simulatedObject->setColor(MakeColor(0, 0, 0, 0, 4));
    simulatedObject->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
    simulatedObject->addPointer(MakePointer(-0.65, 0.65, 0.00));
    simulatedObject->addPointer(MakePointer(-0.15, 0.65, 0.00));
    simulatedObject->addPointer(MakePointer(-0.15, 0.15, 0.00));
    simulatedObject->addPointer(MakePointer(-0.65, 0.15, 0.00));
    
    mainEngine->addSimulatedObjectInWorld(simulatedObject);
    
//    // Sets up an array of values to use as the sprite vertices.
//    const GLfloat spriteVertices[] = {
//        -0.5f, -0.5f,
//        0.5f, -0.5f,
//        -0.5f,  0.5f,
//        0.5f,  0.5f,
//    };
//    
//    // Sets up an array of values for the texture coordinates.
//    const GLshort spriteTexcoords[] = {
//        0, 0,
//        1, 0,
//        0, 1,
//        1, 1,
//    };
//    
//    CGImageRef spriteImage;
//	CGContextRef spriteContext;
//	GLubyte *spriteData;
//	size_t	width, height;
//    
//	// Sets up matrices and transforms for OpenGL ES
//	glViewport(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
//	glMatrixMode(GL_MODELVIEW);
//	
//	// Clears the view with black
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	
//	// Sets up pointers and enables states needed for using vertex arrays and textures
//	glVertexPointer(2, GL_FLOAT, 0, spriteVertices);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glTexCoordPointer(2, GL_SHORT, 0, spriteTexcoords);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	
//	// Creates a Core Graphics image from an image file
//	spriteImage = [UIImage imageNamed:@"Sprite.png"].CGImage;
//	// Get the width and height of the image
//	width = CGImageGetWidth(spriteImage);
//	height = CGImageGetHeight(spriteImage);
//	// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
//	// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.
//    
//	if(spriteImage) {
//		// Allocated memory needed for the bitmap context
//		spriteData = (GLubyte *) calloc(width * height * 4, sizeof(GLubyte));
//		// Uses the bitmap creation function provided by the Core Graphics framework. 
//		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
//		// After you create the context, you can draw the sprite image to the context.
//		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
//		// You don't need the context at this point, so you need to release it to avoid memory leaks.
//		CGContextRelease(spriteContext);
//		
//        /* OpenGL name for the sprite texture */
//        GLuint spriteTexture;
//        
//		// Use OpenGL ES to generate a name for the texture.
//		glGenTextures(1, &spriteTexture);
//		// Bind the texture name. 
//		glBindTexture(GL_TEXTURE_2D, spriteTexture);
//		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		// Specify a 2D texture image, providing the a pointer to the image data in memory
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
//		// Release the image data
//		free(spriteData);
//		
//		// Enable use of the texture
//		glEnable(GL_TEXTURE_2D);
//		// Set a blending function to use
//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//		// Enable blending
//		glEnable(GL_BLEND);
//	}
}

- (void)initializeLibraryGraphic
{
    NSString *vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString *fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    NSString *geomShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"gsh"];
    const char* vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* geomShaderSource = [[NSString stringWithContentsOfFile:geomShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    mainGraphic = new MainGraphic();
    mainGraphic->initializeShader(vertShaderSource, fragShaderSource, geomShaderSource);
    mainGraphic->initializeNDC(self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)initializeLibraryEngine
{
    mainEngine = new MainEngine();
    mainEngine->updateInformation();
    mainEngine->start();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
//    CGPoint cgPoint = [[touches anyObject] locationInView: self.view]; 
//    Pointer * pointer = MakePointer(cgPoint.x, cgPoint.y, 0);
//    NSLog(@"Began -> x: %f, y: %f, count touches: %u", pointer->x, pointer->y, [touches count]);
//    NSLog(@"width: %f, height %f", self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
//    CGPoint cgPoint = [[touches anyObject] locationInView: self.view]; 
//    Pointer pointer = MakePointer(cgPoint.x, cgPoint.y, 0);
//    NSLog(@"Cancelled -> x: %f, y: %f", pointer.x, pointer.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//    CGPoint cgPoint = [[touches anyObject] locationInView: self.view]; 
//    Pointer pointer = MakePointer(cgPoint.x, cgPoint.y, 0);
//    NSLog(@"Ended -> x: %f, y: %f", pointer.x, pointer.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
//    CGPoint cgPoint = [[touches anyObject] locationInView: self.view]; 
//    Pointer pointer = MakePointer(cgPoint.x, cgPoint.y, 0);
//    NSLog(@"Moved -> x: %f, y: %f", pointer.x, pointer.y);
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
//    mainEngine->updateInformation();
//    mainGraphic->updateInformation();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    mainGraphic->draw(mainEngine->getWorld()->getSimulatedObjects());
}

@end
