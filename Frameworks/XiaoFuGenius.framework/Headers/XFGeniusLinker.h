//
//  XFGeniusLinker.h
//  XiaoFuGenius
//
//  Created by xiaofutech on 2018/5/15.
//  Copyright © 2018年 xiaofutech. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol XFGeniusLinkerDelegate <NSObject>
@optional
/**
 蓝牙开启状态通知
 */
- (void)XFGeniusLinkerDelegateBluetoothStatus:(BOOL)status;

/**
 蓝牙扫描结果通知
 */
- (void)XFGeniusLinkerDelegateScanUpdate:(NSNotification *)notify;

/**
 手机与设备的蓝牙连接状态通知
 */
- (void)XFGeniusLinkerDelegateBleConnected:(BOOL)connected;

/**
 设备充电状态通知
 {@"Electric":@(NSInteger), @"IsCharge":@(NSInteger),
 @"Success":@(NSInteger), @"Msg":NSString}
 */
- (void)XFGeniusLinkerDelegateBatteryUpdate:(NSDictionary *)batteryInfo;

/**
 STA，模式已启动，等待调用方法
 + (void)STA: Password: Completion:
 发送指令，指示设备连接指定的wifi
 */
- (void)XFGeniusLinkerDelegatePreparedForSTA_SSID:(NSString *)ssid;

/**
 STA，开始5G网络类型检查
 备注：前提是需要开启5G网络类型检查开关，并且设备版本支持5G网络类型检查
 */
- (void)XFGeniusLinkerDelegate5GverifyProcessStart;

/**
 STA，开始ping设备已获取到的IP地址
 备注：前提是需要开启Ping开关
 */
- (void)XFGeniusLinkerDelegateIPpingProcessStart;

/**
 STA，设备网络连接过程中，遇到的其它错误
 补充：wifiStatus：{-5:5g网络，-3:ping失败, -2:旧版本，未搜索到ssid, -1:密码错误, 0:请求失败或超时, 1:成功}
 注1：{wifiStatus = -5，设备网络连接过程中，5G网络的判断回调，前提，需要打开5G判断开关；当前判断并非十分准确，
      主要是借助设备搜索指定ssid来实现的；若（设备蓝牙版本<1.1.1 && 核心版本<1.3.0）， 则2.4g的12+信道，也会被误判为5g}
 注2：{wifiStatus = -3，设备网络连接过程中，星巴克类公共wifi的可连接判断回调，前提，需要打开staPing开关；
      当前判断并非十分准确，如果网络为非公共验证类wifi，但是由于其它原因导致ping失败，同样会误判为公共验证类wifi}
 注3：{wifiStatus = -2，（设备蓝牙版本<1.0.11 && 核心版本<1.2.7，不支持5G判断），
      故当前wifi可能为2.4g的12+信道，或5g}
 */
- (void)XFGeniusLinkerDelegateSTA_ErrorsConfirmed:(int)wifiStatus ErrMsg:(NSString *)errMsg;

/**
 AP，模式已启动，等待用户确认连接设备热点，然后再次调用方法
 + (void)GeniusNetworkStatusCheckCompletion: StartCompletion:
 如果已连接正确的热点，此时状态会改变成功ap模式已连接，且能获取到设备ip地址
 备注：建议调用前，主动判断手机当前连接ssid是否是指定的设备热点，YES调用上述方法，NO直接提示未连接指定热点
 */
- (void)XFGeniusLinkerDelegatePreparedForAP_SSID:(NSString *)ssid Pwd:(NSString *)pwd;
@end

@interface XFGeniusLinker : NSObject

#pragma mark - Basics
/**
 Method,01 - 初始化连接管理类
 */
+ (void)InitLinker;

/**
 Method,02 - 添加观察者
 */
+ (void)AddObserver:(id <XFGeniusLinkerDelegate>)observer;

/**
 Method,03 - 移除观察者
 */
+ (void)RemoveObserver:(id <XFGeniusLinkerDelegate>)observer;

#pragma mark - Scan Event
/**
 Method,04 - 开始扫描附近设备
 调用前，注意断开与当前设备的连接，如果启动了主控芯片，则断开之前，还应关闭主控芯片
 */
+ (void)StartScan;

/**
 Method,05 - 停止扫描
 */
+ (void)StopScan;

#pragma mark - Ble Connect Event
/**
 Method,06 - 开始连接设备蓝牙，（geniusId，geniusName 选其一即可，但不能同时为空）
 @param geniusId 设备Id
 @param geniusName 设备名称
 补充：连接成功后会自动获取固件版本信息，如果获取失败，则会断开蓝牙连接（逻辑测试中...）
 并重置Genius信息字典
 */
+ (void)BleConnectGeniusId:(NSString *)geniusId GeniusName:(NSString *)geniusName
                Completion:(void (^)(BOOL success, int status, NSString *msg))completion;
/**
 Method,07 - 断开手机与设备的蓝牙连接
 */
+ (void)BleDisConnect:(void (^)(BOOL success, NSString *msg))completion;

/**
 Method,08 - 仅用于等待设备返回确认反馈，此方法不会发出任何命令
 */
+ (void)BleBind:(void (^)(BOOL success, NSString *msg))completion;

/**
 Method,09 - 关机
 */
+ (void)Shutdown;

