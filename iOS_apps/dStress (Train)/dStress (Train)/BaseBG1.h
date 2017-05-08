//
//  BaseBG1.h
//  iHealthBGCompatible
//
//  Created by XuJianbo on 14-12-9.
//  Copyright (c) 2014年 andon. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BaseBG1 : NSObject

- (NSString*)deviceID;
#pragma mark 获取BG1的固件版本号
-(NSString*)getBG1FirmwareVersion;
@end
