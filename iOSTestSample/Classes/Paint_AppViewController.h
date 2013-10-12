//
//  Paint_AppViewController.h
//  Paint App
//
//  Created by albert renshaw on 4/12/10.
//  Copyright providence 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Paint_AppViewController : UIViewController {
	UIImageView *drawImage;
	int mouseMoved;
	BOOL mouseSwiped;
	CGPoint lastPoint;
}

@end

