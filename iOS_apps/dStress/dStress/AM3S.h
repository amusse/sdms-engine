//
//  AM3S.h
//  iHealthApp2
//
//  Created by 小翼 on 14-7-2.
//  Copyright (c) 2014年 andon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AMMacroFile.h"

@interface AM3S : NSObject
@property (retain, nonatomic) NSMutableString *randomString;
@property (retain, nonatomic) NSString *currentUUID;
@property (retain, nonatomic) NSString *serialNumber;
@property (retain, nonatomic) NSString *firmwareVersion;

/**
 * Establish memory and measurement connection,Only after verification through this interface can we move onto using other API's.
 * @param tempUser includes properties：clientID，clientSecret，userID.userID，either email or mobile phone number (mobile phone number not yet supported).ClientID and clientSecret, the only identification for users of the SDK, requires registration from iHealth administrator, please email:heguangming@ihealthlabs.com.cn for more information
 * @param disposeAuthenticationBlock The return parameters of ’‘userid’, ’clientID’,and ‘clientSecret’ after verification.
 * The interpretation for the verification:
 *  1. UserAuthen_RegisterSuccess, New-user registration succeeded.
 *  2. UserAuthen_LoginSuccess， User login succeeded.
 *  3. UserAuthen_CombinedSuccess, The user is iHealth user as well, measurement via SDK has been activated, and the data from the measurement belongs to the user.
 *  4. UserAuthen_TrySuccess, testing without Internet connection succeeded.
 *  5. UserAuthen_InvalidateUserInfo, Userid/clientID/clientSecret verification failed.
 *  6. UserAuthen_SDKInvalidateRight, SDK has not been authorized.
 *  7. UserAuthen_UserInvalidateRight,User has not been authorized.
 *  8. UserAuthen_InternetError, Internet error, verification failed.
 *  --PS:
 *  The measurement via SDK will be operated in the case of 1-3, and will be terminated if any of 4-8 occurs. The interface needs to be re-called after analyzing the return parameters.
 *  @Notice  Notice: when a new user registers via SDK, an ‘iHealth disclaimer’ will pop up automatically, and will require the user to agree in order to continue. SDK applications require an Internet connection.
 * @param serialNub Uniquely identifies the user, the SDK requires this to be stored. This ID will be sent to the AM3S and will allow the AM3S to pair with only this user.
 * @param disposeAskUserID The user ID that is stored on the AM3, 0 indicates that there is no user inf
 * @param binedSerialnub The user's AM3S's MAC Address
 * @param currentSerialNub The connected user's MAC Address
 * @param disposeErrorBlock Communication error codes, see AM3S error descriptions.
 */
-(void)commandCreateUserManageConnectWithUser:(HealthUser *)tempUser Authentication:(BlockUserAuthentication)disposeAuthenticationBlock currentUserSerialNub:(CurrentSerialNub)serialNub amUser:(DisposeAM3SAskUserID)disposeAskUserID binedAMSerialNub:(DisposeBinedAMSerialNub)binedSerialnub currentSerialNub:(DisposeCurrentSerialNub)currentSerialNub DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;



