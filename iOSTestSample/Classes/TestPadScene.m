//
//  HelloWorldLayer.m
//  TestView
//
//  Created by harry Kim April 2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

// Import the interfaces
#import "TestPadScene.h"

#import <UIKit/UIKit.h>
#import "CommonData.h"
#import "TestPadSequence.h"

#import <MediaPlayer/MediaPlayer.h>
#import "MovieLayer.h"
// HelloWorld implementation
@implementation TestPadScene

enum {
	kTagPage = 0,
    kTagBackground = 1,
    kTagMenu = 2,
    kTagSpriteSheet = 3,
};

//NSArray* arrayPoint;
//RDDrawingSprite* mds;
//RDDrawingSpriteStyle* style;

CGRect drawFrame;

const int drawX = 180;
const int drawY = 330;
const int drawEndX = 768-180;
const int drawEndY = 1024-330;


bool CheckMousePosition( int mx, int my)
{
	if( mx < drawX ||
	   my < drawY ||
	   mx > drawEndX ||
	   my > drawEndY )
		return false;
	else {
		return true;
	}

}

////////////////////////////////////////////////////////
///////////////////////touch paint//////////////////////
////////////////////////////////////////////////////////
- (void)ccTouchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	mouseSwiped = NO;
	UITouch *touch = [touches anyObject];
	
	if ([touch tapCount] == 2) {
		drawImage.image = nil;
		return;
	}
	
	lastPoint = [touch locationInView:[[CCDirector sharedDirector]openGLView]];
	//lastPoint.y -= 20;
	
}


- (void)ccTouchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	mouseSwiped = YES;
	UITouch *touch = [touches anyObject];	
	CGPoint currentPoint = [touch locationInView:[[CCDirector sharedDirector]openGLView]];
	//currentPoint.y -= 20; // only for 'kCGLineCapRound'
	if( CheckMousePosition(currentPoint.x,currentPoint.y) && CheckMousePosition(lastPoint.x,lastPoint.y))
	{
		UIGraphicsBeginImageContext(drawFrame.size);
		//Albert Renshaw - Apps4Life
		[drawImage.image drawInRect:CGRectMake(0, 0, drawImage.frame.size.width, drawImage.frame.size.height)]; //originally self.frame.size.width, self.frame.size.height)];
		CGContextSetLineCap(UIGraphicsGetCurrentContext(), kCGLineCapRound); //kCGLineCapSquare, kCGLineCapButt, kCGLineCapRound
		CGContextSetLineWidth(UIGraphicsGetCurrentContext(), 20.0); // for size
		CGContextSetRGBStrokeColor(UIGraphicsGetCurrentContext(), 1.0, 0.0, 0.0, 1.0); //values for R, G, B, and Alpha
		CGContextBeginPath(UIGraphicsGetCurrentContext());
		CGContextMoveToPoint(UIGraphicsGetCurrentContext(), lastPoint.x, lastPoint.y);
		CGContextAddLineToPoint(UIGraphicsGetCurrentContext(), currentPoint.x, currentPoint.y);
		CGContextStrokePath(UIGraphicsGetCurrentContext());
		drawImage.image = UIGraphicsGetImageFromCurrentImageContext();
		UIGraphicsEndImageContext();
	}
	lastPoint = currentPoint;
	
	mouseMoved++;
	
	if (mouseMoved == 10) {
		mouseMoved = 0;
	}
}

- (void)ccTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	UITouch *touch = [touches anyObject];
	
	if ([touch tapCount] == 2) {
		drawImage.image = nil;
		return;
	}
	if(!mouseSwiped) {
		if( CheckMousePosition(lastPoint.x,lastPoint.y) )
		{
			UIGraphicsBeginImageContext(drawFrame.size);
			[drawImage.image drawInRect:CGRectMake(0, 0, drawImage.frame.size.width, drawImage.frame.size.height)]; //originally self.frame.size.width, self.frame.size.height)];
			CGContextSetLineCap(UIGraphicsGetCurrentContext(), kCGLineCapRound); //kCGLineCapSquare, kCGLineCapButt, kCGLineCapRound
			CGContextSetLineWidth(UIGraphicsGetCurrentContext(), 20.0);
			CGContextSetRGBStrokeColor(UIGraphicsGetCurrentContext(), 1.0, 0.0, 0.0, 1.0);
			CGContextMoveToPoint(UIGraphicsGetCurrentContext(), lastPoint.x, lastPoint.y);
			CGContextAddLineToPoint(UIGraphicsGetCurrentContext(), lastPoint.x, lastPoint.y);
			CGContextStrokePath(UIGraphicsGetCurrentContext());
			CGContextFlush(UIGraphicsGetCurrentContext());
			drawImage.image = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
		}
	}
}

