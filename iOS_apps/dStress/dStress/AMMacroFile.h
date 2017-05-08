//
//  AMMacroFile.h
//  AMDemoCode
//
//  Created by zhiwei jing on 14-8-12.
//  Copyright (c) 2014年 zhiwei jing. All rights reserved.
//

#import "HealthUser.h"

#ifndef AMDemoCode_AMMacroFile_h
#define AMDemoCode_AMMacroFile_h

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
typedef void (^BlockUserAuthentication)(UserAuthenResult result);//the result of userID verification

//Uniquely identifies the user, the SDK requires this to be stored. This ID will be sent to the AM and will allow the AM to pair with only this user.
typedef void (^CurrentSerialNub)(NSInteger serialNub);

#define AM3Discover         @"AM3Discover"
#define AM3ConnectFailed    @"AM3ConnectFailed"
#define AM3ConnectNoti      @"AM3ConnectNoti"
#define AM3DisConnectNoti   @"AM3DisConnectNoti"

#define AM3SDiscover        @"AM3SDiscover"
#define AM3SConnectFailed   @"AM3SConnectFailed"
#define AM3SConnectNoti     @"AM3SConnectNoti"
#define AM3SDisConnectNoti  @"AM3SDisConnectNoti"

#define AM4Discover         @"AM4Discover"
#define AM4ConnectFailed    @"AM4ConnectFailed"
#define AM4ConnectNoti      @"AM4ConnectNoti"
#define AM4DisConnectNoti   @"AM4DisConnectNoti"

#define AMDeviceID @"ID"
#define AMSDKSportRightApi  @"OpenApiActivity"
#define AMSDKSleepRightApi  @"OpenApiSleep"

//cloud
#define AMDate @"AMDate"
#define AMCalorie @"AMcalorie"
#define AMStepNum @"AMstepNum"
#define AMStepSize @"AMstepSize"
//cloud


///////////////////////////////////////////////////////////////

//AM3

///////////////////////////////////////////////////////////////


#define AM3TimeInterval @"TimeInterval"

#define AM3ActiveHistoryDateYear    @"ActiveHistoryDateYear"
#define AM3ActiveHistoryDateMonth   @"ActiveHistoryDateMonth"
#define AM3ActiveHistoryDateDay     @"ActiveHistoryDateDay"
#define AM3ActiveHistoryTotoalNum   @"ActiveHistoryTotoalNum"
#define AM3ActiveStepSize           @"ActiveStepSize"

#define AM3SleepHistoryDateYear    @"SleepHistoryDateYear"
#define AM3SleepHistoryDateMonth   @"SleepHistoryDateMonth"
#define AM3SleepHistoryDateDay     @"SleepHistoryDateDay"
#define AM3SleepHistoryDateHour    @"SleepHistoryDateHour"
#define AM3SleepHistoryDateMinute  @"SleepHistoryDateMinute"
#define AM3SleepHistoryDateSeconds @"SleepHistoryDateSeconds"
#define AM3SleepHistoryTotoalNum   @"SleepHistoryTotoalNum"

typedef enum{
    AM3Error_OverTime = 0,    //Communication error
    AM3Error_NoRespond,       //You did not receive a response within a certain period of time, usually clogged Bluetooth
    AM3Error_ResetDeviceFaild,//Reset failed
    AM3Error_Disconnect,       //AM disconnect
    
    AM3Error_ParameterError = 400,    //ParameterError
    AM3Error_UserInvalidateRight,    //User has not been authorized

    AM3Error_FirmwareVersionIsNotSupported  //firmware version is not supported
}AM3ErrorID;

typedef enum{
    AM3TimeFormat_hh,//12
    AM3TimeFormat_HH//24
}AM3TimeFormat;

typedef enum{
    AM3StateUnit_mile,
    AM3StateUnit_km
}AM3StateUnit;



typedef enum{
    AM3StateModel_sleep,
    AM3StateModel_active,
    AM3StateModel_fly,
    AM3StateModel_drive
}AM3StateModel;

typedef enum{
    AM3StateInfo_waist,
    AM3StateInfo_wrist,
    AM3StateInfo_sleep
}AM3StateInfo;

