//
//  RDDrawingSprite.m
//  DrawingSpriteExample
//
//  Created by robodude666 on 6/9/10.
//  Copyright robodude666 2010. All rights reserved.
//

#import "RDDrawingSprite.h"

@implementation RDDrawingSpriteLine

@synthesize startPoint;
@synthesize endPoint;

#pragma mark -
#pragma mark Create SpriteLine

+(id)createWithStartPoint:(CGPoint)start endPoint:(CGPoint)end
{
	return [[self alloc] initWithStartPoint:start endPoint:end];
}

-(id)initWithStartPoint:(CGPoint)start endPoint:(CGPoint)end
{
	self = [super init];
	if(self)
	{
		startPoint = start;
		endPoint = end;
	}
	
	return self;
}
@end

@implementation RDDrawingSpriteShape

@synthesize lineSegments;
@synthesize drawingStyle;
@synthesize circleShape;
@synthesize circleVerticies;
@synthesize circleVerticiesCount;

#pragma mark -
#pragma mark Create SpriteShape

+(id)createShapeWithStyle:(RDDrawingSpriteStyle*)style
{
	return [[self alloc] initShapeWithStyle:style];
}

-(id)initShapeWithStyle:(RDDrawingSpriteStyle*)style
{
	self = [super init];
	if(self)
	{
		drawingStyle = [style retain];
		lineSegments = [[NSMutableArray alloc] init];
		
		circleShape = NO;
		circleVerticies = nil;
		circleVerticiesCount = 0;
	}
	
	return self;
}

#pragma mark -
#pragma mark Misc. SpriteShape Methods

-(void)addSegment:(RDDrawingSpriteLine*)shape
{
	[lineSegments addObject:shape];		
}

-(void)convertToCircleShape:(GLfloat*)verticies count:(int)count
{
	circleShape = YES;
	
	circleVerticies = verticies;
	circleVerticiesCount = count;
}

-(void)removeAllLineShapes
{
	[lineSegments removeAllObjects];
}

-(void)setDrawingStyle:(RDDrawingSpriteStyle*)style
{
	if(drawingStyle)
		[drawingStyle release];
	
	drawingStyle = [style retain];
}

-(void)dealloc
{
	[drawingStyle release];
	
	[lineSegments removeAllObjects];
	[lineSegments release];
	
	
	if (circleVerticies)
		free(circleVerticies);

	[super dealloc];
}

@end



@implementation RDDrawingSpriteStyle

@synthesize lineColor;
@synthesize fillColor;
@synthesize lineThickness;
@synthesize lineAlpha;
@synthesize fillEnabled;

#pragma mark -
#pragma mark Create Style

+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor
{
	return [[[self alloc] initStyleWithThickness:thickness lineColor:linecolor alpha:alpha fillColor:fillcolor] autorelease];
}

+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha
{
	return [[[self alloc] initStyleWithThickness:thickness lineColor:linecolor alpha:alpha] autorelease];
}

+(id)styleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor
{
	return [[[self alloc] initStyleWithThickness:thickness lineColor:linecolor alpha:100.0f] autorelease];
}

+(id)styleWithThickness:(float)thickness
{
	return [[[self alloc] initStyleWithThickness:thickness lineColor:ccBLACK alpha:100.0f] autorelease];
}

-(id)initStyleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor
{

	self = [super init];
	if(self)
	{
		lineThickness = thickness;
		lineColor = linecolor;
		lineAlpha = alpha;
		fillColor = fillcolor;
		
		
		fillEnabled = YES;
	}
	
	return self;	
}

-(id)initStyleWithThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha
{
	
	self = [super init];
	if(self)
	{
		lineThickness = thickness;
		lineColor = linecolor;
		lineAlpha = alpha;
		
		fillEnabled = NO;
	}
	
	return self;	
}

#pragma mark -
#pragma mark Update Style's Fill Settings

-(void)setFillColor:(ccColor3B)color
{
	fillColor = color;
	fillEnabled = YES;
}

-(void)setFillEnabled:(BOOL)enable
{
	fillEnabled = enable;
}

@end



@implementation RDDrawingSprite

@synthesize boundingBox;

#pragma mark -
#pragma mark Create DrawingSprite

+(id)createDrawingSprite
{
	return [[[self alloc] initDrawingSprite] autorelease];	
}

-(id)initDrawingSprite
{
	self = [super init];
	if(self)
	{
		currentStyle = [[RDDrawingSpriteStyle styleWithThickness:3] retain];
		arrayOfShapes = [[NSMutableArray alloc] init];
	}
	
	return self;	
}

