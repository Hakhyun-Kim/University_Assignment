//
//  BookShelfScene.m
//  TestView
//
//  Created by harry Kim April 2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "BookShelfScene.h"
#import "TestBookScene.h"

@implementation BookShelfScene

+(id) scene
{	
	// return the scene
	BookShelfScene* scene = [BookShelfScene node];
	return scene;
}

- (id) init { if( (self=[super init]) ) {
	// 배경 이미지를 표시하기 위해 Sprite를 이용합니다. 
	CCSprite *bgSprite = [CCSprite spriteWithFile:@"bg_book.png"];
	bgSprite.anchorPoint = CGPointZero;
	// ccp는 CGPointMake 
	[bgSprite setPosition: ccp(0, 0)];
	// MenuScene에 배경 Sprite를 Child로 넣습니다. z-index는 0으로 설정합니다. // 위에 선언된 enum을 참고하세요. 모든 CCNode는 tag를 가질 수 있습니다. // tag 값을 주게되면 로컬변수를 사용하여 만들어진 // CCNode를 -(CCNode*) getChildByTag:(int) aTag 메소드를 이용하여 찾을 수 있습니다. // 우리는 retain된 변수를 사용하므로 이 예제에서는 getChildByTag 메소드를 사용할 일이 // 없습니다.
	[self addChild:bgSprite z:0 tag:0];
	
	[CCMenuItemFont setFontSize:30];
	[CCMenuItemFont setFontName:@"Courier New"];
	
	// 메뉴 버튼을 만듭니다. // itemFromNormalImage는 버튼이 눌려지기 전에 보여지는 이미지이고, // selectedImage는 버튼이 눌려졌을 때 보여지는 이미지입니다. // target을 self로 한 것은 버튼이 눌려졌을 때 발생하는 터치 이벤트를 MeneScene에서 // 처리를 하겠다는 것입니다. // @selector를 이용하여 버튼이 눌려졌을 때 어떤 메소드에서 처리를 할 것인지 결정합니다. 
	CCMenuItem* bookItem = [CCMenuItemImage itemFromNormalImage:@"book1.png"
													selectedImage:@"book1.png"
													disabledImage:@"book1.png"
														   target:self
														 selector:@selector(bookItemCallback:)];
	
	CCMenuItem* Test1land = [CCMenuItemFont itemFromString:@"TestLand" target:self selector:@selector(Test1landItemCallback:)];
	
	CCMenu *menu = [CCMenu menuWithItems: bookItem, /*Test1land,*/
					//self.parkItem, self.libraryItem, self.shopItem, 
					nil];
	Test1land.position = ccp(180,120);
	bookItem.position = ccp(0,0);
	menu.position = ccp(54+80,1024-95-183/*768-78*/);
	
	[self addChild:menu z:1 tag:1];
	} 
	return self;
}


// 메뉴 아이템(버튼)을 만들 때 이벤트 핸들러로 등록된 메소드를 만듭니다. // 이 예제에서는 콘솔(Console)에 메세지를 보여주는 것으로 대신합니다. 
- (void) bookItemCallback: (id) sender 
{
	[[CCDirector sharedDirector] pushScene:[CCFadeTransition transitionWithDuration:1.0f scene:[TestBookScene scene]]];
	
	//[[CCDirector sharedDirector] pushScene: [TestBookScene scene]];
}
							  
- (void) Test1landItemCallback: (id) sender 
{
	[[CCDirector sharedDirector] popScene];
}

- (void) dealloc 
{
	
	[super dealloc];
} 


@end
