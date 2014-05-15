//
//  UIDelegateBridge.mm
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#import "UIWebViewBridge.h"
#import "EAGLView.h"


@implementation UIWebViewBridge
-(id)init
{
    isActive = true;
    mustDimissKeyboard = false;
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    return self;

}

-(void)dealloc
{
    [mView CleanUp];
    [mBackButton release];
    [mToolbar release];
	[mWebView release];
	[mView release];
    [super dealloc];
}

-(void) SetCheckinID:(const char*)IdToCheckin
{
    self->IdToCheckin = IdToCheckin;
}

-(void)keyboardWasShown:(NSNotification*)aNotification
{
 
    //[[EAGLView sharedEGLView]  endEditing:YES];
    // Locate non-UIWindow.
    UIWindow *keyboardWindow = nil;
    for (UIWindow *testWindow in [[UIApplication sharedApplication] windows])
    {
        if (![[testWindow class] isEqual:[UIWindow class]])
        {
            keyboardWindow = testWindow;
            break;
        }
    }

    // Locate UIWebFormView.
    for (UIView *formView in [keyboardWindow subviews]) {
        // iOS 5 sticks the UIWebFormView inside a UIPeripheralHostView.
        if ([[formView description] rangeOfString:@"UIPeripheralHostView"].location != NSNotFound)
        {

            for (UIView *subView in [formView subviews])
            {
                if ([[subView description] rangeOfString:@"UIWebFormAccessory"].location != NSNotFound)
                    [subView setUserInteractionEnabled:false];
            }
        }
     }
 
}

-(bool) setUnitID : (int )newKey
{
    self->newKey = newKey;
}



