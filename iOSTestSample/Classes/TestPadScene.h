//
//  HelloWorldLayer.h
//  TestView
//
//  Created by harry Kim April 2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


// When you import this file, you import all the cocos2d classes
#import "cocos2d.h"


// HelloWorld Layer
@interface TestPadScene : CCLayer
{
	UIImageView *drawImage;
	int mouseMoved;
	BOOL mouseSwiped;
	CGPoint lastPoint;
}

// returns a Scene that contains the HelloWorld as the only child
+(id) scene;

@end
