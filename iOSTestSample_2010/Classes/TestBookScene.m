//
//  HelloWorldLayer.m
//  TestView
//
//  Created by harry Kim April 2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

// Import the interfaces
#import "TestBookScene.h"
#import "TestPadScene.h"
#import "TestProjectAppDelegate.h"
#import "CommonData.h"

#import <UIKit/UIKit.h>
#import "SimpleAudioEngine.h"

UINavigationBar* optionsNavBar = nil;
UIToolbar* g_toolBar = nil;
UISlider *g_slider = nil;
CCSprite *g_bgPage = nil;
UILabel *pageLabel = nil;
// HelloWorld implementation
@implementation TestBookScene

enum {
	kTagPage = 0,
    kTagBackground = 1,
    kTagMenu = 2,
    kTagSpriteSheet = 3,
};

//bool bScheduleHideMenu = FALSE;

const int MaxPage = 20;	//8~28

-(void)SetMenuPage
{
	optionsNavBar.topItem.title = [NSString stringWithFormat:@"TestProject %dpage", [g_commonData GetRealPage]];
	
	pageLabel.text = [NSString stringWithFormat: @"%d page", [g_commonData GetRealPage]];
}

-(void) OnMenu
{
	if( optionsNavBar != nil && g_toolBar != nil )
		return;
	
	UIView * view = [[CCDirector sharedDirector]openGLView];
	
	//Get the bounds of the parent view
	CGRect rootViewBounds = view.bounds;
	
	//Get the height of the parent view.
	CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
	
	//Get the width of the parent view,
	CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
	
	//set up navigation bar
	optionsNavBar = [[UINavigationBar alloc] init];
	
	CGFloat navbarHeight = 44.0f;//[optionsNavBar frame].size.height;
	
	[optionsNavBar setFrame:CGRectMake(0.0f, 0.0f, rootViewWidth, navbarHeight)];
	
	[optionsNavBar setDelegate:self];
	//optionsNavBar.barStyle = UIBarStyleBlack;
	//optionsNavBar.translucent = TRUE;
	
	UINavigationItem *doneItem = [[UINavigationItem alloc] init];
	doneItem.title = @"TestProject";
	//doneItem.title = [NSString stringWithFormat:@"TestProject %dpage", [g_commonData GetRealPage]];
	//doneItem.hidesBackButton = FALSE;
	//[doneItem setHidesBackButton:FALSE animated:YES];
	
	UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStyleBordered target:self action:@selector(BackButtonClicked:)];
	[doneItem setLeftBarButtonItem:backButton animated:YES];
	[backButton release];
	
	UIBarButtonItem *mainButton = [[UIBarButtonItem alloc] initWithTitle:@"Main" style:UIBarButtonItemStyleBordered target:self action:@selector(MainButtonClicked:)];
	[doneItem setRightBarButtonItem:mainButton animated:YES];
	[mainButton release];
	
	//NSArray * array = [[NSArray alloc] initWithObjects:backButton, mainButton, nil];
	//[doneItem setItems:array animated:YES];
	
	[optionsNavBar pushNavigationItem:doneItem animated:YES];
	[doneItem release];
	
	
	//UIBarButtonItem* sndButton = 
	//[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemPlay target:self action:@selector(SndClicked:)];
	
	
	//[optionsNavBar pushNavigationItem:sndButton animated:YES];
	//[sndButton release];
	
	[[[CCDirector sharedDirector]openGLView]addSubview:optionsNavBar];
	
	//set up toolbar
	
	g_toolBar = [[UIToolbar alloc] init];
	[g_toolBar setDelegate:self];
	g_toolBar.barStyle = UIBarStyleBlack;
	g_toolBar.translucent = TRUE;
	[g_toolBar sizeToFit];
	
	//Caclulate the height of the toolbar
	CGFloat toolbarHeight = [g_toolBar frame].size.height;
	
	//Create a rectangle for the toolbar
	CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
	//Reposition and resize the receiver
	[g_toolBar setFrame:rectArea];
	
	// *bottomItem = [[UINavigationItem alloc] init];
	//bottomItem.title = @"TestProject";
	
	//CCLabel* label = [CCLabel labelWithString:@"TestLand" fontName:@"Marker Felt" fontSize:64];
	//label.position = rectArea.origin;
	//label.position = ccp(label.position.x + 100, label.position.y);
	//[self addChild:label];
	
	UIBarButtonItem* addButton = 
	[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(MainButtonClicked:)];
	//addButton.origin.x = 100;
	
	UIBarButtonItem* bookmarksButton = 
	[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemBookmarks target:self action:@selector(MainButtonClicked:)];
	//bookmarksButton.origin.x = 200;
	
	UIBarButtonItem *enkoButton = [[UIBarButtonItem alloc] initWithTitle:@"  En  /  Ko  " style:UIBarButtonItemStyleBordered target:self action:@selector(EnKoClicked:)];
	//UIImage *image = [UIImage imageNamed:@"Snd.png"];
	//UIBarButtonItem *sndButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStyleBordered target:self action:@selector(SndClicked:)];
	
	UIBarButtonItem* fixedSpace = 
	[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:self action:@selector(SndClicked:)];

	UIBarButtonItem* sndButton = 
	[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemPlay target:self action:@selector(SndClicked:)];
	
	[g_toolBar setItems:[NSArray arrayWithObjects:addButton,bookmarksButton/*,enkoButton*/,fixedSpace,sndButton,nil]];
	
	//addButton.width = 200;
		
	[view addSubview:g_toolBar];
	
	// Initialize
	rectArea.origin.x = rootViewWidth/2;
	rectArea.size.width = rootViewWidth/4;
	g_slider = [ [ UISlider alloc ] initWithFrame: rectArea ];
	g_slider.minimumValue = 0.0;
	g_slider.maximumValue = MaxPage;
	g_slider.tag = 0;
	g_slider.value = g_commonData.currentPage;
	g_slider.continuous = YES;
	//[aSlider SetState: aSlider.state | UISlider.showValue];
	[g_slider addTarget:self action:@selector(sliderAction:) forControlEvents:UIControlEventValueChanged];
	
	[view addSubview:g_slider];
	
	rectArea.origin.x = rootViewWidth/4;
	rectArea.size.width = rootViewWidth/4;
	pageLabel = [ [UILabel alloc ] initWithFrame:rectArea ];
	pageLabel.textAlignment =  UITextAlignmentCenter;
	pageLabel.textColor = [UIColor whiteColor];
	pageLabel.backgroundColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.0];
	pageLabel.font = [UIFont fontWithName:@"Arial Rounded MT Bold" size:(36.0)];
	[view addSubview:pageLabel];
	pageLabel.text = @"page";
	
	
	[self SetMenuPage];
	
	//UIBarButtonItem *sliderAsToolbarItem = [[UIBarButtonItem alloc] initWithCustomView:aSlider];
	//sliderAsToolbarItem.action = @selector(MainButtonClicked:);
	//sliderAsToolbarItem.target = self;
	
	// Set the width of aSlider
	//[sliderAsToolbarItem setWidth:250.0];	
	
	//UIMenuItem * menuItem = [[UIMenuItem alloc] initWithTitle:@"Test" action:@selector(outItemCallback:)];
	//UIMenuController *menuCont = [UIMenuController sharedMenuController];
	//[menuCont setTargetRect:view.frame inView:view.superview];
	//menuCont.arrowDirection = UIMenuControllerArrowLeft;
	//menuCont.menuItems = [NSArray arrayWithObject:menuItem];
	//[menuCont setMenuVisible:YES animated:YES];
	
	//if(bScheduleHideMenu == FALSE)
	//{
	//	bScheduleHideMenu = TRUE;
	
	//}
	[self RescheduleHideMenu];
}

