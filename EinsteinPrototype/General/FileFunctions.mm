//
//  FileFunctions.mm
//  EinsteinPrototype
//
//  Created by Camargo on 22/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "FileFunctions.h"
void FileFunctions::writeDay(std::string theDate)
{
    NSLog(@"%@:%s",@"Data",theDate.c_str());
    NSString * theStringDate = [NSString stringWithFormat:@"%s",theDate.c_str()];
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:theStringDate forKey:@"noticias"];
    [defaults synchronize];
}

std::string FileFunctions::loadDay()
{
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * theStringDate = [defaults objectForKey:@"noticias"];
    NSLog(@"%@:%@",@"Data lida",theStringDate);
    std::string result = "";
    
    if (theStringDate != nil) {
        result = [theStringDate cStringUsingEncoding:NSUTF8StringEncoding];
    }
    
    return result;
   
}