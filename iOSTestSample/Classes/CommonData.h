//
//  CommonData.h
//  TestProject
//
//  Created by harry Kim April 2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CommonData : NSObject {

@public
	
	int currentPage;
	bool bKorLang;
	int TestPadId;
	NSString* TestPadName;

}

@property int currentPage; 
@property bool bKorLang; 
@property int TestPadId; 

CommonData * g_commonData;

+(void)Init;
+(void)DeInit;

-(void)Create;
//-(void)Deinitazize

- (NSString*) GetPageName;
- (NSString*) CheckTestPadName:(CGPoint) currentPoint;
- (NSString*) GetTestPadName;

- (int) GetRealPage;

@end