-(id)init
{
	NSAssert(NO, @"Please use +createDrawingSprite");
	return nil;
}

#pragma mark -
#pragma mark Configure DrawingSprite's Style
-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha fillColor:(ccColor3B)fillcolor
{
	if(currentStyle == nil)
	{
		currentStyle = [RDDrawingSpriteStyle styleWithThickness:thickness lineColor:linecolor alpha:alpha fillColor:fillcolor];
	}
	else
	{
		[currentStyle setLineThickness:thickness];
		[currentStyle setLineColor:linecolor];
		[currentStyle setLineAlpha:alpha];
		[currentStyle setFillColor:fillcolor];
		[currentStyle setFillEnabled:YES];
	}
}

-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor alpha:(float)alpha
{
	if(currentStyle == nil)
	{
		currentStyle = [RDDrawingSpriteStyle styleWithThickness:thickness lineColor:linecolor alpha:alpha];
	}
	else
	{
		[currentStyle setLineThickness:thickness];
		[currentStyle setLineColor:linecolor];
		[currentStyle setLineAlpha:alpha];
	}
}

-(void)setStyleThickness:(float)thickness lineColor:(ccColor3B)linecolor
{
	if(currentStyle == nil)
	{
			currentStyle = [RDDrawingSpriteStyle styleWithThickness:thickness lineColor:linecolor];
	}
	else
	{
		[currentStyle setLineThickness:thickness];
		[currentStyle setLineColor:linecolor];
	}
		   
}

-(void)setStyleThickness:(float)thickness
{
	if(currentStyle == nil)
	{
		currentStyle = [RDDrawingSpriteStyle styleWithThickness:thickness];
	}
	else
	{
		[currentStyle setLineThickness:thickness];
	}

}

-(void)setStyle:(RDDrawingSpriteStyle*)style
{
	if(currentStyle != nil)
		[currentStyle release];

	currentStyle = [style retain];
}

#pragma mark -
#pragma mark Manual Drawing Methods

-(void)startDrawing
{
	if(currentShape == nil)
	{
		currentShape = [RDDrawingSpriteShape createShapeWithStyle:currentStyle];
	}
}

-(void)endDrawing
{
	if(currentShape != nil)
	{
		[arrayOfShapes addObject:currentShape];
		[currentShape release];
		currentShape = nil;
		prevPoint = ccp(0,0);
		
		[self updateSizes];
	}
}

-(void)moveTo:(CGPoint)point
{
	prevPoint = point;
}

-(void)lineTo:(CGPoint)point
{
	[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:prevPoint endPoint:point] autorelease]];
	
	prevPoint = point;
}

-(void)clear
{
	[arrayOfShapes removeAllObjects];	
}

#pragma mark -
#pragma mark Draw a Line

-(void)drawLineFrom:(CGPoint)start to:(CGPoint)end withStyle:(RDDrawingSpriteStyle*)style
{
	[self startDrawing];
	[currentShape removeAllLineShapes];
	[currentShape setDrawingStyle:style];
	[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:start endPoint:end] autorelease]];	
	[self endDrawing];
}

-(void)drawLineFrom:(CGPoint)start to:(CGPoint)end
{
	[self drawLineFrom:start to:end withStyle:currentStyle];	
}

-(void)drawLineWithArray:(NSArray*)arrayOfPoints withStyle:(RDDrawingSpriteStyle*)style
{
	[self startDrawing];
	[currentShape removeAllLineShapes];
	[currentShape setDrawingStyle:style];
	
	CGPoint start;
	BOOL firstPoint = YES;
	
	for(id val in arrayOfPoints)
	{
			if([val isKindOfClass:[NSValue class]])
			{
				CGPoint end = [(NSValue*)val CGPointValue];
				
				if( end.x == 0 )
				{
					firstPoint = YES;
				}
				else 
				if(firstPoint)
				{
					start = end;
					firstPoint = NO;
				}
				else
				{
					[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:start endPoint:end] autorelease]];
					start = end;
					//firstPoint = YES;
				}

			}
	}
	
	[self endDrawing];
}

-(void)drawLineWithArray:(NSArray*)arrayOfPoints
{
	[self drawLineWithArray:arrayOfPoints withStyle:currentStyle];
}


-(void)drawLineWithCArray:(CGPoint*)arrayOfPoints count:(int)count withStyle:(RDDrawingSpriteStyle*)style
{

	NSMutableArray *array = [[[NSMutableArray alloc] initWithCapacity:count] autorelease];
	
	for(int i = 0; i < count; i++)
	{
		[array addObject:ccp2nsv(arrayOfPoints[i])];
	}
	
	[self drawLineWithArray:array withStyle:style];
	
}

