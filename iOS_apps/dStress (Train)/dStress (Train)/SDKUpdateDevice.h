//
//  SDKUpdateDevice.h
//  iHealthDemoCode
//
//  Created by daiqingquan on 16/7/27.
//  Copyright © 2016年 zhiwei jing. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum{
    UpdateNetworkError=200,             //UpdateNetworkError
    UpdateOrderError=201,               //Before starting the upgrade must go to query version first
    UpdateDeviceEnd = 301,              //UpdateDeviceEnd
    UpdateDeviceDisconnect = 300,       //UpdateDeviceDisconnect
    UpdateInputError=302,               //UpdateInputError
    NOUpdateUpgrade=400,                //NOUpdateUpgrade
}UpdateDeviceError;

typedef enum{
    UpdateModelStatusFree = 1,
    UpdateModelStatusBusy
}UpdateModelStatus;


typedef void (^DisposeUpdateVersionResult)(NSDictionary * updateVersionDic);

typedef void (^DisposeUpdateErrorBlock)(UpdateDeviceError errorID);

typedef void (^UpdateModuleState)(NSNumber*updateModuleState);

typedef void (^DisposeEndUpdateResult)(NSNumber* endUpdate);

typedef void (^DisposeDownloadFirmwareStart)();

typedef void (^DisposeDownloadFirmwareFinish)();

typedef void (^DisposeUpdateProgress)(NSNumber *progress);

typedef void (^DisposeUpdateResult)(NSNumber*updateResult);

typedef void (^TransferSuccess)(NSNumber*transferSuccess);


@interface SDKUpdateDevice : NSObject{
    
    DisposeUpdateVersionResult _disposeUpdateVersionResult;
    
    DisposeUpdateErrorBlock _disposeUpdateErrorBlock;
    
    UpdateModuleState _updateModuleState;
    
    DisposeEndUpdateResult _disposeEndUpdateResult;
    
    DisposeDownloadFirmwareStart _disposeDownloadFirmwareStart;
    
    DisposeUpdateProgress _disposeUpdateProgress;
    
    DisposeDownloadFirmwareFinish _disposeDownloadFirmwareFinish;
    
    DisposeUpdateResult _disposeUpdateResult;
    
    TransferSuccess _transferSuccess;
    
    NSMutableDictionary*updateDeviceInfoDic;
    
}
@property Boolean isInUpdateModel;

@property(nonatomic,strong)NSMutableArray *beforeUpdateImageArray;

@property(nonatomic,strong)NSMutableArray *afterUpdateImageArray;

@property(nonatomic,strong)NSString *protocolString;

@property(nonatomic,strong)NSString *currentDeviceUUID;

@property Boolean testMode;

@property Boolean isTransferFlag;


/**
 * Initialize SDKUpdateDevice controller class
 */
+(SDKUpdateDevice*)shareSDKUpdateDeviceInstance;


/**
 * Get device Version and cloudDeviceVersion.
 * Import Parameters: UUIDString:device UUID.
 * @param DisposeUpdateVersionResult  include:DeviceType、DeviceVersion、DeviceStatus(1：YES，0：NO)、CloudDeviceVersion、KeepUpdateFlag（flag，0：firstUpdate，1：go on update）、UpdateProtocolVersion(100:Equipment upgrades returned directly after the success or failure 101:Upgrade is complete only on behalf of the firmware successfully transferred, reconnect the device after the upgrade success or failure).
 * @param DisposeErrorBlock Update error codes, see UpdateDeviceError error descriptions.
 */

-(void)commandGetUpdateVersionWithDeviceUUID:(NSString*)uuidString DisposeUpdateVersionResult:(DisposeUpdateVersionResult)updateVersionDic  DisposeErrorBlock:(DisposeUpdateErrorBlock)disposeErrorBlock;
/**
 * Get Update Module State.
 * @param UpdateModuleState  (0:free,1:uploading)
 * @param DisposeErrorBlock Update error codes, see UpdateDeviceError error descriptions.
 */
-(void)commandGetUpdateModuleState:(UpdateModuleState)updateModuleState DisposeErrorBlock:(DisposeUpdateErrorBlock)disposeErrorBlock;
/**
 * Stop Update.
 * @param DisposeEndUpdateResult （bool 1：sucess，0：fail）
 * @param DisposeErrorBlock Update error codes, see UpdateDeviceError error descriptions.
 */
-(void)commandEndUpdateWithDeviceUUID:(NSString*)uuidString DisposeEndUpdateResult:(DisposeEndUpdateResult)endUpdateResult  DisposeErrorBlock:(DisposeUpdateErrorBlock)disposeErrorBlock;

/**
 *  Update Device.
 * Import Parameters: UUIDString:device UUID.
 * @param DisposeDownloadFirmwareStart:  start update firmware from cloud.
 * @param DisposeDownloadFirmwareFinish: finish update firmware from cloud.
 * @param DisposeUpdateProgress:   Update Device progress（0-100）.
 * @param DisposeUpdateResult:  Update Device result（bool 1：sucess，0：fail）Applicable to the protocol of 100.
 * @param TransferSuccess:  TransferSuccess Applicable to the protocol of 101.
 * @param DisposeErrorBlock Update error codes, see UpdateDeviceError error descriptions.
 */

-(void)commandStartUpdateWithDeviceUUID:(NSString*)uuidString DownloadFirmwareStart:(DisposeDownloadFirmwareStart)disposeDownloadFirmwareStart DisposeDownloadFirmwareFinish:(DisposeDownloadFirmwareFinish)disposeDownloadFirmwareFinish DisposeUpdateProgress:(DisposeUpdateProgress)disposeUpdateProgress DisposeUpdateResult:(DisposeUpdateResult)disposeUpdateResult TransferSuccess:(TransferSuccess)transferSuccess  DisposeErrorBlock:(DisposeUpdateErrorBlock)disposeErrorBlock;


@end