typedef void (^DisposeAM3ErrorBlock)(AM3ErrorID errorID);//Communication error codes, see AM3 error descriptions.
typedef void (^DisposeAM3GetUserBinedDeviceMACBlock)(NSString *deviceMAC);//UserBinedDeviceMAC
typedef void (^DisposeAM3GetDeviceUserIDBlock)(unsigned int userID);//userID
typedef void (^DisposeAM3SyncTimeBlock)(BOOL resetSuc);//SyncTime
typedef void (^DisposeAM3TimeFormatBlock)(AM3TimeFormat  timeFormat);//dateFormatter
typedef void (^DisposeAM3TimeFormatSettingBlock)(BOOL resetSuc);//setdateFormatter
typedef void (^DisposeAM3SetDeviceUserIDBlock)(BOOL resetSuc);//set user ID
typedef void (^DisposeAM3SetUserInfoBlock)(BOOL resetSuc);//set user infomation
typedef void (^DisposeAM3SetBMRBlock)(BOOL resetSuc);//setBMR
typedef void (^DisposeAM3ActiveStartTransmission)(NSDictionary *startDataDictionary);//Start uploading motion data
typedef void (^DisposeAM3ActiveHistoryData)(NSArray *historyDataArray);//sportData
typedef void (^DisposeAM3ActiveFinishTransmission)();//Upload motion complete
typedef void (^DisposeAM3SleepStartTransmission)(NSDictionary *startDataDictionary);//Start uploading sleep data
typedef void (^DisposeAM3SleepHistoryData)(NSArray *historyDataArray);//sleepData
typedef void (^DisposeAM3SleepFinishTransmission)();//Upload sleep complete
typedef void (^DisposeAM3GetCurrentActiveInfo)(NSDictionary *activeDictionary);//Total calories and steps for today, including parameters：Step、Calories、TotalCalories
typedef void (^DisposeAM3TotoalAlarmData)(NSMutableArray *totoalAlarmArray);//Alarm array contains up to 3 alarms, each one needs the following parameters：AlarmId、Time、IsRepeat、Switch、（Sun、Mon、Tue、Wed、Thu、Fri、Sat)
typedef void (^DisposeAM3SetAlarmBlock)(BOOL resetSuc);//set Alarm
typedef void (^DisposeAM3DeleteAlarmBlock)(BOOL resetSuc);//delete Alarm
typedef void (^DisposeAM3RemindInfoBlock)(NSArray *remindInfo);//remind
typedef void (^DisposeAM3SetReminderBlock)(BOOL resetSuc);//set remind
typedef void (^DisposeAM3StateInfoBlock)(AM3StateInfo queryState);//query State
typedef void (^DisposeAM3BatteryBlock)(NSNumber *battery);//AM battery percentage, from 0～100.
typedef void (^DisposeAM3ResetDeviceBlock)(BOOL resetSuc);//Restore factory settings.
typedef void (^DisposeAM3DisconnectBlock)(BOOL resetSuc);//disconnect
typedef void (^DisposeAM3UserInfoBlock)(NSDictionary *userInfo);//query userinfo
typedef void (^DisposeAM3SetStateBlock)(BOOL resetSuc);//query userinfo
typedef void (^DisposeAM3SetStateModelBlock)(BOOL resetSuc); //setStateModel
///////////////////////////////////////////////////////////////

//AM3S

///////////////////////////////////////////////////////////////

#define AM3STimeInterval @"TimeInterval"

#define AM3SActiveHistoryDateYear    @"ActiveHistoryDateYear"
#define AM3SActiveHistoryDateMonth   @"ActiveHistoryDateMonth"
#define AM3SActiveHistoryDateDay     @"ActiveHistoryDateDay"
#define AM3SActiveHistoryTotoalNum   @"ActiveHistoryTotoalNum"
#define AM3SActiveStepSize           @"ActiveStepSize"

#define AM3SSleepHistoryDateYear    @"SleepHistoryDateYear"
#define AM3SSleepHistoryDateMonth   @"SleepHistoryDateMonth"
#define AM3SSleepHistoryDateDay     @"SleepHistoryDateDay"
#define AM3SSleepHistoryDateHour    @"SleepHistoryDateHour"
#define AM3SSleepHistoryDateMinute  @"SleepHistoryDateMinute"
#define AM3SSleepHistoryDateSeconds @"SleepHistoryDateSeconds"
#define AM3SSleepHistoryTotoalNum   @"SleepHistoryTotoalNum"