////////////////////////////////////////////////////////
////////////////end of touch paint//////////////////////
////////////////////////////////////////////////////////

+(id) scene
{
	// 'scene' is an autorelease object.
	CCScene *scene = [CCScene node];
	
	// 'layer' is an autorelease object.
	TestPadScene *layer = [TestPadScene node];
	
	// add layer as a child to scene
	[scene addChild: layer];
	
	// return the scene
	return scene;
}

-(void) onEnter
{
	[super onEnter];
	
	drawImage = [[UIImageView alloc] initWithImage:nil];
	drawFrame = [[[CCDirector sharedDirector]openGLView] frame];//CGRectMake(100.0f, 100.0f, 420.f, 144.0f);
	drawImage.frame = drawFrame;
	mouseMoved = 0;	
	
	[[[CCDirector sharedDirector]openGLView]addSubview:drawImage];
}

-(void) onExit
{
	[super onExit];
	
	[drawImage removeFromSuperview];
	[drawImage release];
	
	drawImage = nil;
	
	//[self OnHideTestSequence];
}

// on "init" you need to initialize your instance
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init] )) {
	
		// create and initialize a Label
		//CCLabel* label = [CCLabel labelWithString:@"TestLand" fontName:@"Marker Felt" fontSize:64];

		CCSprite *bgPage = [CCSprite spriteWithFile:[g_commonData GetPageName]];
		CCSprite *bgPad = [CCSprite spriteWithFile:@"bg_768x1024.png"];
		CCSprite *TestSprite = [CCSprite spriteWithFile:[g_commonData GetTestPadName]];
		
		// ask director the the window size
		CGSize size = [[CCDirector sharedDirector] winSize];
	
		// position the label on the center of the screen
		//label.position =  ccp( size.width /2 , size.height/2 );
		
		bgPage.position = ccp( size.width /2 , size.height/2 );
		//bgSprite.rotation = 90;
		
		bgPad.position = ccp( size.width /2 , size.height/2 );
		//bgSprite.rotation = 90;
		
		TestSprite.position = ccp( size.width /2 , size.height/2 );
		// add the label as a child to this Layer
		
		
		[self addChild: bgPage z:kTagPage tag:kTagPage];
		[self addChild: TestSprite z:kTagSpriteSheet tag:kTagSpriteSheet];
		[self addChild: bgPad z:kTagBackground tag:kTagBackground];
		//[self addChild: label z:kTagMenu tag:kTagMenu];
		
		//[label runAction:[CCFadeIn actionWithDuration:3.0f]];
		//[label runAction:[CCFadeOut actionWithDuration:3.0f]];
		
		isTouchEnabled = YES;
		
		//drawImage = [[UIImageView alloc] initWithImage:nil];
		//drawImage.frame = self.view.frame;
		//[self.view addSubview:drawImage];
		mouseMoved = 0;
		
		CCMenuItem* numItem = [CCMenuItemImage itemFromNormalImage:@"btn_num_active.png"
													  selectedImage:@"btn_num_touch.png"
															 target:self
														   selector:@selector(numItemCallback:)];
		CCMenuItem* outItem = [CCMenuItemImage itemFromNormalImage:@"btn_out_active.png"
													   selectedImage:@"btn_out_touch.png"
													   disabledImage:@"btn_out_no_active.png"
															  target:self
															selector:@selector(outItemCallback:)];
		
		CCMenu *menu = [CCMenu menuWithItems: numItem, outItem, 
						nil];
		
		menu.position = ccp(0,0);
		numItem.position = ccp(219+135/2,1024-773-50/2);
		outItem.position = ccp(439+135/2,1024-773-50/2);
		
		[self addChild:menu z:1 tag:1];
		
		
		//Paint_AppViewController * viewController = [Paint_AppViewController alloc];
		//[[[CCDirector sharedDirector]openGLView]addSubview:viewController.view];
		
	}
	return self;
}

bool bShowTestSequnece = FALSE;
CCLabel* label[10] = {nil, nil, nil, nil, nil,nil, nil, nil, nil, nil};


- (void) OnHideTestSequence
{
	for (int i = 0; i < 10; i++)
	{
		if(label[i] != nil)
		{
			[self removeChild:label[i] cleanup:TRUE];
			//[label[i] release];
			label[i] = nil;
		}
	}
}