-(void) RescheduleHideMenu
{
	[[CCScheduler sharedScheduler] unscheduleSelector:@selector(hideMenuCallback:) forTarget:self];
	[self schedule: @selector(hideMenuCallback:) interval:3];
}

-(void) OnHideMenu
{
	if( optionsNavBar != nil )
	{
		[optionsNavBar removeFromSuperview];
		[optionsNavBar release];
		optionsNavBar = nil;
		
	}
	if( g_toolBar != nil )		
	{
		[g_toolBar removeFromSuperview];
		[g_toolBar release];
		g_toolBar = nil;
	}
	
	if( g_slider != nil )
	{
		[g_slider removeFromSuperview];
		[g_slider release];
		g_slider = nil;
	}
	
	if( pageLabel != nil )
	{
		
		[pageLabel removeFromSuperview];
		[pageLabel release];
		pageLabel = nil;
	}
}

+(id) scene
{
	// 'scene' is an autorelease object.
	CCScene *scene = [CCScene node];
	
	// 'layer' is an autorelease object.
	TestBookScene *layer = [TestBookScene node];
	
	// add layer as a child to scene
	[scene addChild: layer];
	
	// return the scene
	return scene;
}

- (void)ChangeLanguage
{
	g_commonData.bKorLang = !g_commonData.bKorLang;
	
	[[CCDirector sharedDirector] replaceScene:
		[CCFlipYTransition transitionWithDuration:2.0f scene:[TestBookScene scene]]];
}

