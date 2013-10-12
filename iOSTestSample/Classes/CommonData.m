//
//  CommonData.m
//  TestProject
//
//  Created by harry Kim April 2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "CommonData.h"


@implementation CommonData

@synthesize currentPage,bKorLang,TestPadId;


const int StartPage = 7;

+ (void) Init
{
	g_commonData = [CommonData alloc];
	[g_commonData Create];
}

+ (void) DeInit
{
	[g_commonData release];
}

-(void)Create
{
	currentPage = 0;
	bKorLang = TRUE;
	TestPadId = 0;
	TestPadName = nil;
}
- (int) GetRealPage
{
	return currentPage + StartPage;
}
- (NSString*) GetPageName
{	
	if(bKorLang)
		return [NSString stringWithFormat:@"page%.2d_kr.png", StartPage+currentPage];
	else
		return [NSString stringWithFormat:@"page%.2d_en.png", StartPage+currentPage];
	
}

- (NSString*) GetTestPadName
{
	return TestPadName;
}

- (NSString*) CheckTestPadName:(CGPoint) currentPoint
{
	
	
	return nil;	
}




@end
