//
//  AppDelegate.h
//  
//
//  Created by harry Kim April 2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>


//@class Paint_AppViewController;

@interface TestProjectAppDelegate : NSObject <UIApplicationDelegate> {
	UIWindow *window;
	
    //Paint_AppViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
//@property (nonatomic, retain) Paint_AppViewController *viewController;

//+(UIWindow*) getwindow;

@end