- (void)SetPage:(int)page IsSlide:(bool) bSlide
{
	if( g_commonData.currentPage!= page && page >= 0 && page <= MaxPage )
	{
		[[CCDirector sharedDirector] setDepthTest:TRUE];
		BOOL bBack = g_commonData.currentPage > page;
		g_commonData.currentPage = page;
		//if(bSlide)
		{
			if(bBack)
			{
				[[CCDirector sharedDirector] replaceScene:
				 [CCSlideInLTransition transitionWithDuration:1.0f scene:[TestBookScene scene]]];
			}
			else 
			{
				[[CCDirector sharedDirector] replaceScene:
				 [CCSlideInRTransition transitionWithDuration:1.0f scene:[TestBookScene scene]]];
			}

		}
		//else 
		//{
		//	[[CCDirector sharedDirector] replaceScene:
		//	 [CCPageTurnTransition transitionWithDuration:1.0f scene:[TestBookScene scene] backwards:bBack]];
		//}
	}
	else 
	{
		[self OnMenu];
	}

}

- (bool)IsInCenter:(CGPoint)currentPoint
{
	// ask director the the window size
	CGSize winSize = [[CCDirector sharedDirector] winSize];
	if( winSize.width - currentPoint.x > winSize.width / 4 &&
	   winSize.width - currentPoint.x < winSize.width / 2 + winSize.width / 4 &&
	   winSize.height - currentPoint.y > winSize.height / 4 &&
	   winSize.height - currentPoint.y < winSize.height / 2 + winSize.height / 4
	   )
		return TRUE;
	
	return FALSE;
}

- (bool)IsRightButtonPos:(CGPoint)currentPoint
{
	// ask director the the window size
	CGSize winSize = [[CCDirector sharedDirector] winSize];
	if( winSize.width - currentPoint.x < winSize.width / 4
	   //&& winSize.height - currentPoint.y > winSize.height / 2 - winSize.height / 4
	   //&& winSize.height - currentPoint.y < winSize.height / 2 + winSize.height / 4 
	   )
		return TRUE;
	
	return FALSE;
}


- (bool)IsLeftButtonPos:(CGPoint)currentPoint
{
	// ask director the the window size
	CGSize winSize = [[CCDirector sharedDirector] winSize];
	if( winSize.width - currentPoint.x > winSize.width / 2 + winSize.width / 4
	   //&& winSize.height - currentPoint.y > winSize.height / 2 - winSize.height / 4
	   //&& winSize.height - currentPoint.y < winSize.height / 2 + winSize.height / 4 
	   )
		return TRUE;
	
	return FALSE;
}


CGPoint lastPoint;

- (void)ccTouchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];
	
	lastPoint = [touch locationInView:[[CCDirector sharedDirector]openGLView]];
}

- (void)ccTouchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	
	UITouch *touch = [touches anyObject];
	
	CGPoint currentPoint = [touch locationInView:[[CCDirector sharedDirector]openGLView]];
	
	if( lastPoint.x < currentPoint.x )
		[self SetPage:g_commonData.currentPage - 1 IsSlide:TRUE];
	else
		[self SetPage:g_commonData.currentPage + 1 IsSlide:TRUE];
}
- (void)ccTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{	
	UITouch *touch = [touches anyObject];
	
	CGPoint currentPoint = [touch locationInView:[[CCDirector sharedDirector]openGLView]];
	
	NSString * TestPad = [g_commonData CheckTestPadName:currentPoint];
	
	if( TestPad != nil )
	{
		[self numItemCallback:0];
	}
	else if( [self IsLeftButtonPos:currentPoint] )
	{
		[self SetPage:g_commonData.currentPage - 1 IsSlide:FALSE];
	}
	else if( [self IsRightButtonPos:currentPoint] )
	{	
		[self SetPage:g_commonData.currentPage + 1 IsSlide:FALSE];
	}
	//else if ( [self IsInCenter:currentPoint] )
	//{
	//	[self OnHideMenu];
	//}
	else
	{
		[self OnMenu];
		//[self schedule: @selector(hideMenuCallback:) interval:5];
	}

	
}

-(void) onEnter
{
	[super onEnter];
	
	//[self OnMenu];
	
}

-(void)onExit
{
	[super onExit];
	[self OnHideMenu];
	
	
	[[CCDirector sharedDirector] setDepthTest:FALSE];
}

