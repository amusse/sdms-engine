//
//  AudioBasicCommunication.h
//  BG1CommDemo
//
//  Created by daiqingquan on 13-12-23.
//  Copyright (c) 2013年 daiqingquan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NewRIOInterface.h"
#import "BaseBG1.h"

@interface AudioBasicCommunication : BaseBG1<AVAudioSessionDelegate,AVAudioPlayerDelegate> {
    
	BOOL isListening;
    BOOL isPosted;
	
    uint8_t allCodeBuf[170];
    
	AudioComponentInstance toneUnit;
    BOOL starthead;
    uint8_t someDataBuf[170];
    NSTimer*startTimer;
    BOOL singleCommand;
    BOOL sendBloodCommand;
    BOOL sendResultCommand;
    BOOL sendDeadCommand;
    
    BOOL haveHeadPhone;
    int codeState;
    BOOL MicrophoneEnableBG1Flg;

    
    /////加密认证相关/////
    uint8_t bufIDDecodePointer[16];
    uint8_t bufR1DecodePointer[16];
    uint8_t bufR2DecodePointer[16];
    
    uint8_t *bufR1;
    uint8_t *bufR2;
    
    NSData *databufR1;
    NSData *databufR2;
    ///////////////////
    
    /////三次重发相关//////
    
    BOOL needResend01;
    int resendTime01;
    
    BOOL needResend20;
    int resendTime20;
    
    BOOL needResend3f;
    int resendTime3f;
    
    BOOL needResend4f;
    int resendTime4f;
    
    BOOL needResend2d;
    int resendTime2d;
    
    BOOL needResendfa;
    int resendTimefa;
    
    BOOL needResendfc;
    int resendTimefc;
    
    BOOL needResend50;
    int resendTime50;
    
    BOOL needResend501;
    int resendTime501;
    
    BOOL needResend251;
    int resendTime251;
    
    BOOL needResend252;
    int resendTime252;
    
    BOOL needResend253;
    int resendTime253;
    
    BOOL needResend254;
    int resendTime254;
    
    BOOL needResend255;
    int resendTime255;
    
    BOOL needResend256;
    int resendTime256;
    
    /////////////////
    
@public
    int control;
	double frequencyBasic;
	double sampleRateBasic;
	double theta;
}
@property(nonatomic, retain) NSString*myUserID;
@property(nonatomic, retain) NSString*codeData;
@property(nonatomic, retain) NSData *databufR1;
@property(nonatomic, retain)  NSData *databufR2;
@property(nonatomic, retain) NSString *deviceID;
@property(nonatomic, retain) NewRIOInterface *rioRef;
@property(assign) BOOL isListening;
@property(assign) int send;
@property (nonatomic, retain) NSMutableArray*tempArray;

@property (nonatomic, retain) NSMutableArray*numArray;
@property (nonatomic, retain) NSString*IDPSPro;
@property (nonatomic, retain) NSString*IDPSName;
@property (nonatomic, retain) NSString*IDPSserialnumber;


@property (strong, nonatomic)UIAlertView * ErrorAlert;
#pragma mark Listener Controls
+ (AudioBasicCommunication *)shareBG1BasicInstance;
- (void)frequencyChangedWithValue:(NSData*)newFrequency;
-(NSData *)getRightCommandFromData:(NSData *)tempData withAccurancy:(int )accurancy andDataLen:(int )smallLen;
-(uint16_t)CheckCRC:(uint8_t *)data length:(NSInteger)length;
-(NSArray*)readBG1IDPSFromPlist;

#pragma mark 下发code
-(Boolean)solveEncodeContent:(NSString *)encodeString validDate:(NSString*)date remainNum:(NSNumber*)num;
-(Boolean)solveEncodeContent:(NSString *)encodeString dataOwner:(NSString *)dataOwnerID;
-(void)solveEncodeforOneCodeWithDataOwner:(NSString *)dataOwnerID;
-(void)commandBG1Connented;
-(void)commandBG1DisConnented;


@end