#pragma mark - Core Chip Op Event
/**
 Method,10 - 执行设备 网络状态检查，网络连接
 @param checkCompletion 执行设备网络状态检查完成的回调，
 @param startCompletion 启动设备网络连接完成的回调，
 备注：type：设备当前连接类型 0 未知，1 STA，2 AP；
 ip：成功获取到的设备ip地址；
 */
+ (void)GeniusNetworkStatusCheckCompletion:(void (^)(BOOL success, NSString *msg, int type,
                                                     NSString *ssid, NSString *password,
                                                     NSString *ip))checkCompletion
                           StartCompletion:(void (^)(BOOL success, NSString *msg, int type,
                                                     NSString *ip))startCompletion;

/**
 Method,11 - 执行设备网络连接，公共wifi类型
 备注：配合 GeniusNetworkStatusCheckCompletion:StartCompletion: 使用
 一般需要在代理方法 XFGeniusLinkerDelegatePreparedForSTA_SSID: 调用之后进行..
 */
+ (void)STA:(NSString *)ssid Password:(NSString *)password;

/**
 Method,12 - 执行设备网络连接，设备热点类型
 备注：可以直接调用，但建议配合 GeniusNetworkStatusCheckCompletion:StartCompletion: 使用
 完全交由上述方法自动进行，或者用上述方法确定当前处于非ap连接状态之后手动调用..
 */
+ (void)AP;

/**
 Method,13 - 执行设备主控芯片的断电操作
 */
+ (void)PowerDown:(void (^)(BOOL success, NSString *msg))completion;

#pragma mark - Calibration
/**
 获取是否需要校准
 @param completion -2：未知状态码，-1：请求失败，0：不需要校准，1：需要校准，2：已校准
 注：版本要求，Ble 2.1.0 & Core 3.1.0
 */
+ (void)CalibrationStatusCheck:(void (^)(int status, NSString *msg))completion;

/**
 图像校准
 @param command 校准命令 1：校准(ave>250)，2：校准(ave<=250)，3：清除(回滚)
 @param completion 完成回调
 注：版本要求，Ble 2.1.0 & Core 3.1.0
 */
+ (void)CalibrationImageWithCommand:(int)command
                         Completion:(void (^)(BOOL success, NSString *msg))completion;

/**
 RestartNVDS
 一键恢复 校准配置文件
 注：版本要求，Ble 2.1.0 & Core 3.1.0
 */
+ (void)CalibrationRestartNVDS:(void (^)(BOOL success, NSString *msg))completion;

/**
 有线模式 设置
 @param command 设置命令，-2：混合模式（新设备），-1：混合模式（旧设备），1：有线模式（旧设备），2：有线模式（新设备）
 @param completion 完成回调
 注：版本要求，Ble 2.1.0 & Core 3.1.0
 */
+ (void)SetWiredModeCommand:(int)command Completion:(void (^)(BOOL success, NSString *msg))completion;

#pragma mark - Update Event
/**
 Method,14 - 蓝牙固件升级，升级期间，会多次调用 response
 @response 升级进度回调，（status：1 正常，0 失败），（value：升级进度 0-100），（msg：升级说明，含失败说明）
 */
+ (void)UpdateBLE:(NSData*)firmware
         Response:(void (^)(int status, int value, NSString *msg))response;

/**
 Method,15 - 核心固件升级，升级期间，会多次调用 response
 @response 升级进度回调，（status：1 正常，0 失败），（value：升级进度 0-100），（msg：升级说明，含失败说明）
 */
+ (void)UpdateCore:(NSData*)firmware
          Response:(void (^)(int status, int value, NSString *msg))response;

#pragma mark - Get (Battery、MAC、Version) Info Event
/**
 Method,16 - 取得设备 电量 信息
 */
+ (void)Battery:(void (^)(BOOL success, BOOL isCharge, int electric, NSString *msg))completion;

/**
 Method,17 - 取得设备 MAC 信息
 */
+ (void)MAC:(void (^)(BOOL success, NSString *mac, NSString *msg))completion;

/**
 Method,18 - 取得设备 蓝牙、内核 固件版本信息
 备注：会启动主控芯片
 */
+ (void)Version:(void (^)(BOOL success, NSString *Ble,NSString *Core,
                          int BleValue, int CoreValue, NSString *msg))completion;

#pragma mark - Get Info Event
/**
 Method,19 - 获取手机蓝牙开启状态
 */
+ (BOOL)BluetoothStatus;

/**
 Method,20 - 获取手机与设备的蓝牙连接状态
 @return 0 未连接，1 连接中，2已连接
 */
+ (int)BleConnectStatus;

/**
 Method,21 - 获取设备信息，前提是蓝牙连接成功
 {@"BindID":NSString, @"BindName":NSString,
 @"BleVersion":NSString, @"CoreVersion":NSString,
 @"BleVerValue":@(NSInteger), @"CoreVerValue":@(NSInteger)}
 */
+ (NSDictionary *)Genius;

#pragma mark - Helper Methods
/**
 Method,22 - 获取手机当前连接wifi的ssid
 */
+ (NSString *)GetSSID;

/**
 Method,23 - 获取当前已缓存的所有成功连接的公共WiFi的数据
 */
+ (NSArray <NSDictionary *>*)STACaches;

/**
 Method,24 - 获取手机当前连接wifi的密码，如果存在
 */
+ (NSString *)STACachesContains:(NSString *)ssid;

/**
 Method,25 - 清除已缓存的所有成功连接的公共WiFi的数据
 */
+ (void)STACachesAllCleaned;

@end
