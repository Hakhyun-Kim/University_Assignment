//
//  RDDrawingSprite.h
//  DrawingSpriteExample
//
//  Created by robodude666 on 6/9/10.
//  Copyright robodude666 2010. All rights reserved.
//

#import "cocos2d.h"


@class RDDrawingSpriteShape;
@class RDDrawingSpriteStyle;
@class RDDrawingSprite;


// Useful Macros

#define ccpasnsv(__x__, __y__ ) ([NSValue valueWithCGPoint:ccp(__x__, __y__)])
#define ccp2nsv(__ccp__) ([NSValue valueWithCGPoint:__ccp__])

@interface RDDrawingSpriteLine : NSObject
{
	
@protected
	CGPoint startPoint;
	CGPoint endPoint;
}

@property (nonatomic, assign) CGPoint startPoint;
@property (nonatomic, assign) CGPoint endPoint;

+(id)createWithStartPoint:(CGPoint)start endPoint:(CGPoint)end;
-(id)initWithStartPoint:(CGPoint)start endPoint:(CGPoint)end;

@end


@interface RDDrawingSpriteShape : NSObject
{
	
@protected
	NSMutableArray *lineSegments;
	
	RDDrawingSpriteStyle *drawingStyle;	

	BOOL circleShape;
	GLfloat *circleVerticies;
	int circleVerticiesCount;
}

@property (nonatomic, readonly) NSMutableArray* lineSegments;
@property (nonatomic, readonly) RDDrawingSpriteStyle* drawingStyle;
@property (nonatomic, readonly) BOOL circleShape;
@property (nonatomic, readonly) GLfloat *circleVerticies;
@property (nonatomic, readonly) int circleVerticiesCount;

+(id)createShapeWithStyle:(RDDrawingSpriteStyle*)style;
-(id)initShapeWithStyle:(RDDrawingSpriteStyle*)style;

-(void)addSegment:(RDDrawingSpriteLine*)shape;
-(void)convertToCircleShape:(GLfloat*)verticies count:(int)count;


-(void)removeAllLineShapes;
-(void)setDrawingStyle:(RDDrawingSpriteStyle*)style;

@end


@interface RDDrawingSpriteStyle : NSObject
{
	
@protected
	ccColor3B lineColor;
	ccColor3B fillColor;
	float lineThickness;
	float lineAlpha;
	
	BOOL fillEnabled;
}

@property (nonatomic, readwrite, assign) ccColor3B lineColor;
@property (nonatomic, readwrite, assign) ccColor3B fillColor;
@property (nonatomic, readwrite, assign) float lineThickness;
@property (nonatomic, readwrite, assign) float lineAlpha;
@property (nonatomic, readwrite, assign) BOOL fillEnabled;

+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor;
+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha;
+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor;
+(id)styleWithThickness:(float)thickness;

-(id)initStyleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor;
-(id)initStyleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha;

@end


@interface RDDrawingSprite : CCNode
{
	
@protected
	RDDrawingSpriteShape *currentShape;
	RDDrawingSpriteStyle *currentStyle;
	
	NSMutableArray *arrayOfShapes;
	
	CGPoint prevPoint;

	CGRect boundingBox;
}

@property (nonatomic, readonly, assign) CGRect boundingBox;

+(id)createDrawingSprite;
-(id)initDrawingSprite;

-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor;
-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha;
-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor;
-(void)setStyleThickness:(float)thickness;
-(void)setStyle:(RDDrawingSpriteStyle*)style;

-(void)startDrawing;
-(void)endDrawing;

-(void)clear;

-(void)updateSizes;

-(void)moveTo:(CGPoint)point;
-(void)lineTo:(CGPoint)point;

-(void)drawLineFrom:(CGPoint)start to:(CGPoint)end withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawLineFrom:(CGPoint)start to:(CGPoint)end;

-(void)drawLineWithArray:(NSArray*)arrayOfPoints withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawLineWithArray:(NSArray*)arrayOfPoints;

-(void)drawLineWithCArray:(CGPoint*)arrayOfPoints count:(int)count withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawLineWithCArray:(CGPoint*)arrayOfPoints count:(int)count;

-(void)drawRectangle:(CGRect)rect withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawRectangle:(CGRect)rect;

-(void)drawEllipse:(float)width height:(float)height center:(CGPoint)center withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawEllipse:(float)width height:(float)height center:(CGPoint)center;

-(void)drawCircle:(float)radius center:(CGPoint)center withStyle:(RDDrawingSpriteStyle*)style;
-(void)drawCircle:(float)radius center:(CGPoint)center;


// Private
-(void)drawCircleShape:(RDDrawingSpriteShape*)shape;
-(void)drawNoneFilledShape:(RDDrawingSpriteShape*)shape;
-(void)drawFilledShape:(RDDrawingSpriteShape*)shape;

@end