#define ReportState @"ReportState"

#define Work_outMeasureDate @"Work_outMeasureDate"
#define Work_outTimeNumber @"Work_outTimeNumber"
#define Work_outStepNumber @"Work_outStepNumber"
#define Work_outLengthNumber @"Work_outLengthNumber"
#define Work_outCalories @"Work_outCalories"
#define Work_outTimeZone @"Work_outTimeZone"

#define Sleep_summaryMeasureDate @"Sleep_summaryMeasureDate"
#define Sleep_summarySleepTime @"Sleep_summarySleepTime"
#define Sleep_summarysleepEfficiency @"Sleep_summarysleepEfficiency"
#define Sleep_summarysleepAddMinute @"Sleep_summarysleepAddMinute"
#define Sleep_summaryTimeZone @"Sleep_summaryTimeZone"


typedef enum{
    AM3SError_OverTime = 0,    //Communication error
    AM3SError_NoRespond,       //You did not receive a response within a certain period of time, usually clogged Bluetooth
    AM3SError_ResetDeviceFaild,//Reset failed
    AM3SError_Disconnect,       //AM disconnect
    
    AM3SError_ParameterError = 400,    //ParameterError
    AM3SError_UserInvalidateRight,          //User has not been authorized
    AM3SError_FirmwareVersionIsNotSupported  //firmware version is not supported
}AM3SErrorID;


typedef enum{
    AM3STimeFormat_hh,//12
    AM3STimeFormat_HH,//24
    AM3STimeFormat_NoEuropeAndhh,
    AM3STimeFormat_EuropeAndhh,
    AM3STimeFormat_NoEuropeAndHH,
    AM3STimeFormat_EuropeAndHH,
}AM3STimeFormatAndNation;


typedef enum{
    AM3SKmUnit_mile,
    AM3SKmUnit_km,
}AM3SKmUnit;

typedef enum{
    AM3SState_waist,
    AM3SState_wrist,
    AM3SState_sleep
}AM3SQueryState;



typedef enum{
    AM3SPicture_one,//
    AM3SPicture_two,//
}AM3SPicture;


typedef enum{
    AM3SReportStage_Work_out = 1,
    AM3SReportStage_Sleep_summary,
}AM3SReportStage;


typedef enum{
    AM3SActive_State = 0,//sport
    AM3SSleep_State = 1,//sleep
    AM3SFly_State = 2, //flight
    AM3SWorkout_State = 4, //workout
}AM3SActiveState;


typedef void (^DisposeAM3SGetUserBinedDeviceMACBlock)(NSString *deviceMAC);//UserBinedDeviceMAC

typedef void (^DisposeAM3SGetDeviceUserIDBlock)(unsigned int userID);//userID


typedef void (^DisposeAM3SErrorBlock)(AM3SErrorID errorID);//Communication error codes, see AM3S error descriptions.


