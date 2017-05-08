//
//  NewAudioBasicCommunication.h
//  BG1CommDemo
//
//  Created by daiqingquan on 13-12-23.
//  Copyright (c) 2013年 daiqingquan. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "NewRIOInterface.h"
#import "BaseBG1.h"
#import "BG1NewClass.h"

@interface NewAudioBasicCommunication : BaseBG1<AVAudioSessionDelegate,AVAudioPlayerDelegate> {
    
    
    NSArray *prtocolArray;//子协议名称数组
    uint8_t BasicTempData[170];
    
	BOOL isListening;
    BOOL isPosted;
	
    //////////////////////
    
    BOOL starthead;
    NSTimer*startTimer;
   
    BOOL sendBloodCommand;
    BOOL sendResultCommand;
    BOOL sendDeadCommand;
    BOOL resendAPIDflg;
    
    
    BOOL oldBG1Flg;             //新旧BG1判断标志
    BOOL isIDPSFlg;
    
    BOOL haveHeadPhone;
    BOOL authenticationSuccessFlg;
    BOOL needAuthentication;
    BOOL sendBottleIDFlg;
    BOOL sendCodeFinishedFlg;

    
    NSDictionary  *IdpsOrderDic;
    NSMutableDictionary *totalDataBagDic;
    NSMutableDictionary *lastSendDic;//上层发送的最后一条命令
    NSMutableDictionary *reSendTimeOfDevice;//上层发送的命令重发次数
    
    int _reSendTime;//重发标志位
    
    NSData *tempCommandID;

    Boolean FBFlag;
    Boolean FDFlag;
    
    Boolean authenFlag;
    

    NSMutableDictionary *sendDictionary;
    NSMutableData *lastSendData;//最后一次得数据
    
    NSMutableDictionary *BufDic;//每包的缓存字典
    NSMutableArray *sendDataArray;//分包后分包集合

    NSMutableDictionary *sendIndexDic;//发送顺序ID
    NSMutableDictionary *sendMaxLenDic;//发送每包最大长度
    NSMutableArray *authenticationDeviceList;//认证通过设备
    NSMutableArray *authenticationDownDeviceList;//暂时认证通过的设备
    
    
    BOOL isBG1SFlg;
    
    //认证定时器，定时清理认证失败设备，打开认证开关
    NSTimer *authenTimer;
    
    //定时判断是否为BG1 1304
    NSTimer *isOldBG1Timer;
    
    //握手阶段通信定时器  xu_20151119
    NSTimer *communicationHandTimer;
    
    uint8_t theSendOrderID;  //xu_保存发送的命令ID
    
    NSNumber *bgModelNumber; //用于BG1版本限制 xu_20160531
    
@public
    int control;
	double frequency;
	double sampleRateBasic;
	
}
@property(nonatomic, retain) NSString*codeData;
@property(nonatomic, retain) NSData *databufR1;
@property(nonatomic, retain)  NSData *databufR2;
@property(nonatomic, retain) NSString *deviceID;
@property(nonatomic, retain) NewRIOInterface *rioRef;
@property(assign) BOOL isListening;


@property (nonatomic, retain) NSMutableArray *numArray;


@property (strong, nonatomic)UIAlertView *ErrorAlert;


+(NSString *) macAddress;

#pragma mark authentication

-(NSString *)dataFilePathNameLastR1To:(NSString *)to;

-(void)forceStartAuthenticationTo:(NSString *)to Protocol:(NSString *)protocol;

-(void)BG1StartAuthen:(NSString *)to Protocol:(NSString *)protocol;

-(void)startBG1AuthenticationTo:(NSString *)to Protocol:(NSString *)protocol;

-(void)sendBG1AuthenticationR2:(NSData *)R2 To:(NSString *)to Protocol:(NSString *)protocol makeSureIndex:(uint8_t)makeSureIndex;

-(void)failedToBG1AuthenticationTo:(NSString *)to Protocol:(NSString *)protocol makeSureIndex:(uint8_t)makeSureIndex;

#pragma mark -接收相关函数

-(void)receiveCommand:(NSData *)command From:(NSString *)from Protocol:(NSString *)protocol;

-(NSData *)handleCommand:(NSData *)command From:(NSString *)from Protocol:(NSString *)protocol;

//查询设备总接收包数,判断是否接收完毕
-(int)detectDataBag:(NSString *)deviceID;

//拼接小包数据为整包
-(NSData *)detectDataBagRec:(NSString *)deviceID;



#pragma mark -发送相关函数

-(void)sendData:(NSData *)data To:(NSString *)BG1SerialNum Protocol:(NSString *)protocol IsNeedRespond:(BOOL)isNeedRespond MakeSureStatueID:(uint8_t)makeSureStatueID MakeSureIndex:(uint8_t) makeSureIndex isBottomLayer:(BOOL)isBottomLayer;

-(NSData *)reMakeSendData:(NSData *)data To:(NSString *)BG1SerialNum Protocol:(NSString *)protocol IsNeedRespond:(BOOL)isNeedRespond MakeSureStatueID:(uint8_t)MakeSureStatueID MakeSureIndex:(uint8_t)MakeSureIndex isBottomLayer:(BOOL)isBottomLayer;

-(void)MakeSureMethodBG1:(NSMutableDictionary *)dic;

-(void)totalBG1CommandMakeSureMethod:(NSMutableDictionary *)dic;

//取消指定设备底重发
-(void)cancel:(uint8_t)index from:(NSString *)from;
//取消整条命令的重发
-(void)cancelTotalCommand:(NSString *)from;

#pragma mark
#pragma mark - BG1 通讯辅助方法
-(void)saveBG1DeviceIDToPlist:(NSString*)myDeviceID;
-(NSString*)readBG1DeviceIDFromPlist:(NSString*)strID;
-(void)saveBG1IdpsHandToPlist:(NSDictionary*)dic;
-(NSArray*)readBG1IdpsHandFromPlist;
-(void)deleteIdpsHandInfoPlist;

#pragma mark Listener Controls
+ (NewAudioBasicCommunication *)NewBG1CommunicationObject;
- (void)handAndGetCommand:(NSData*)commandData;
-(NSData *)getRightCommandFromData:(NSData *)tempData withAccurancy:(int)accurancy andDataLen:(int )smallLen;

#pragma mark  计算校验和
-(uint16_t)CheckCRC:(uint8_t *)data length:(NSInteger)length;
-(uint8_t)checkData:(NSData*)data;//计算校验和

#pragma mark -

-(void)setAppID:(NSNotification *)notification;
-(void)isOldBG1Device;

-(void)commandNewBG1Connented;

@end
