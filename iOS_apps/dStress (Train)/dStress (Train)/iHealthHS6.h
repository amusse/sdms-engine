//
//  iHealthHS6.h
//  iHealthHS6
//
//  Created by daiqingquan on 15/11/25.
//  Copyright © 2015年 daiqingquan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HealthUser.h"

/*HS6 error instruction*/
/*
 error code:
 
 7:User verify error;
 
 101,102,103,104:networkerror
 
 */

typedef enum{
    IHHS6SDKUnitWeight_kg = 0,  //
    IHHS6SDKUnitWeight_lbs,     //
    IHHS6SDKUnitWeight_oz,     //
} IHHS6SDKUnitWeight;           //WeightUnit

typedef void (^DisposeHS6SuccessBlock)(NSDictionary* deviceInfo);

typedef void (^DisposeHS6FailBlock)(NSString* failmsg);

typedef void (^DisposeHS6EndBlock)(NSDictionary* deviceDic);

typedef void (^DisposeHS6ErrorBlock)(NSNumber* error);

//Binding QR Device
typedef void(^BinedQRDeviceBlock)(NSArray *resultArray);
//Binding QR Device
typedef void(^BinedQRDeviceErrorBlock)(NSString *errorCode);

//unbind Device
typedef void(^DisBinedQRDeviceBlock)(NSArray *resultArray);
//unbind QR Device
typedef void(^DisBinedQRDeviceErrorBlock)(NSString *errorCode);

typedef void (^BlockHS6UserAuthentication)(UserAuthenResult result);


typedef void (^DisposeHS6GetOpenAPISuccessBlock)(NSDictionary* openAPIInfoDic);
typedef void (^DisposeHS6GetOpenAPIErrorBlock)(NSDictionary *errorCode);

typedef void (^DisposeHS6SyncWeightUnitSuccessBlock)(BOOL syncWeightUnit);
typedef void (^DisposeHS6SyncWeightUnitErrorBlock)(NSString *errorCode);

@interface iHealthHS6 : NSObject{

    DisposeHS6SuccessBlock _disposeHS6SuccessBlock;
    
    DisposeHS6FailBlock _disposeHS6FailBlock;
    
    DisposeHS6EndBlock _disposeHS6EndBlock;
    
    DisposeHS6ErrorBlock _disposeHS6ErrorBlock;
    
    BlockHS6UserAuthentication _blockHS6UserAuthentication;
    
    
    BinedQRDeviceBlock _blockBinedQRDevice;
    BinedQRDeviceErrorBlock _blockBinedQRDeviceError;
    
    DisBinedQRDeviceBlock _blockDisBinedQRDevice;
    DisBinedQRDeviceErrorBlock _blockDisBinedQRDeviceError;

    DisposeHS6GetOpenAPISuccessBlock _blockDisGetOpenAPI;
    DisposeHS6GetOpenAPIErrorBlock _blockDisGetOpenAPIError;
    
    DisposeHS6SyncWeightUnitSuccessBlock _blockDisSyncWeightUnit;
    DisposeHS6SyncWeightUnitErrorBlock _blockDisSyncWeightUnitError;

}

+(iHealthHS6 *)shareIHHS6Controller;


/*Binding QR Device*/
//When using the SDK for the first time, measuring method needs to be called to finish user verification.
/*
 Import parameter:
 tempUser: Properties included: clientID，clientSecret，userID，height，weight，isAthlete，sex.
 deviceID: device Mac
 clientID & clientSecret: the only identification for users of the SDK, requires registration from iHealth administrator, please email: lvjincan@ihealthlabs.com.cn.com for more information.
 Return parameters:
 BlockHS6UserAuthentication: The return parameters of ’‘userid’, ‘height’, ’clientID’, and ‘clientSecret’ after verification
 
 The interpretation for the verification:
 HS6UserAuthen_RegisterSuccess: New-user registration succeeded.
 HS6UserAuthen_LoginSuccess: User login succeeded.
 HS6UserAuthen_CombinedSuccess: The user is an iHealth user as well, measurement via SDK has been activated, and the data from the measurement belongs to the user.
 HS6UserAuthen_TrySuccess: Testing without internet connection succeeded.
 HS6UserAuthen_InvalidateUserInfo: Userid/clientID/clientSecret verification failed.
 HS6UserAuthen_SDKInvalidateRight: SDK has not been authorized.
 HS6UserAuthen_UserInvalidateRight: User has not been authorized.
 HS6UserAuthen_InternetError: Internet error, verification failed.
 The measurement via SDK will be operated in the case of 1-4, and will be terminated if any of 5-8 occurs. The interface needs to be re-called after analyzing the return parameters.
 Notice: when a new user registers via SDK, an ‘iHealth disclaimer’ will pop up automatically, and will require the user to agree in order to continue. SDK applications require an Internet connection; there is 10-day trial period if the SDK cannot connect to the internet, the SDK is fully functional during tryout period, but will be terminated without a working internet connection after 10 days.
 
 binedResult: Action = 1;(1:bind sucess 2：full user bind faild 3,:ts error bind faild 4 other),
 BineNum=10
 MAC
 Position = 1;（user in weight number）
 SetWifi=0
 Status = 1;（1:binding 2：unbind）
 TS (time);

 binedError: error code
 Error code definition：
 refer to “error” : HS6 error instruction.
 */

-(void)cloudCommandUserBinedQRDeviceWithUser:(HealthUser *)tempUser  deviceID:(NSString *)tempDeviceID BlockHS6UserAuthentication:(BlockHS6UserAuthentication)BlockHS6UserAuthentication binedResult:(BinedQRDeviceBlock)result binedError:(BinedQRDeviceErrorBlock)error;