-(CCSprite*)CreateEffect:(NSString*) fileName withPosition:(CGPoint) position
{
	
	CCSprite* event1 = [CCSprite spriteWithFile:fileName];
	[self addChild: event1 z:kTagMenu tag:kTagMenu];
	event1.position = position;
	
	CCBlink* blink = [CCBlink actionWithDuration:7.0f blinks:10];
	CCFadeOut* fadeout = [CCFadeOut actionWithDuration:1.0f];
	
	CCSequence* actions = [CCSequence actionOne:blink two:fadeout];
	
	[event1 runAction:actions];
	
	return event1;
}

CCSprite* g_effect1 = nil;
CCSprite* g_effect2 = nil;

-(void)initPage
{
	g_bgPage = [CCSprite spriteWithFile:[g_commonData GetPageName]];
	//CCSprite *TestSprite = [CCSprite spriteWithFile:@"han_01.png"];
	
	// ask director the the window size
	CGSize size = [[CCDirector sharedDirector] winSize];
	
	g_bgPage.position = ccp( size.width /2 , size.height/2 );
	//bgSprite.rotation = 90;
	
	[self addChild: g_bgPage z:kTagPage tag:kTagPage];
	//[self addChild: TestSprite z:kTagSpriteSheet tag:kTagSpriteSheet];
	
	
	isTouchEnabled = YES;	
	
	BOOL bKor = g_commonData.bKorLang;
			
}

-(void)releasePage
{
	//[[CCScheduler sharedScheduler] unscheduleAllSelectorsForTarget:self];
	if(g_effect1 != nil)
	{
		[self removeChild:g_effect1 cleanup:TRUE];
		g_effect1 = nil;
	}
	if(g_effect2 != nil)
	{
		[self removeChild:g_effect2 cleanup:TRUE];
		g_effect2 = nil;
	}
	[self removeChild:g_bgPage cleanup:TRUE];
	//[g_bgPage release];
	g_bgPage = nil;
}

// on "init" you need to initialize your instance
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init] )) {
		
		// create and initialize a Label
		//CCLabel* label = [CCLabel labelWithString:@"TestLand" fontName:@"Marker Felt" fontSize:64];
		
		//[self addChild: label z:kTagMenu tag:kTagMenu];
		
		// ask director the the window size
		//CGSize size = [[CCDirector sharedDirector] winSize];
		// position the label on the center of the screen
		//label.position =  ccp( size.width /2 , size.height/2 );
		
		//[label runAction:[CCFadeIn actionWithDuration:3.0f]];
		//[label runAction:[CCFadeOut actionWithDuration:3.0f]];
		
		[self initPage];
	}
	return self;
}


- (IBAction) sliderAction: (UISlider*) sender 
{
	[self RescheduleHideMenu];
	g_commonData.currentPage = (int)sender.value;
	
	//[[CCDirector sharedDirector] replaceScene:[CCPageTurnTransition transitionWithDuration:1.0f scene:[TestBookScene scene]]];
	[self releasePage];
	[self initPage];
	
	[self SetMenuPage];
	
	//NSLog(@"outItemCallback버튼이눌려졌음.");
	//http://www.xprogress.com/post-35-uislider-tutorial-example-how-to-use-slider-in-iphone-sdk-xcode/
}

- (void) EnKoClicked: (id) sender
{
	[self RescheduleHideMenu];
	[self ChangeLanguage];
}

- (void) SndClicked: (id) sender
{
	[[SimpleAudioEngine sharedEngine] playEffect:@"1-Page.mp3"];//play a sound
}

- (void) hideMenuCallback: (id) sender 
{
	//bScheduleHideMenu = FALSE;
	[self OnHideMenu];
}

- (void) numItemCallback: (id) sender 
{
	[[CCDirector sharedDirector] pushScene:[CCCrossFadeTransition transitionWithDuration:1.0f scene: [TestPadScene scene]]];
}

- (void) BackButtonClicked: (id) sender 
{
	[[CCDirector sharedDirector] popScene];
	//NSLog(@"outItemCallback버튼이눌려졌음.");
}


- (void) MainButtonClicked: (id) sender 
{
	[[CCDirector sharedDirector] popScene];
	[[CCDirector sharedDirector] popScene];
	//NSLog(@"outItemCallback버튼이눌려졌음.");
}


- (void) outItemCallback: (id) sender 
{
	[[CCDirector sharedDirector] popScene];
	//NSLog(@"outItemCallback버튼이눌려졌음."); 
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
