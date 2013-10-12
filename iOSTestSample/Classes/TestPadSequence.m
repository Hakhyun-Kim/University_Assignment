//
//  TestPadSequence.m
//  TestProject
//
//  Created by harry Kim April 2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TestPadSequence.h"
#import <UIKit/UIKit.h>
#import "CommonData.h"


@implementation TestPadSequence

enum {
	kTagPage = 0,
    kTagBackground = 1,
    kTagMenu = 2,
    kTagSpriteSheet = 3,
};

+(id) scene
{
	TestPadSequence* scene = [TestPadSequence node];
	
	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init] )) {
		
		// create and initialize a Label
		//CCLabel* label = [CCLabel labelWithString:@"testLand" fontName:@"Marker Felt" fontSize:64];
		
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
		CCMenuItem* outItem = [CCMenuItemImage itemFromNormalImage:@"btn_out_active.png"
													 selectedImage:@"btn_out_touch.png"
													 disabledImage:@"btn_out_no_active.png"
															target:self
														  selector:@selector(outItemCallback:)];
		
		CCMenu *menu = [CCMenu menuWithItems: /*numItem,*/ outItem, 
						nil];
		
		menu.position = ccp(0,0);
		outItem.position = ccp(476+135/2,1024-753-50/2);
		
		[self addChild:menu z:1 tag:1];
		
	}
	return self;
}

- (void) outItemCallback: (id) sender 
{
	[[CCDirector sharedDirector] popScene];
}


@end