//Carrega paginas HTMLS
- (void) LoadRequest
{
    switch (newKey)
    {
        case 1:
        [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Av.+Albert+Einstein,+627%2F701+-+Morumbi+-+S%C3%A3o+Paulo&aq=&sll=-23.594804,-46.737303&sspn=0.006971,0.012467&ie=UTF8&hq=&hnear=Av.+Albert+Einstein,+627+-+Morumbi,+S%C3%A3o+Paulo,+05652-000&t=m&z=14&iwloc=A"]]];
            break;
        case 2:
        [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Rua+Apiac%C3%A1s,+85+%E2%80%93+Perdizes+%E2%80%93+S%C3%A3o+Paulo+%E2%80%93+SP&aq=&sll=-23.570897,-46.644854&sspn=0.006972,0.012467&ie=UTF8&hq=&hnear=R.+Apiac%C3%A1s,+85+-+Perdizes,+S%C3%A3o+Paulo,+05017-020&t=m&z=14&iwloc=A"]]];
            
            break;
        case 3:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps/ms?msa=0&msid=217401198874215165552.0004d873da2a24017d15c&hl=pt&ie=UTF8&t=m&ll=-23.585061,-46.663664&spn=0,0&source=embed"]]];
            
            
            break;
        case 4:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Av.+Brasil,+953+%E2%80%93+Jardins+%E2%80%93+S%C3%A3o+Paulo+%E2%80%93+SP&aq=&sll=-23.59913,-46.715176&sspn=0.006971,0.012467&ie=UTF8&hq=&hnear=Av.+Brasil,+953+-+Jardim+S%C3%A3o+Paulo,+Americana+-+S%C3%A3o+Paulo,+13468-000&t=m&z=14&iwloc=A"]]];
            break;
        case 5:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com/maps/ms?msa=0&msid=217401198874215165552.0004d873a72116894adb7&hl=pt-BR&ie=UTF8&t=m&ll=-23.500703,-46.857621&spn=0,0&source=embed"]]];
            break;
        case 6:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Rua+Madre+Cabrini,+462+-+Vila+Mariana+-+S%C3%A3o+Paulo+-+SP&aq=&sll=-22.748999,-47.339142&sspn=0.05612,0.099735&vpsrc=0&ie=UTF8&hq=&hnear=R.+Madre+Cabrini,+462+-+Vila+Mariana,+S%C3%A3o+Paulo,+04020-001&t=m&z=14&ll=-23.59266,-46.638511"]]];
            
            break;
        case 7:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Av.+Prof.+Francisco+Morato,+4.293+-+Butant%C3%A3+-+S%C3%A3o+Paulo+-+SP&aq=&sll=-23.59266,-46.638511&sspn=0.006971,0.012467&ie=UTF8&hq=&hnear=Av.+Prof.+Francisco+Morato,+4293+-+Vila+S%C3%B4nia,+S%C3%A3o+Paulo,+05521-200&t=m&z=14&iwloc=A"]]];
            break;
        case 8:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Rua+Manoel+Ant%C3%B4nio+Pinto,+210+-+Vila+Andrade+-+S%C3%A3o+Paulo+-+SP&aq=&sll=-23.598992,-46.715019&sspn=0.006971,0.012467&ie=UTF8&hq=&hnear=R.+Manuel+Ant%C3%B4nio+Pinto,+210+-+Vila+Andrade,+S%C3%A3o+Paulo,+05663-020&t=m&z=14&iwloc=A"]]];
            
            break;
        case 9:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com/maps/ms?msid=217401198874215165552.0004d8741f31b6bb773cb&msa=0&ll=-23.56084,-46.660438&spn=0.030683,0.055532&iwloc=0004d8742367e4c5f5dfa"]]];
            break;
		case 10:
			[mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?q=Av+Magalh%C3%A3es+de+Castro+12.000,+Morumbi+%E2%80%93+S%C3%A3o+Paulo+%E2%80%93+SP&hl=pt-BR&ie=UTF8&sll=-22.546052,-48.635514&sspn=8.97272,11.348877&t=h&hnear=Av.+Magalh%C3%A3es+de+Castro,+12000+-+Morumbi,+S%C3%A3o+Paulo,+05502-001&z=17"]]];
			break;
        case 19:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=R.+Ruggero+Fasano,+7+-+Morumbi+&aq=&sll=-23.599543,-46.716249&sspn=0.006971,0.012467&vpsrc=0&doflg=ptk&ie=UTF8&hq=&hnear=R.+Ruggero+Fasano,+7+-+Morumbi,+S%C3%A3o+Paulo,+05653-120&ll=-23.599543,-46.716249&spn=0.006971,0.012467&t=m&z=14&iwloc=0004d8886e53ce64c284f"]]];
            break;
        case 51:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=R.+Ruggero+Fasano,+7+-+Morumbi+&aq=&sll=-23.599543,-46.716249&sspn=0.006971,0.012467&vpsrc=0&doflg=ptk&ie=UTF8&hq=&hnear=R.+Ruggero+Fasano,+7+-+Morumbi,+S%C3%A3o+Paulo,+05653-120&ll=-23.599543,-46.716249&spn=0.006971,0.012467&t=m&z=14&iwloc=0004d8886e53ce64c284f"]]];
            break;
        case 89:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://maps.google.com.br/maps?f=q&source=embed&hl=pt&geocode=&q=Av.+Albert+Einstein,+626+-+Morumbi+S%C3%A3o+Paulo,+05652-000,+Brazil&aq=&sll=-23.599248,-46.715831&sspn=0.006971,0.012467&g=Av.+Albert+Einstein,+760+-+Morumbi+S%C3%A3o+Paulo,+05652-000,+Brazil&ie=UTF8&hq=&hnear=Av.+Albert+Einstein,+626+-+Morumbi,+S%C3%A3o+Paulo,+05652-000&ll=-23.599248,-46.715831&spn=0.006971,0.012467&t=m&z=14&iwloc=0004d888489a4b93d60ac"]]];
 
            break;
        case 98:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://www.facebook.com/dialog/feed?%20app_id=495531450500874&link=https://developers.facebook.com/docs/reference/dialogs/&name=Esteve%20em&%20caption=Checkout&description=Estevem%20no%20dia%20tal%20tlalala%20&redirect_uri=https://www.facebook.com/&display=touch"]]];
            break;
        case 99:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://foursquare.com/oauth2/authenticate?client_id=NVGS345UC4PRZAON3MB5EYKAC3Q0BURGQ35123Y1Y3K1CHQQ&response_type=token&redirect_uri=http://www.minhalistafacil.com.br"]]];
            break;
            
        case 100:
            [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.farofastudios.com.br/einstein/app.php"]]];
            break;
        default:
            break;
    }
}

-(void) setStartLayerWebView : (LayerWebView*) iLayerWebView
{
    mLayerWebView = iLayerWebView;

    
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 130, 320 , 352)];
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 320, 352)];
    int winsize = [[EAGLView sharedEGLView] getHeight];
    if (winsize > 480) {
        mView = [[UIView alloc] initWithFrame:CGRectMake(0, 140, 320 , 430)];
        mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 320, 430)];
    }
    //[mView setBackgroundColor:[UIColor redColor]];
    //[mView setContentMode:UIViewContentModeBottom];
    


   
    mWebView.delegate = self;
    [self LoadRequest];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(keyboardWasShown:)
                                          name:UIKeyboardDidShowNotification object:nil];
    [mView addSubview:mWebView];

}

-(void) setStartLayerWebViewWithURL : (BrowserLayer *)iLayerWebView andUrl:(char*) url
{
  
    int winsize = [[EAGLView sharedEGLView] getHeight];
        mView = [[UIView alloc] initWithFrame:CGRectMake(0, 70, 320 , winsize-70)];
        mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 320, winsize-70)];

    mWebView.delegate = self;
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.google.com"]]];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWasShown:)
                                                 name:UIKeyboardDidShowNotification object:nil];
    [mView addSubview:mWebView];
    
}


