#import "MovieLayer.h"

@implementation MovieLayer

#pragma mark -
#pragma mark Init and Dealloc

bool bStart = false;

- (void) movieFinishCallback: (id) sender 
//- (void) movieFinished:(NSNotification*)notification
{
	[[CCDirector sharedDirector] popScene];
}

+ (id) scene
{
	return [self layerWithMovieName:@"test1test.mp4" type:@"" target:self selector:@selector(movieFinishCallback:)];
}

+ (id)layerWithMovieName:(NSString*)name type:(NSString*)movieType target:(id)target selector:(SEL)selector
{
	return [[[self alloc] initWithMovieName:name type:movieType target:target selector:selector] autorelease];
}

- (id)initWithMovieName:(NSString*)name type:(NSString*)movieType target:(id)target selector:(SEL)selector
{
	if ((self = [super init]) != nil)
	{  
		NSBundle *bundle = [NSBundle mainBundle];
		NSString *moviePath = [bundle pathForResource:name ofType:movieType];
		if (moviePath)
		{
			NSURL *moviePathURL = [NSURL fileURLWithPath:moviePath];
			[self loadMovieAtURL:moviePathURL target:target selector:selector];
			
			// Notification
			[[NSNotificationCenter defaultCenter] 
			 addObserver:target 
			 selector:selector
			 name:MPMoviePlayerPlaybackDidFinishNotification
			 object:nil];
			
			[self play];
		}  
	}
	return self;
}

- (void)dealloc
{
	if(bStart)
	{
		[theMovie.view removeFromSuperview];
		[theMovie release];
		[super dealloc];
		bStart = false;
	}
}

#pragma mark -
#pragma mark Class functions

- (void)loadMovieAtURL:(NSURL*)theURL target:(id)target selector:(SEL)selector
{
	CGSize size = [[CCDirector sharedDirector] winSize];
	
	theMovie = [[[MPMoviePlayerController alloc] initWithContentURL:theURL] retain];
	
	theMovie.scalingMode = MPMovieScalingModeAspectFill;
	//[theMovie setFullscreen:TRUE animated:TRUE];
	theMovie.movieControlMode = MPMovieControlModeHidden;
	
	theMovie.view.frame = CGRectMake(0, 0, size.width, size.height); 
	theMovie.view.backgroundColor = [UIColor clearColor];
	
	// Transform
	theMovie.view.transform = CGAffineTransformMakeRotation(-270.0f * (M_PI/180.0f));
	theMovie.view.center = [[CCDirector sharedDirector] openGLView].center;
	
	[[[CCDirector sharedDirector] openGLView] addSubview:theMovie.view]; 
}

- (void)play
{
	bStart = true;
	[theMovie play];
}
@end
