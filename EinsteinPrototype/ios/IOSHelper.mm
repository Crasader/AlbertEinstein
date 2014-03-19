//
//  IOSHelper.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 19/03/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//


#include "IOSHelper.h"


void IOSHelper::nativeMessage(char * title,char * message)
{
    
    UIAlertView* themessage=[[UIAlertView alloc] initWithTitle:[NSString stringWithCString:title encoding:NSUTF8StringEncoding]
                                                    message:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]
                                                   delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
    
    [themessage show];
    
}

IOSHelper::IOSHelper() {
    // TODO Auto-generated constructor stub
    
}

IOSHelper::~IOSHelper() {
    // TODO Auto-generated destructor stub
}