-(void)drawLineWithCArray:(CGPoint*)arrayOfPoints count:(int)count
{
	[self drawLineWithCArray:arrayOfPoints count:count withStyle:currentStyle];
}


#pragma mark -
#pragma mark Draw Rectangle

-(void)drawRectangle:(CGRect)rect withStyle:(RDDrawingSpriteStyle *)style
{
	[self startDrawing];
	[currentShape removeAllLineShapes];
	[currentShape setDrawingStyle:style];
	
	float originX = rect.origin.x;
	float originY = rect.origin.y;	
	float originXW = originX + rect.size.width;
	float originYH = originY + rect.size.height;
	
	[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:ccp(originX, originY) endPoint:ccp(originXW, originY)] autorelease]];	
	[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:ccp(originXW, originY) endPoint:ccp(originXW, originYH)] autorelease]];	
	[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:ccp(originXW, originYH) endPoint:ccp(originX, originYH)] autorelease]];	
	
	if([style fillEnabled] != YES)
		[currentShape addSegment:[[RDDrawingSpriteLine createWithStartPoint:ccp(originX, originYH) endPoint:ccp(originX, originY)] autorelease]];	

	[self endDrawing];
}

-(void)drawRectangle:(CGRect)rect
{
	[self drawRectangle:rect withStyle:currentStyle];
}

#pragma mark -
#pragma mark Draw Ellipse & Circles

-(void)drawEllipse:(float)width height:(float)height center:(CGPoint)center withStyle:(RDDrawingSpriteStyle*)style
{
	#define SEGMENTS 60
		
	const float coef = 2.0f * M_PI / SEGMENTS;
	
	float *vertices = malloc(sizeof(float) * 2*(SEGMENTS+1));
	if(!vertices) return;
	
	memset(vertices, 0, sizeof(float) * 2*(SEGMENTS+1));
	
	int count = 0;
	for(int i = 0; i < (SEGMENTS+1); i++)
	{
		float rads = i * coef;		
		vertices[count++] = width/2 * cosf(rads) + center.x;
		vertices[count++] = height/2 * sinf(rads) + center.y;
	}
	
	
	
	[self startDrawing];
	[currentShape removeAllLineShapes];
	[currentShape setDrawingStyle:style];
	
	[currentShape convertToCircleShape:vertices count:(SEGMENTS+1) * 2];
	[self endDrawing];
}

-(void)drawEllipse:(float)width height:(float)height center:(CGPoint)center
{
	[self drawEllipse:width height:height center:center withStyle:currentStyle];
}

-(void)drawCircle:(float)radius center:(CGPoint)center withStyle:(RDDrawingSpriteStyle*)style
{
	[self drawEllipse:radius*2 height:radius*2 center:center withStyle:style];
}

-(void)drawCircle:(float)radius center:(CGPoint)center
{
	[self drawEllipse:radius*2 height:radius*2 center:center withStyle:currentStyle];
}

#pragma mark -
#pragma mark Private Draw Methods

-(void)draw
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	for(RDDrawingSpriteShape *shape in arrayOfShapes)
	{
		
		if([shape circleShape] == YES)
		{
			[self drawCircleShape:shape];
		}
		else
		{
			if([[shape drawingStyle] fillEnabled] == NO)
			{
				[self drawNoneFilledShape:shape];
			}
			else
			{
				[self drawFilledShape:shape];
			}
		}		
	}
	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

-(void)drawCircleShape:(RDDrawingSpriteShape*)shape
{
	RDDrawingSpriteStyle *style = [shape drawingStyle];
	ccColor3B fillColor = [style fillColor];
	ccColor3B lineColor = [style lineColor];	
	float lineAlpha = [style lineAlpha]/100.0f;
	
	glLineWidth([style lineThickness]);
	glVertexPointer (2, GL_FLOAT , 0, [shape circleVerticies]); 
	
	if([style fillEnabled] == YES)
	{
		glColor4ub(fillColor.r, fillColor.g, fillColor.b, lineAlpha * 255.0f);
		glDrawArrays (GL_TRIANGLE_FAN, 0, [shape circleVerticiesCount]/2);
	}
	
	glColor4ub(lineColor.r, lineColor.g, lineColor.b, lineAlpha * 255.0f);
	glDrawArrays(GL_LINE_STRIP, 0, [shape circleVerticiesCount]/2);
}

