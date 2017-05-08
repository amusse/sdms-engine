//
//  POMacroFile.h
//  POSDK
//
//  Created by 小翼 on 14-8-11.
//  Copyright (c) 2014年 hejiasu. All rights reserved.
//

#import "HealthUser.h"

#ifndef POSDK_POMacroFile_h
#define POSDK_POMacroFile_h

#define PO3DeviceID @"ID"
#define PO3SDKRightApi  @"OpenApiSpO2"

#define PO3Discover       @"PO3Discover"
#define PO3ConnectFailed   @"PO3ConnectFailed"
#define PO3ConnectNoti @"PO3ConnectNoti"
#define PO3DisConnectNoti @"PO3DisConnectNoti"

typedef enum{
    PO3Error_OverTime  = 0,  // Bluetooth Communication Error
    PO3Error_ResetDeviceFaild,     // Send failed
    PO3Error_Disconnect,      // Device is disconnected
    PO3Error_ParameterError = 400,   //ParameterError
    PO3Error_UserInvalidateRight,    //User has not been authorized
    PO3Error_FirmwareVersionIsNotSupported //firmware version is not supported
}PO3ErrorID;




/*
 UserAuthen_RegisterSuccess: New-user registration succeeded.
 UserAuthen_LoginSuccess: User login succeeded.
 UserAuthen_CombinedSuccess: The user is an iHealth user as well, measurement via SDK has been activated, and the data from the measurement belongs to the user.
 UserAuthen_TrySuccess: Testing without internet connection succeeded.
 UserAuthen_InvalidateUserInfo: Userid/clientID/clientSecret verification failed.
 UserAuthen_SDKInvalidateRight: SDK has not been authorized.
 UserAuthen_UserInvalidateRight: User has not been authorized.
 UserAuthen_InternetError: Internet error, verification failed.
 The measurement via SDK will be operated in the case of 1-3, and will be terminated if any of 4-8 occurs. The interface needs to be re-called after analyzing the return parameters.
 Notice: when a new user registers via SDK, an ‘iHealth disclaimer’ will pop up automatically, and will require the user to agree in order to continue. SDK applications require an Internet connection.
 */
typedef void (^BlockUserAuthentication)(UserAuthenResult result);


//Uniquely identifies the user, the SDK requires this to be stored. This ID will be sent to the AM and will allow the AM to pair with only this user.
typedef void (^CurrentSerialNub)(NSInteger serialNub);



typedef void (^DisposePO3DisconnectBlock)(BOOL resetSuc);//PO3 result
typedef void (^DisposePO3ResetDeviceBlock)(BOOL resetSuc);//PO3 result

typedef void (^DisposePO3ErrorBlock)(PO3ErrorID errorID);//PO3 error
typedef void (^DisposePO3BatteryBlock)(NSNumber *battery);    //PO3 Energy
typedef void (^DisposePO3HistoryData)(NSDictionary *historyDataDic);//Historical data

typedef void (^DisposePO3WaveHistoryData)(NSDictionary *waveHistoryDataDic);//Historical data
typedef void (^StartPO3Transmission)(BOOL startData);
typedef void (^FinishPO3Transmission)(BOOL finishData);
typedef void (^DisposePO3DataCount)(NSNumber* dataCount);//Historical data count
typedef void (^DisposePO3MeasureData)(NSDictionary* measureDataDic);//Current data
typedef void (^StartPO3MeasureData)(BOOL startData);//start Memory
typedef void (^FinishPO3MeasureData)(BOOL finishData);//Memory complete
typedef void (^DisposeSynchronousTimeFinishBlock) (BOOL finishSynchronous);//Sync time complete



#define ContinuaPOConnectNoti     @"ContinuaPOConnectNoti"
#define ContinuaPODisConnectNoti  @"ContinuaPODisConnectNoti"
#define ContinuaPODiscover        @"ContinuaPODiscover"
#define ContinuaPOConnectFailed   @"ContinuaPOConnectFailed"

#define ContinuaPOSDKRightApi  @"OpenApiContinuaPO"


typedef enum{
    ContinuaPOError_Disconnect,       //device disconnect
    ContinuaPOError_UserInvalidateRight,    //User has not been authorized
}ContinuaPOErrorID;


typedef void (^DisposeContinuaPOErrorBlock)(ContinuaPOErrorID errorID);//Communication error codes, see Continua PO error descriptions.
typedef void (^DisposeContinuaPOBatteryBlock)(NSNumber *battery);//ContinuaPO Energy

#endif