/*unBind QR Device*/
/*
 Import parameter:
 tempUser: Properties included: clientID，clientSecret，userID，height.
 deviceID: device Mac
 clientID & clientSecret: the only identification for users of the SDK, requires registration from iHealth administrator, please email: lvjincan@ihealthlabs.com.cn.com for more information.
 Return parameters:
 
 disBinedResult: Action = 1;(1:bind sucess 2：full user bind faild 3,:ts error bind faild 4 other),
 MAC
 Status = 2;（1:binding 2：unbind）
 TS
 
 disBinedError: error code
 Error code definition：
 refer to “error” : HS6 error instruction.
 */


-(void)cloudCommandUserDisBinedQRDeviceForUser:(HealthUser *)tempUser withDeviceID:(NSString *)tempDeviceID disBinedResult:(DisBinedQRDeviceBlock)result disBinedError:(DisBinedQRDeviceErrorBlock)error;

/*set HS6 wifi*/
/*
 Import parameter:
 password: wifi password.
 Return parameters:
 
 disposeHS6SuccessBlock: bind sucess
 
 disposeHS6FailBlock:user bind faild
 
 disposeHS6EndBlock: end set
 
 disposeHS6ErrorBlock: error code
 Error code definition：
 refer to “error” : HS6 error instruction.
 */

-(void)commandSetHS6WithPassWord:(NSString*)password disposeHS6SuccessBlock:(DisposeHS6SuccessBlock)disposeHS6SuccessBlock disposeHS6FailBlock:(DisposeHS6FailBlock)disposeHS6FailBlock  disposeHS6EndBlock:(DisposeHS6EndBlock)disposeHS6EndBlock disposeHS6ErrorBlock:(DisposeHS6ErrorBlock)disposeHS6ErrorBlock;


/*get open APItoken*/
/**
 * Get AccessToken of HS6 user,and this method is a time consuming operation that cannot be called
 * in the main thread.
 * After get AccessToken, you can call openApi(http://developer.ihealthlabs.com) to pull data form iHealth cloud.
 *
 * @param clientID     the identification of the SDK
 * @param clientSecret the identification of the SDK
 * @param username      the identification of the user
 * @return boolean whether has permission of using the method,false you need to get the
 * permission firstly.
 * The result of get token is returnned by
 * {@link iHealthDeviceHs6Callback#onNotify(String, String, String, String)} and its'
 * action is {@link #ACTION_HS6_GET_TOKEN}.
 *
 * eg. {APIName="OpenApiActivity OpenApiBG OpenApiBP OpenApiFood OpenApiSleep OpenApiSpO2 OpenApiSport
 * OpenApiUserInfo OpenApiWeight",
 * AccessToken="9fuIPl3Bo6lqJfbYjXFjuPnNwNqVfxjiUE7cMCZSjrX22RJSoKf28jtIhI0v86wjV5GJ21bc6LvMNbfYG0QsZ7cYuUSO0EkaiFTST*GcjZvvTKxfEOmhQTfLXTXYAOA
 * wCoXlEs0DRqJaHZU5JS30ssyLNlqADPV9dlvWZitQmIfXjF6CSZM2SuRCD*bbbrqtwBsn*sC24OEoQCRpDau6wQ",
 * ClassParamentKey =     {
 * "client_id" = 2a8387e3f4e94407a3a767a72dfd52ea;
 * "client_para" = 99;
 * "client_secret" = fd5e845c47944a818bc511fb7edb0a77;
 * hash = d86bf92a851bf9211851d8e6827eea55;
 * username = "he@12.com";
 * };
 * "Expires":3672000,
 * "RefreshToken":"9fuIPl3Bo6lqJfbYjXFjuPnNwNqVfxjiUE7cMCZSjrX22RJSoKf28jtIhI0v86wjV5GJ21bc6LvMNbfYG0QsZ2TcvTQNInn85XdPIJRIe-9zB-eaY5utBVKmtLjJdEEmBlx5le5mT6oF7WBwVkwx*CUpSsdgUcyE3mG3FJnSHlajogaUSUgvMgmgUaVEMYzv4pcbCUltGNAMqJt5wwvBZA",
 * "RefreshTokenExpires":31536000,
 * "UUID":"",
 * "UserID":"51027f3e09a14a55917e687c628a0f13",
 * "UserNameKey"
 * "UserOpenID"
 * "UserRegion":"https:\/\/api.ihealthlabs.com.cn:8443",
 * "client_para":"random_str"}
 */

-(void)commandHS6GetOpenAPITokenWithUser:(HealthUser *)tempUser withSuccessBlock:(DisposeHS6GetOpenAPISuccessBlock)getOpenAPIBlock withErrorBlock:(DisposeHS6GetOpenAPIErrorBlock)errorBlock;





/*sync Weight Unit*/
/*
 Import parameter:
 unit:  IHHS6SDKUnitWeight_kg,IHHS6SDKUnitWeight_lb,IHHS6SDKUnitWeight_oz
 Return parameters:
 unitBlock: YES or NO
 errorBlock: error code
 Error code definition：
 refer to “error” : HS6 error instruction.
 */
-(void)commandHS6WithUser:(HealthUser *)tempUser withSyncWeightUnit:(IHHS6SDKUnitWeight)unit withSuccessBlock:(DisposeHS6SyncWeightUnitSuccessBlock)unitBlock withErrorBlock:(DisposeHS6SyncWeightUnitErrorBlock)errorBlock;


@end