/**
 * Sending a random number,This API sends a random number to the AM3S. Only when the random number matches the number displayed on the AM3S screen can the device be bound to the device.
 * @param disposeBlock True: Sent successfully，False: Failed。Random number is six digits, ranging from 0 – 999999. AM3S will receive the random number and display on screen. The user will have to enter it into the app.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandAM3SSetRandomNumber:(DisposeRandomNumberSetting)setRandomNumberBlock withErrorBlock:(DisposeAM3SErrorBlock)errorBlock;



/**
 * Restore Binding AM3S to user,Account binding requires an active internet connection
 * @param userID userID
 * @param tempRandom the 6 random numbers displayed on the AM3S。
 * @param disposeBlock True: Success， False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandSetAM3SUserID:(NSNumber*)userID withRandom:(NSString *)tempRandom DisposeBlock:(DisposeAM3SBlock)disposeBlock  DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;





/**
 * Restore AM3S initialization,Must be called the first time to ensure that the AM3S has correct user information, goals, time, battery checks, etc.
 * @param tempUser User information, needs to include the following：birthday、height、weight、bmr、sex、lengthUnit.birthday，NSDate.height，(cm).weight，(kg).bmr，user basal metabolic rate.sex，UserSex_Female or UserSex_Male.lengthUnit，total distance，LengthUnit_Mile is imperial units.LengthUnit_Kilometer for metric units.
 * @param goalNumber User goal number of steps. Default is 10,000
 * @param disposeStateInfo AM status，State_wrist  (AM3S being worn on the wrist)，State_waist (AM3S worn with belt clip).
 * @param disposeBattery AM battery percentage, from 0～100.
 * @param disposeBlock True: Success， False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandSyncUserInfoWithUser:(HealthUser *)tempUser andGoal:(NSNumber*)goalNumber DisposeStateInfo:(DisposeAM3SStateInfo)disposeStateInfo DisposeBattery:(DisposeAM3SBattery)disposeBattery DisposeBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;






/**
 * Restore factory settings.
 * @param disposeBlock True: Success， False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandResetDeviceDisposeResultBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;



/**
 * Query Alarm.
 * @param disposeTotoalAlarmData Alarm array contains up to 3 alarms, each one needs the following parameters：AlarmId、Time、IsRepeat、Switch、（Sun、Mon、Tue、Wed、Thu、Fri、Sat)AlarmId：1, 2, 3.Time：HH:mm.IsRepeat：Repeat alarm， True: Repeat， False: Don't repeat.Switch：Alarm on/off. True: On, False: Off.Sun、Mon、Tue、Wed、Thu、Fri、Sat：True.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandQueryAlarmInfo:(DisposeAM3STotoalAlarmData)disposeTotoalAlarmData DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;


/**
 * Set Alarm.
 * @param alarmDic Alarm information, include parameters：AlarmId、Time、IsRepeat、Switch、（Sun、Mon、Tue、Wed、Thu、Fri、Sat)
 * @param disposeBlock True: Alarm set successfully，False: Failed
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandSetAlarmWithAlarmDictionary:(NSDictionary *)alarmDic DisposeResultBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;




/**
 * Delete alarm.
 * @param alarmID alarmID：1, 2, 3.
 * @param disposeBlock True: Delete successful，False: Failed
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandDeleteAlarmViaID:(NSNumber *)alarmID DisposeResultBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;



/**
 * Query reminder.
 * @param remindInfo Array containing following parameters：Time、Switch.Time：format HH:mm, time between reminders (HH*60+mm) minutes.Switch：Reminder on/off，True: On， False: Off.
 * @param disposeErrorBlock see AM3S error descriptions
 */
-(void)commandQueryReminder:(RemindAM3SInfo)remindInfo DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;




/**
 * Set reminders.
 * @param reminderDic Array containing collowing parameters：Time、Switch.
 * @param disposeBlock True: Success，False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions
 */
-(void)commandSetReminderwithReminderDictionary:(NSDictionary *)reminderDic DisposeResultBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;


/**
 * Upload AM3S data,Data type: 5 minutes of motion data, 5 minutes of sleep data, total number of steps for the day, and total calories. Also includes the number of steps for the 5 minutes of motion data, total calories for the current time, calories of the steps, and total calories. If calculations for every 5 minutes of motion data is required, you will need to calculate the difference between two records.
 * @param startAM3STransmission Start uploading motion data, including parameters：StartActiveHistoryDate、StepSize、StartActiveHistoryTotoalNum。StartActiveHistoryDate：Start date，yyyy-MM-dd.StepSize：Length of each step, cm。StartActiveHistoryTotoalNum：Number of records
 * @param disposeAM3SProgressData data upload percentage, 0.0～1.0.
 * @param AM3ShistoryData data，including the following parameters：AMDate、AMCalorie、AMStepNum。AMDate：Workout time， NSDate.AMCalorie: Current time total calories。AMStepNum：Total number of steps。
 * @param finishAM3STransmission complete.
 * @param startSleepTransmission Start uploading sleep data, including parameters：SleepActiveHistoryDate、AM3SSleepHistoryTotoalNum.SleepActiveHistoryDate：Sleep start time，yyyy-MM-dd HH:mm:ss.StartActiveHistoryTotoalNum: Number of records.disposeSleepProgressData:  AM sleep data upload percent 0.0～1.0.sleepHistoryData：Sleep data, including the following parameters:：AMDate、SleepData。
 * @param disposeSleepProgressData Sleep time, NSDate.
 * @param sleepHistoryData Sleep grade, 0: awake, 1: light sleep, 2: deep sleep.finishSleepTransmission：Upload complete.
 * @param finishSleepTransmission complete.
 * @param disposeQueryCurrentActiveInfo Total calories and steps for today, including parameters：Step、Calories、TotalCalories.Step：Number of steps taken today.Calories：Number of calories burned today.TotalCalories：Sum calories burned and bmr today.
 * @param disposeErrorBlock see AM3S error descriptions
 * @param am3SisOnTransmission Invalidate.
 * @param sleepisOnTransmission Invalidate.
 */