typedef void (^DisposeAM3SSetRandomNumberBlock)(NSString *randomNumString);//random number
typedef void (^DisposeAM3SSyncTimeBlock)(BOOL resetSuc);//SyncTime
typedef void (^DisposeAM3STimeFormatAndNationBlock)(AM3STimeFormatAndNation  timeFormatAndNation);////dateFormatter
typedef void (^DisposeAM3STimeFormatAndNationSettingBlock)(BOOL resetSuc);////setdateFormatter
typedef void (^DisposeAM3SSetDeviceUserIDBlock)(BOOL resetSuc);//set user ID
typedef void (^DisposeAM3SSetUserInfoBlock)(BOOL resetSuc);//set user infomation
typedef void (^DisposeAM3SSetBMRBlock)(BOOL resetSuc);//setBMR
typedef void (^DisposeAM3SActiveStartTransmission)(NSDictionary *startDataDictionary);//Start uploading motion data
typedef void (^DisposeAM3SActiveHistoryData)(NSArray *historyDataArray);//sportData
typedef void (^DisposeAM3SActiveFinishTransmission)();//Upload motion complete
typedef void (^DisposeAM3SSleepStartTransmission)(NSDictionary *startDataDictionary);//Start uploading sleep data
typedef void (^DisposeAM3SSleepHistoryData)(NSArray *historyDataArray);//sleepData
typedef void (^DisposeAM3SSleepFinishTransmission)();//Upload sleep complete
typedef void (^DisposeAM3SGetCurrentActiveInfo)(NSDictionary *activeDictionary);//Total calories and steps for today, including parameters：Step、Calories、TotalCalories
typedef void (^DisposeAM3SResetDeviceBlock)(BOOL resetSuc);//Restore factory settings.
typedef void (^DisposeAM3STotoalAlarmData)(NSMutableArray *totoalAlarmArray);//Alarm array contains up to 3 alarms, each one needs the following parameters：AlarmId、Time、IsRepeat、Switch、（Sun、Mon、Tue、Wed、Thu、Fri、Sat)
typedef void (^DisposeAM3SSetAlarmBlock)(BOOL resetSuc);//set Alarm
typedef void (^DisposeAM3SDeleteAlarmBlock)(BOOL resetSuc);//delete Alarm
typedef void (^DisposeAM3SRemindInfoBlock)(NSArray *remindInfo);//remind
typedef void (^DisposeAM3SSetReminderBlock)(BOOL resetSuc);//set remind
typedef void (^DisposeAM3SStateInfoBlock)(AM3SQueryState queryState);//query State
typedef void (^DisposeAM3SBatteryBlock)(NSNumber *battery);//AM battery percentage, from 0～100.
typedef void (^DisposeAM3SDisconnectBlock)(BOOL resetSuc);//disconnect
typedef void (^DisposeAM3SStageMeasureDataBlock)(NSArray *measureDataArray);
typedef void (^DisposeAM3SStageMeasureFinishBlock)(BOOL resetSuc);
typedef void (^DisposeAM3SUserInfoBlock)(NSDictionary *userInfo);//query userinfo
typedef void (^DisposeAM3SGetPictureBlock)(AM3SPicture picture);//query picture
typedef void (^DisposeAM3SSetPictureBlock)(BOOL resetSuc);//set picture





//---------------------------------------------

typedef void (^DisposeRandomNumberSetting)(BOOL resetSucSetting);

//The user's AM3's MAC Address
typedef void (^DisposeBinedAMSerialNub) (NSString *binedSerialNub);
//The connected user's MAC Address
typedef void (^DisposeCurrentSerialNub) (NSString *currentSerialNub);

//YES: Account bonding successfu. NO: Failed
typedef void (^DisposeBinedUserResult) (BOOL result);
//True: Success，False: Failed.
typedef void (^DisposeDisBinedUserResult) (BOOL result);
typedef void (^DisposeAM3SBlock)(BOOL resetSuc);//YES: Command successfu. NO: Failed
typedef void (^RemindAM3SInfo)(NSArray *remindInfo);//Array containing following parameters：Time、Switch
typedef void (^StartAM3STransmission)(NSDictionary *startDataDictionary);//Start uploading motion data
typedef void (^StartSleepTransmission)(NSDictionary *startDataDictionary);//Start uploading sleep data
typedef void (^DisposeAM3SProgressData)(NSNumber *progress);//AM data upload percentage, 0.0～1.0
typedef void (^DisposeSleepProgressData)(NSNumber *progress);//AM data upload percentage, 0.0～1.0
typedef void (^AM3SHistoryData)(NSArray *historyDataArray);//sportData
typedef void (^SleepHistoryData)(NSArray *historyDataArray);//sleepData
typedef void (^FinishAM3STransmission)();//Upload motion complete
typedef void (^FinishSleepTransmission)();//Upload sleep complete
typedef void (^SleepIsOnTransmission)(BOOL isTransmiting);//
typedef void (^AM3SIsOnTransmission)(BOOL isTransmiting);//
typedef void (^DisposeAM3SStateInfo)(AM3SQueryState queryState);//AM status，State_wrist  (AM3S being worn on the wrist)，State_waist (AM3 worn with belt clip).
typedef void (^DisposeAM3SBattery)(NSNumber *battery);//AM battery percentage, from 0～100.
typedef void (^DisposeAM3SAskUserID)(unsigned int userID);//user ID
typedef void (^DisposeAM3SQueryCurrentActiveInfo)(NSDictionary *activeDictionary);//Total calories and steps for today, including parameters：Step、Calories、TotalCalories
typedef void (^DisposeAM3SUserInfo)(NSDictionary *userInfo);
typedef void (^DisposeTimeFormat)(AM3STimeFormatAndNation timeFormat);
typedef void (^DisposeTimeFormatSetting)(BOOL resetSucSetting);
typedef void (^DisposeSyncSportCount)(NSNumber *sportCount);
typedef void (^DisposeMeasureData)(NSArray *measureDataArray);
typedef void (^DisposeFinishMeasure)(BOOL finishUpload);
typedef void (^DisposePicture)(AM3SPicture picture);
typedef void (^DisposePictureSetting)(BOOL resetSucSetting);
typedef void (^DisposeQueryBinedSerialNub) (NSString *serialNub);
//---------------------------------------------