- (void) OnShowTestSequence
{
	int TestId = g_commonData.TestPadId;
	
	int count[] = { 0, 4, 8, 9, 6, 4, 2 };
	
	CGPoint areas[10];
	
	for (int i = 0; i < count[TestId]; i++)
	{
		NSString* name = [NSString stringWithFormat:@"%d", i+1];
		label[i] = [CCLabel labelWithString:name fontName:@"Helvetica" fontSize:32];
		label[i].color = ccc3(0,0,0);
		[self addChild: label[i] z:kTagMenu tag:kTagMenu];
		CGPoint value;
		value.x = 190+areas[i].x;
		value.y = 690-areas[i].y;
		label[i].position = value;
		//[label runAction:[CCFadeIn actionWithDuration:3.0f]];
		//[label runAction:[CCFadeOut actionWithDuration:3.0f]];
	}
}

- (void) numItemCallback: (id) sender 
{
	bShowTestSequnece = !bShowTestSequnece;
	if(bShowTestSequnece)
		[self OnShowTestSequence];
	else {
		[self OnHideTestSequence];
	}

	//[[CCDirector sharedDirector] pushScene: [TestPadSequence scene]];
	NSLog(@"numItemCallback버튼이눌려졌음."); 
}


- (void) PlayMovie
{
	//NSURL *moviePathURL = [NSURL fileURLWithPath:@"test1test.mp4"];
	NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"test1test.mp4" ofType:@""]];
	MPMoviePlayerController *moviePlayer = [[MPMoviePlayerController alloc] initWithContentURL:url];

// Register to receive a notification when the movie has finished playing.
[[NSNotificationCenter defaultCenter] addObserver:self
										 selector:@selector(moviePlayBackDidFinish:)
											 name:MPMoviePlayerPlaybackDidFinishNotification
										   object:moviePlayer];

if ([moviePlayer respondsToSelector:@selector(setFullscreen:animated:)]) {
	// Use the new 3.2 style API
	moviePlayer.controlStyle = MPMovieControlStyleNone;
	moviePlayer.shouldAutoplay = YES;
	// This does blows up in cocos2d, so we'll resize manually
	[moviePlayer setFullscreen:YES animated:YES];
	moviePlayer.scalingMode = MPMovieScalingModeAspectFill;
	[moviePlayer.view setTransform:CGAffineTransformMakeRotation((float)M_PI_2)];
	CGSize winSize = [[CCDirector sharedDirector] winSize];
	moviePlayer.view.frame = CGRectMake(0, 0, winSize.width, winSize.height);	// width and height are swapped after rotation
	[[[CCDirector sharedDirector] openGLView] addSubview:moviePlayer.view];
} else {
	// Use the old 2.0 style API
	moviePlayer.movieControlMode = MPMovieControlModeHidden;
	[moviePlayer play];
}
}
- (void)moviePlayBackDidFinish:(NSNotification*)notification {
	MPMoviePlayerController *moviePlayer = [notification object];
	[[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:moviePlayer];
	
	// If the moviePlayer.view was added to the openGL view, it needs to be removed
	if ([moviePlayer respondsToSelector:@selector(setFullscreen:animated:)]) {
		[moviePlayer.view removeFromSuperview];
	}
	
	[moviePlayer release];
	
	[[CCDirector sharedDirector] popScene];
}

//id movieLayer = 0;
- (void) movieFinished:(NSNotification*)notification
{
	//[movieLayer release];
	[[CCDirector sharedDirector] popScene];
	
	//[[CCDirector sharedDirector] popScene];
}

- (void) outItemCallback: (id) sender 
{
	//if g_commonData.
	int TestId = g_commonData.TestPadId;
	if(TestId == 3)
		[self PlayMovie];
	else {
		[[CCDirector sharedDirector] popScene];
	}

	//id movieLayer = [MovieLayer layerWithMovieName:@"test1test.mp4" type:@"" target:self selector:@selector(movieFinished:)];
	//[[CCDirector sharedDirector] pushScene:movieLayer];
	//[movieLayer play];
	NSLog(@"outItemCallback버튼이눌려졌음."); 
}

// on "dealloc" you need to release all your retained objects
- (void) dealloc
{
	// in case you have something to dealloc, do it in this method
	// in this particular example nothing needs to be released.
	// cocos2d will automatically release all the children (Label)
	
	// don't forget to call "super dealloc"
	[super dealloc];
}
@end