-(void)commandSyncAllAM3SData:(StartAM3STransmission)startAM3STransmission
            DisposeProgress:(DisposeAM3SProgressData)disposeAM3SProgressData historyData:(AM3SHistoryData)AM3ShistoryData
         FinishTransmission:(FinishAM3STransmission)finishAM3STransmission startsleepdata:(StartSleepTransmission) startSleepTransmission
       DisposeSleepProgress:(DisposeSleepProgressData)disposeSleepProgressData sleephistoryData:(SleepHistoryData)sleepHistoryData
    FinishSleepTransmission:(FinishSleepTransmission)finishSleepTransmission
          CurrentActiveInfo:(DisposeAM3SQueryCurrentActiveInfo) disposeQueryCurrentActiveInfo
          DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock
        AM3SIsOnTransmission:(AM3SIsOnTransmission)am3SisOnTransmission
      SleepIsOnTransmission:(SleepIsOnTransmission)sleepisOnTransmission;


/**
 * Upload AM3S report data.
 * @param disposeSyncSportCount Total report number.
 * @param disposeMeasureData Report data, including parameters:、ReportStage_Work_out(1)、ReportStage_Sleep_summary(2).ReportStage_Work_out:Report Stage active,including parameters:Work_outCalories(Workout calories burned)、Work_outLengthNumber(Workout distance)、Work_outMeasureDate(Start time)、Work_outStepNumber(Workout number of steps)、Work_outTimeNumber(Length of workout)、dataID(data ID);ReportStage_Sleep_summary:Report Stage sleep,including parameters:Sleep_summaryMeasureDate（Sleep start time）、Sleep_summarySleepTime（Sleep duration）、Sleep_summarysleepAddMinute（Correct sleep duration length）、Sleep_summarysleepEfficiency（Sleep efficiency percentage, range is 0-100） 
 * @param disposeFinishMeasure YES: Success，NO: Failed.
 * @param disposeErrorBlock see AM3S error descriptions
 */
-(void)commandSetSyncsportCount:(DisposeSyncSportCount)disposeSyncSportCount DisposeMeasureData:(DisposeMeasureData)disposeMeasureData disposeFinishMeasure:(DisposeFinishMeasure)disposeFinishMeasure  DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;


/**
 * Disconnect AM3S connection.
 * @param disposeBlock True: Success，False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandDisconnectDisposeBlock:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;

/**
 * Query AM3S Picture.
 * @param disposePicture Picture_one,Picture_two
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandAM3SQueryPicture:(DisposePicture)disposePicture disposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;

/**
 * Query AM3S Set Picture.
 * @param AM3SPicture Picture_one,Picture_two
 * @param disposeSetPicture True: Success，False: Failed.
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandAM3SSetPicture:(AM3SPicture)pictures resultBlock:(DisposePictureSetting)disposeSetPicture disposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;


/**
 * Query AM3S TimeFormat.
 * @param disposeTimeFormat AM3STimeFormat_hh,AM3STimeFormat_HH,AM3STimeFormat_NoEuropeAndhh,AM3STimeFormat_EuropeAndhh,AM3STimeFormat_NoEuropeAndHH,AM3STimeFormat_EuropeAndHH,
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandQueryTimeFormat:(DisposeTimeFormat)disposeTimeFormat disposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock;



/**
 * Get user infomation
 * @param userInfoBlock including parameters:age,steplength,height,gender,weight,unit,goal
 * @param disposeErrorBlock see AM3S error descriptions.
 */
-(void)commandQueryUserInfo:(DisposeAM3SBlock)disposeBlock DisposeErrorBlock:(DisposeAM3SErrorBlock)disposeErrorBlock DisposeUserInfo:(DisposeAM3SUserInfo)disposeUserInfo;

@end