-(BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    if ([[[request URL] absoluteString] rangeOfString:@"maps.google.com.br/help/maps/helloworld/mobile/iphone"].location != NSNotFound)
    {
        [self LoadRequest];
        return NO;
    }
    
    NSString *URLString = [[request URL] absoluteString];
    if ([[[request URL] absoluteString] rangeOfString:@"facebook"].location != NSNotFound)
    {
        if (([URLString rangeOfString:@"#_=_"].location != NSNotFound) &&([URLString rangeOfString:@"login.php"].location == NSNotFound))
        {
            [self CleanUp];
            return NO;
        }

        if ([URLString rangeOfString:@"post_id"].location != NSNotFound)
        {
            [self CleanUp];
            return NO;
        }
    }

    if ([[[request URL] absoluteString] rangeOfString:@"minhalistafacil"].location != NSNotFound)
    {
        if ([URLString rangeOfString:@"access_token="].location != NSNotFound)
        {
            NSString *accessToken = [[URLString componentsSeparatedByString:@"="] lastObject];
            NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
            [defaults setObject:accessToken forKey:@"access_token"];
            
            const char* token = [accessToken cString];
            LayerWebView::setSocialNetworkToken(FOURSQUARE,token);
            LayerWebView::checkInFourSquare(self->IdToCheckin);
            if(newKey == 99)
            {
                LayerWebView::setSocialNetworkToken(FOURSQUARE,token);
                LayerWebView::checkInFourSquare(self->IdToCheckin);
            }
        }
    }
    
    [mWebView endEditing:YES];
    [mView endEditing:YES];
    [[EAGLView sharedEGLView]  endEditing:YES];

    NSLog(@"  %@", [[request URL] absoluteString] );
 
    //mView.center = CGPointMake(160, 258);
    NSURL *nURL = [request URL];
    NSString* someString = [nURL absoluteString];
    NSRange isRange = [someString rangeOfString:@"einstein.br" options:NSCaseInsensitiveSearch];
    if(isRange.length > 0)
        return NO;
    
    mLayerWebView->showLoadingPage();
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView
{
    mLayerWebView->hideLoadingPage();
    [mView addSubview:mWebView];
    [[EAGLView sharedEGLView] addSubview:mView];
    
    if(newKey == 99)
    {
        NSString *URLString = [[thisWebView.request URL] absoluteString];
        if ([URLString rangeOfString:@"access_token="].location != NSNotFound)
        {
            NSString *accessToken = [[URLString componentsSeparatedByString:@"="] lastObject];
            const char* token = [accessToken cString];
            LayerWebView::setSocialNetworkToken(FOURSQUARE,token);
            LayerWebView::checkInFourSquare(IdToCheckin);
            NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
            [defaults setObject:accessToken forKey:@"access_token"];
        }
    }
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error
{
	if ([error code] != -999 && error != NULL)
    {
        UIAlertView *alert;
        if( newKey == 98)
        {
		 alert = [[UIAlertView alloc] initWithTitle:@"Erro de Rede" message:@"Erro ao tentar acessar o Facebook. Tente novamente mais tarde, quando voce tiver uma conexao de rede."
														   delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        }
        else if( newKey == 99)
        {
            alert = [[UIAlertView alloc] initWithTitle:@"Erro de Rede" message:@"Erro ao tentar acessar o FourSquare. Tente novamente mais tarde, quando voce tiver uma conexao de rede."
                                              delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        }
        else
        {
            alert = [[UIAlertView alloc] initWithTitle:@"Erro de Rede" message:@"Erro ao tentar acessar o Google Maps. Tente novamente mais tarde, quando voce tiver uma conexao de rede."
														   delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        }
        [alert show];
        [alert release];
        mLayerWebView->showErrorImage();
        
	}
}

-(bool)isKeyBoardInDisplay
{
    UIWindow *keyboardWindow = nil;
    for (UIWindow *testWindow in [[UIApplication sharedApplication] windows])
    {
        if (![[testWindow class] isEqual:[UIWindow class]])
        {
            keyboardWindow = testWindow;
            break;
        }
    }
    for (UIView *formView in [keyboardWindow subviews])
        if ([[formView description] rangeOfString:@"UIPeripheralHostView"].location != NSNotFound)
            return true;
    return false;
}

-(void) CleanUp{
 
     mLayerWebView->hideLoadingPage();
    [mWebView endEditing:YES];
    [mView endEditing:YES];
    [[EAGLView sharedEGLView]  endEditing:YES];
    
    isActive = false;
    [mWebView stopLoading];
    [mWebView loadHTMLString:@"" baseURL:nil];
     mWebView.delegate = nil;
 	[mToolbar removeFromSuperview];
	[mWebView removeFromSuperview];
	[mView removeFromSuperview];
}

-(void) backClicked:(id)sender {
    [mView CleanUp];
}
@end