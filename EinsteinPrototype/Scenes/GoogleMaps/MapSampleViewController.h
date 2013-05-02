#import <GoogleMaps/GoogleMaps.h>
#import <UIKit/UIKit.h>

/**
 * MapSampleViewController is a simple UIViewController that describes a sample
 * (empty) Google Maps SDK for iOS demo. More complex demos may be built as
 * subclasses of this class.
 *
 * Note that the default loadView implementation just sets the whole view of
 * this class to mapView; i.e., self.view = self.mapView;.
 */
@interface MapSampleViewController : UIViewController <GMSMapViewDelegate>

/** GMSMapView managed by this controller. */
@property (nonatomic, readonly, strong) GMSMapView *mapView;

/** Updates with the display 'split view controller' button. */
- (void)updateSamplesButton:(UIBarButtonItem *)samplesButton;

/** Notes that describe the functionality of this Sample. */
+ (NSString *)notes;

@end