///////////////////////////////////////////////////////////////


//AM4

///////////////////////////////////////////////////////////////


#define AM4TimeInterval @"TimeInterval"

#define AM4ActiveHistoryDateYear    @"ActiveHistoryDateYear"
#define AM4ActiveHistoryDateMonth   @"ActiveHistoryDateMonth"
#define AM4ActiveHistoryDateDay     @"ActiveHistoryDateDay"
#define AM4ActiveHistoryTotoalNum   @"ActiveHistoryTotoalNum"
#define AM4ActiveStepSize           @"ActiveStepSize"
#define AM4ActiveTimeZone           @"ActiveTimeZone"

#define AM4SleepHistoryDateYear    @"SleepHistoryDateYear"
#define AM4SleepHistoryDateMonth   @"SleepHistoryDateMonth"
#define AM4SleepHistoryDateDay     @"SleepHistoryDateDay"
#define AM4SleepHistoryDateHour    @"SleepHistoryDateHour"
#define AM4SleepHistoryDateMinute  @"SleepHistoryDateMinute"
#define AM4SleepHistoryDateSeconds @"SleepHistoryDateSeconds"
#define AM4SleepHistoryTotoalNum   @"SleepHistoryTotoalNum"
#define AM4SleepHistoryTimeZone   @"SleepHistoryTimeZone"



#define AM4SwimmingMeasureDate @"SwimmingMeasureDate"
#define AM4SwimmingTimeNumber @"SwimmingTimeNumber"
#define AM4SwimmingTimes @"SwimmingTimes"
#define AM4Swimmingcalories @"Swimmingcalories"
#define AM4SwimmingAct @"SwimmingAct"
#define AM4SwimmingPoollength @"SwimmingPoollength"
#define AM4SwimmingCircleCount @"SwimmingCircleCount"
#define AM4EnterSwimmingTime @"EnterSwimmingTime"
#define AM4OutSwimmingTime @"OutSwimmingTime"
#define AM4SwimmingProcessMark @"SwimmingProcessMark"
#define AM4SwimStartTimeStamp @"SwimStartTimeStamp"
#define AM4SwimmingTimeZone @"SwimmingTimeZone"


//AM4
typedef enum{
    AM4Error_OverTime = 0,    //Communication error
    AM4Error_NoRespond,       //You did not receive a response within a certain period of time, usually clogged Bluetooth
    AM4Error_ResetDeviceFaild,//Reset failed
    AM4Error_Disconnect,       //AM disconnect
    
    AM4Error_ParameterError = 400,    //ParameterError
    AM4Error_UserInvalidateRight,          //User has not been authorized
    AM4Error_FirmwareVersionIsNotSupported  //firmware version is not supported
}AM4ErrorID;

typedef enum{
    AM4TimeFormat_hh,//12
    AM4TimeFormat_HH,//24
    AM4TimeFormat_NoEuropeAndhh,//
    AM4TimeFormat_EuropeAndhh,
    AM4TimeFormat_NoEuropeAndHH,
    AM4TimeFormat_EuropeAndHH,
}AM4TimeFormatAndNation;

typedef enum{
    AM4KmUnit_mile,
    AM4KmUnit_km,
}AM4KmUnit;

typedef enum{
    AM4SwimmingUnit_m,
    AM4SwimmingUnit_km,
}AM4SwimmingUnit;

typedef enum{
    AM4State_waist,
    AM4State_wrist,
    AM4State_sleep
}AM4QueryState;

typedef enum{
    AM4Picture_one,
    AM4Picture_two,
}AM4Picture;

typedef enum{
    AM4SwimmingAction_Crawl,
    AM4SwimmingAction_Breaststroke,
    AM4SwimmingAction_Backstroke,
    AM4SwimmingAction_Butterfly,
    AM4SwimmingAction_MixedSwimming,
    AM4SwimmingAction_Unkonw
}AM4SwimmingAction;

