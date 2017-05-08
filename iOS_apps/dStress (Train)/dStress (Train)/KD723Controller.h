//
//  KD723ViewController.h
//  testShareCommunication
//
//  Created by my on 14/10/13.
//  Copyright (c) 2013年 my. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KD723Controller : NSObject{
NSMutableArray *KD723DeviceArray;
}
/**
 * Initialize KD723 controller class
 */
+(KD723Controller *)shareKD723Controller;

/**
 * Get all KD723 instance,Access control class instance after receiving KD723ConnectNoti, then use instance to call KD723 related communication methods.
 */
-(NSArray *)getAllCurrentKD723Instace;

/**
 * Restart search KD723
 */
-(void)startSearchKD723;

/**
 * Stop search KD723
 */
-(void)stopSearchKD723;

@end
