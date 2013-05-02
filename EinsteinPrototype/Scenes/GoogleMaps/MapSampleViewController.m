#import "MapSampleViewController.h"

#import <GoogleMaps/GoogleMaps.h>

@interface MapSampleViewController ()

- (void)updateLeftButtons;

@end

#pragma mark - MyLocationObserver

// MyLocationObserver is a helper class to observe myLocation. It's required
// to be disjoint from the MapSampleViewController as subclasses may want to
// listen to myLocation on its own.
@interface MyLocationObserver : NSObject

- (id)initWithController:(MapSampleViewController *)controller;

@end

@implementation MyLocationObserver {
  __weak MapSampleViewController *controller_;
}

- (id)initWithController:(MapSampleViewController *)controller {
  if ((self = [super init])) {
    controller_ = controller;
  }
  return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context {
  [controller_ updateLeftButtons];
}

@end

#pragma mark - MapSampleViewController

@implementation MapSampleViewController {
  GMSMapView *mapView_;
  MyLocationObserver *observer_;
  UIBarButtonItem *samplesButton_;
}

- (id)init {
  if ((self = [super init])) {
    self.title = [[self class] description];
    observer_ = [[MyLocationObserver alloc] initWithController:self];
  }
  return self;
}

- (void)loadView {
  self.view = self.mapView;
}

- (void)dealloc {
  [self.mapView removeObserver:observer_ forKeyPath:@"myLocation"];
}

- (GMSMapView *)mapView {
  if (mapView_ == nil) {
    CGFloat zoom = 4;
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
      // If this is a phone, default the zoom to one lower - show a little more
      // of Oz.
      zoom -= 1;
    }

    // Create a default GMSMapView, showcasing Australia.
    mapView_ = [[GMSMapView alloc] initWithFrame:CGRectZero];
    mapView_.camera = [GMSCameraPosition cameraWithLatitude:-25.5605
                                                  longitude:133.605097
                                                       zoom:zoom];
    mapView_.delegate = self;
    [self.mapView addObserver:observer_
                   forKeyPath:@"myLocation"
                      options:NSKeyValueObservingOptionNew
                      context:NULL];
  }
  return mapView_;
}

// The default implementation of updateSamplesButton takes over the left-hand
// buttons on this controller's navigation bar.
- (void)updateSamplesButton:(UIBarButtonItem *)samplesButton {
  samplesButton_ = samplesButton;
  [self updateLeftButtons];
}

+ (NSString *)notes {
  return nil;
}

#pragma mark - Private methods

// Update the buttons on the left of this VC's UINavigationItem.
- (void)updateLeftButtons {
  NSMutableArray *buttons = [NSMutableArray array];

  if (samplesButton_) {
    [buttons addObject:samplesButton_];
  }

  if (mapView_.myLocation != nil) {
    UIBarButtonSystemItem myLocationItem = 100;  // "My Location"
    SEL action = @selector(didTapMyLocation);
    UIBarButtonItem *button =
        [[UIBarButtonItem alloc] initWithBarButtonSystemItem:myLocationItem
                                                      target:self
                                                      action:action];
    [buttons addObject:button];
  }

  self.navigationItem.leftItemsSupplementBackButton = YES;
  self.navigationItem.leftBarButtonItems = buttons;
}

// Animate to the user's location.
- (void)didTapMyLocation {
  [mapView_ animateToLocation:mapView_.myLocation.coordinate];
  [mapView_ animateToBearing:0];
  [mapView_ animateToViewingAngle:0];
}

@end
