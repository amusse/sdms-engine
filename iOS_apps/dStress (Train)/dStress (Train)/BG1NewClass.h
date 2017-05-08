//
//  BG1NewClass.h
//  iHealthBG
//
//  Created by xujianbo on 14-10-08.
//  Copyright (c) 2014年 andon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NewAudioBasicCommunication.h"
#import <Foundation/Foundation.h>

#define HDVersion 0x01   //固件信息版本
//插条
typedef void (^DisposeBG1StripInBlock)(BOOL stripIn);
//拔条
typedef void (^DisposeBG1StripOutBlock)(BOOL stripOut);
//滴血
typedef void (^DisposeBG1BloodBlock)(BOOL blood);
//测量结果
typedef void (^DisposeBG1ResultBlock)(NSNumber* result);
//下发code
typedef void (^DisposeBG1SendCodeBlock)(BOOL sendOk);
//错误ID
typedef void (^DisposeBG1ErrorBlock)(NSNumber* errorID);

@interface BG1NewClass : NSObject{
    
    uint8_t allCodeBuf[170];//血糖仪
    uint8_t allCTLCodeBuf[170];//质控液
    
    DisposeBG1ErrorBlock _disposeBG1ErrorBlock;
    DisposeBG1StripInBlock _disposeBG1StripInBlock;
    DisposeBG1StripOutBlock _disposeBG1StripOutBlock;
    DisposeBG1BloodBlock _disposeBG1BloodBlock;
    DisposeBG1ResultBlock _disposeBG1ResultBlock;
    DisposeBG1SendCodeBlock _disposeBG1SendCodeBlock;
    
    
    NewAudioBasicCommunication *bg1temp;
    NSMutableDictionary *IDPSInfoDic;
    NSData *BottleIDData;
    
    BOOL outputResultFlg;
    BOOL sendFullCodeFlg;
    BOOL BG1DeadFlg;
    BOOL isBG1SFlg;
        
}
@property(nonatomic, retain) NSString*myUserID;
@property (strong, nonatomic) NSString *currentUUID;
@property (strong, nonatomic) NSString *serialNumber;
@property (strong, nonatomic)UIAlertView * ErrorAlert;


+ (BG1NewClass *)shareBG1Instance;
// zhai 移除测量块方法
-(void)removeBlock;

//获得IDPS中指定成员
-(void)getIDPSInfo:(uint8_t)IDPSMember from:(NSString *)SerialNub;


-(void)commandSendBG1CodeString:(NSString *)encodeString validDate:(NSString*)date remainNum:(NSNumber*)num DisposeBG1SendCodeBlock:(DisposeBG1SendCodeBlock)disposeBG1SendCodeBlock DisposeErrorBlock:(DisposeBG1ErrorBlock)disposeErrorBlock;

#pragma mark  发送CODE
-(Boolean)solveEncodeContent:(NSString *)encodeString validDate:(NSString*)date remainNum:(NSNumber*)num;
-(Boolean)solveEncodeContent:(NSString *)encodeString dataOwner:(NSString *)dataOwnerID;
-(void)solveEncodeforOneCodeWithDataOwner:(NSString *)dataOwnerID;

-(void)SendBG1SimpleCode: (NSString *)to;
-(void)SendBG1FullCode: (NSString *)to;

#pragma mark 认证失败信息发给应用层
-(void)BG1AuthenticationFailed;
-(void)BG1handSuccessed:(NSString *)from;
-(void)BG1sendShipingID:(NSNotification *)notification;

-(void)BG1StartAuthen:(NSString *)from;
-(void)BG1ForceStartAuthen:(NSString *)from;
-(void)setBottleID:(NSNotification *)notification;
-(void)clearSendFullCodeFlg;

@end
