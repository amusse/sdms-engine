//
//  ABPMController.h
//  MedicaApp
//
//  Created by zhiwei jing on 9/21/15.
//  Copyright (c) 2015 apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ABPMController : NSObject{
    
    NSMutableArray *abpmArray;
}
/**
 * Initialize ABPM controller class.
 */
+(ABPMController *)shareABPMController;

/**
 * Get all ABPM instance,Access control class instance after receiving ABPMConnectNoti, then use instance to call ABPM related communication methods.
 */
-(NSArray *)getAllCurrentABPMInstace;

/**
 * Restart search ABPM
 */
-(void)startSearchABPM;

/**
 * Stop search ABPM
 */
-(void)stopSearchABPM;


@end
