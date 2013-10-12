//
//  MenuScene.m
//  TestView
//
//  Created by harry Kim April 2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MenuScene.h"
#import "BookShelfScene.h"


@implementation MenuScene

//@synthesize parkItem, shopItem, schoolItem, libraryItem;


+(id) scene
{	
	// return the scene
	MenuScene* scene = [MenuScene node];
	return scene;
}


- (id) init { if( (self=[super init]) ) {
	// 배경 이미지를 표시하기 위해 Sprite를 이용합니다. 
	CCSprite *bgSprite = [CCSprite spriteWithFile:@"backscreen_ipad.png"];
	
	bgSprite.anchorPoint = CGPointZero;
	// ccp는 CGPointMake와 같습니다. 
	[bgSprite setPosition: ccp(0, 0)];
	// MenuScene에 배경 Sprite를 Child로 넣습니다. z-index는 0으로 설정합니다. // 위에 선언된 enum을 참고하세요. 모든 CCNode는 tag를 가질 수 있습니다. // tag 값을 주게되면 로컬변수를 사용하여 만들어진 // CCNode를 -(CCNode*) getChildByTag:(int) aTag 메소드를 이용하여 찾을 수 있습니다. // 우리는 retain된 변수를 사용하므로 이 예제에서는 getChildByTag 메소드를 사용할 일이 // 없습니다.
	[self addChild:bgSprite z:0 tag:0];
	
	//[CCMenuItemFont setFontSize:30];
	//[CCMenuItemFont setFontName:@"Courier New"];
	
	// 메뉴 버튼을 만듭니다. // itemFromNormalImage는 버튼이 눌려지기 전에 보여지는 이미지이고, // selectedImage는 버튼이 눌려졌을 때 보여지는 이미지입니다. // target을 self로 한 것은 버튼이 눌려졌을 때 발생하는 터치 이벤트를 MeneScene에서 // 처리를 하겠다는 것입니다. // @selector를 이용하여 버튼이 눌려졌을 때 어떤 메소드에서 처리를 할 것인지 결정합니다. 
	CCMenuItem* test1Item = [CCMenuItemImage itemFromNormalImage:@"test1_active.png"
												  selectedImage:@"test1_click.png"
													disabledImage:@"test1_no_active.png"
														 target:self
													   selector:@selector(schoolItemCallback:)];
	
	// 위에서 만들어지 각각의 메뉴 아이템들을 CCMenu에 넣습니다. 는 각각의 메뉴 버튼이 눌려졌을 때 발생하는 터치 이벤트를 핸들링하고
	// CCMenu	, // 메뉴 버튼들이 어떻게 표시될 것인 지 레이아웃 처리를 담당합니다. 
	//CCMenuItem* schoolItem = [CCMenuItemFont itemFromString:@"school" target:self selector:@selector(schoolItemCallback:)];
	CCMenu *menu = [CCMenu menuWithItems: test1Item, test1Item, riceItem,test4Item,
					//self.parkItem, self.libraryItem, self.shopItem, 
					nil];
	
	menu.position = ccp(0,0);
	test1Item.position = ccp(260+50/2,1024-214-50/2);
	test1Item.position = ccp(229+50/2,1024-770-50/2);

	[test1Item setIsEnabled:FALSE];
	[test1Item setIsEnabled:FALSE];
	
	[self addChild:menu z:1 tag:1];
    } 
	return self;
}


// 메뉴 아이템(버튼)을 만들 때 이벤트 핸들러로 등록된 메소드를 만듭니다. // 이 예제에서는 콘솔(Console)에 메세지를 보여주는 것으로 대신합니다. 
- (void) schoolItemCallback: (id) sender 
{
	//[[CCDirector sharedDirector] pushScene: [BookShelfScene scene]];	
	
	[[CCDirector sharedDirector] pushScene:[CCFadeTransition transitionWithDuration:1.0f scene:[BookShelfScene scene]]];
	
}

- (void) dealloc 
{
	
	[super dealloc];
} 

@end
