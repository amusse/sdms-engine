//
//  HealthHeader.h
//  iHealthDemoCode
//
//  Created by zhiwei jing on 1/25/16.
//  Copyright © 2016 zhiwei jing. All rights reserved.
//

#ifndef HealthHeader_h
#define HealthHeader_h

typedef enum {
    //AM
    HealthDeviceType_AM3,
    HealthDeviceType_AM3S,
    HealthDeviceType_AM4,
    //BP
    HealthDeviceType_BP3L,
    
    HealthDeviceType_BP7S,
    HealthDeviceType_KN550BT,
    HealthDeviceType_KD926,
    HealthDeviceType_KD723,
    HealthDeviceType_ABPM,
    //PO
    HealthDeviceType_PO3,
    //HS
    HealthDeviceType_HS4,
    //ECG
    HealthDeviceType_ECG3,
    //BG
    HealthDeviceType_BG5L,
    //HTS
    HealthDeviceType_HTS,
    HealthDeviceType_CONTINUA_BP,
    HealthDeviceType_CONTINUA_BG,
    HealthDeviceType_CONTINUA_PO,
    HealthDeviceType_CONTINUA_HS
}HealthDeviceType;

#import "ScanDeviceController.h"
#import "ConnectDeviceController.h"


#endif /* HealthHeader_h */