typedef enum{
    AM4ReportStage_Swimming,
    AM4ReportStage_Work_out,
    AM4ReportStage_Sleep_summary,
}AM4ReportStage;

typedef enum{
    AM4Active_State =0,
    AM4Sleep_State =1,
    AM4Fly_State =2,
    AM4Workout_State=4,
    AM4Swimming_State=5,
}AM4ActiveState;



typedef void (^DisposeAM4GetUserBinedDeviceMACBlock)(NSString *deviceMAC);//UserBinedDeviceMAC
typedef void (^DisposeAM4GetDeviceUserIDBlock)(unsigned int userID);//userID
typedef void (^DisposeAM4ErrorBlock)(AM4ErrorID errorID);//Communication error codes, see AM4 error descriptions.
typedef void (^DisposeAM4SetRandomNumberBlock)(NSString *randomNumString);//random number
typedef void (^DisposeAM4SyncTimeBlock)(BOOL resetSuc);//SyncTime
typedef void (^DisposeAM4TimeFormatAndNationBlock)(AM4TimeFormatAndNation  timeFormatAndNation);////dateFormatter
typedef void (^DisposeAM4TimeFormatAndNationSettingBlock)(BOOL resetSuc);////setdateFormatter
typedef void (^DisposeAM4SetDeviceUserIDBlock)(BOOL resetSuc);//set user ID
typedef void (^DisposeAM4SetUserInfoBlock)(BOOL resetSuc);//set user infomation
typedef void (^DisposeAM4SetBMRBlock)(BOOL resetSuc);//setBMR
typedef void (^DisposeAM4ActiveStartTransmission)(NSDictionary *startDataDictionary);//Start uploading motion data
typedef void (^DisposeAM4ActiveHistoryData)(NSArray *historyDataArray);//sportData
typedef void (^DisposeAM4ActiveFinishTransmission)();//Upload motion complete
typedef void (^DisposeAM4SleepStartTransmission)(NSDictionary *startDataDictionary);//Start uploading sleep data
typedef void (^DisposeAM4SleepHistoryData)(NSArray *historyDataArray);//sleepData
typedef void (^DisposeAM4SleepFinishTransmission)();//Upload sleep complete
typedef void (^DisposeAM4GetCurrentActiveInfo)(NSDictionary *activeDictionary);//Total calories and steps for today, including parameters：Step、Calories、TotalCalories
typedef void (^DisposeAM4ResetDeviceBlock)(BOOL resetSuc);//Restore factory settings.
typedef void (^DisposeAM4TotoalAlarmData)(NSMutableArray *totoalAlarmArray);//Alarm array contains up to 3 alarms, each one needs the following parameters：AlarmId、Time、IsRepeat、Switch、（Sun、Mon、Tue、Wed、Thu、Fri、Sat)
typedef void (^DisposeAM4SetAlarmBlock)(BOOL resetSuc);//set Alarm
typedef void (^DisposeAM4DeleteAlarmBlock)(BOOL resetSuc);//delete Alarm
typedef void (^DisposeAM4RemindInfoBlock)(NSArray *remindInfo);//remind
typedef void (^DisposeAM4SetReminderBlock)(BOOL resetSuc);//set remind
typedef void (^DisposeAM4StateInfoBlock)(AM4QueryState queryState);//query State
typedef void (^DisposeAM4BatteryBlock)(NSNumber *battery);//AM battery percentage, from 0～100.
typedef void (^DisposeAM4DisconnectBlock)(BOOL resetSuc);//disconnect
typedef void (^DisposeAM4SwimmingBlock)(BOOL swimmingIsOpen, NSNumber * swimmingLaneLength,NSNumber * NOSwimmingTime, AM4SwimmingUnit unit);//query swimming
typedef void (^DisposeAM4SettingSwimmingBlock)(BOOL resetSuc);//set swimming
typedef void (^DisposeAM4StageMeasureDataBlock)(NSArray *measureDataArray);
typedef void (^DisposeAM4StageMeasureFinishBlock)(BOOL resetSuc);
typedef void (^DisposeAM4UserInfoBlock)(NSDictionary *userInfo);//query userinfo


#endif