-(void)drawNoneFilledShape:(RDDrawingSpriteShape*)shape
{
	CGPoint vertices[2];
	
	RDDrawingSpriteStyle *style = [shape drawingStyle];
	
	for(RDDrawingSpriteLine *line in [shape lineSegments])
	{
		vertices[0] = ccp([line startPoint].x, [line startPoint].y);
		vertices[1] = ccp([line endPoint].x, [line endPoint].y);
		
		ccColor3B lineColor = [style lineColor];	
		
		float lineAlpha = [style lineAlpha] / 100.0f;
		lineColor.r *= lineAlpha;
		lineColor.g *= lineAlpha;
		lineColor.b *= lineAlpha;
				
		glColor4ub(lineColor.r, lineColor.g, lineColor.b, lineAlpha * 255); //line color		
		glLineWidth([style lineThickness]);
		
		glVertexPointer(2, GL_FLOAT, 0, vertices);	
		glDrawArrays(GL_LINES, 0, 2);		
	}	
}

-(void)drawFilledShape:(RDDrawingSpriteShape*)shape
{
	
	int vertexCount = [[shape lineSegments] count];
	CGPoint vertexPoints[vertexCount];
	int vertIndex = 0;
	BOOL firstPoint = YES;
	
	RDDrawingSpriteStyle *style = [shape drawingStyle];	
	ccColor3B fillColor = [style fillColor];
	ccColor3B lineColor = [style lineColor];	
	float lineAlpha = [style lineAlpha]/100.0f;
	
	fillColor.r *= lineAlpha;
	fillColor.g *= lineAlpha;
	fillColor.b *= lineAlpha;
	
	lineColor.r *= lineAlpha;
	lineColor.g *= lineAlpha;
	lineColor.b *= lineAlpha;
	
	for(RDDrawingSpriteLine *line in [shape lineSegments])
	{
		if(firstPoint)
		{
			vertexPoints[0] = ccp([line startPoint].x, [line startPoint].y);
			vertIndex++;
			firstPoint = NO;
		}
		
		vertexPoints[vertIndex] = ccp([line endPoint].x, [line endPoint].y);
		vertIndex++;
	}	
	
	glLineWidth([style lineThickness]);
	
	glVertexPointer(2, GL_FLOAT, 0, vertexPoints);
	
	glColor4ub(fillColor.r, fillColor.g, fillColor.b, lineAlpha * 255);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertIndex);
	
	glColor4ub(lineColor.r, lineColor.g, lineColor.b, (GLubyte)(lineAlpha * 255.0f));
	glDrawArrays(GL_LINE_LOOP, 0, vertIndex);
	
}

#pragma mark -
#pragma mark Misc. Methods

-(void)updateSizes
{

	CGPoint startPoint;
	CGPoint endPoint;
	
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	
	BOOL firstPoint = YES;
		
	for(RDDrawingSpriteShape *shape in arrayOfShapes)
	{

		if([shape circleShape] == YES)
		{

			float possibleMinX = [shape circleVerticies][60];
			float possibleMaxX = [shape circleVerticies][0];
			
			float possibleMinY = [shape circleVerticies][91];
			float possibleMaxY = [shape circleVerticies][31];
			
			if(possibleMinX < minX) minX = possibleMinX;
			if(possibleMaxX > maxX) maxX = possibleMaxX;
			
			if(possibleMinY < minY) minY = possibleMinY;
			if(possibleMaxY > maxY) maxY = possibleMaxY;	

		}
		else
		{

			for(RDDrawingSpriteLine *line in [shape lineSegments])
			{
				endPoint = ccp([line endPoint].x, [line endPoint].y);
				startPoint = ccp([line startPoint].x, [line startPoint].y); 
				
				if(firstPoint)
				{
					minX = startPoint.x;
					maxX = startPoint.x;
					minY = startPoint.y;
					minY = startPoint.y;
					
					firstPoint = NO;					
				}
				
				if(startPoint.x < minX) minX = startPoint.x;
				else if(startPoint.x > maxX) maxX = startPoint.x;
				else if(endPoint.x < minX) minX = endPoint.x;
				else if(endPoint.x > maxX) maxX = endPoint.x;
				
				if(startPoint.y < minY) minY = startPoint.y;
				else if(startPoint.y > maxY) maxY = startPoint.y;
				else if(endPoint.y < minY) minY = endPoint.y;
				else if(endPoint.y > maxY) maxY = endPoint.y;
			}			
		}		
	}
	
	
	float width = maxX - minX;
	float height = maxY - minY;
	
	[self setContentSize:CGSizeMake(width, height)];
	boundingBox = CGRectMake(minX, minY, width, height);
}



-(void)dealloc
{
	[currentShape release];
	[currentStyle release];
	
	[arrayOfShapes removeAllObjects];
	[arrayOfShapes release];

	[super dealloc];
}


@end