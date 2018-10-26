//
//  XFGeniusConfig.h
//  XiaoFuGenius
//
//  Created by xiaofutech on 2018/5/15.
//  Copyright © 2018年 xiaofutech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XFGeniusConfig : NSObject

/**
 debug开关 for CameraDrive
 开启以后，终端会输出debug日志，默认 关闭；
 */
@property (nonatomic, assign) BOOL debugEnableForCameraDrive;

/**
 debug开关
 开启以后，终端会输出debug日志，默认 关闭；
 */
@property (nonatomic, assign) BOOL debugEnable;

/**
 debug log block
 赋值后，内部方法会在适当时机调用，并返回关键日志
 */
@property (nonatomic, copy) void (^debugLogBlock)(NSString *log);

/**
 设置请求超时时间，大于等于10s，默认 10s；
 该属性暂未生效，如有需要，请自行处理相关方法调用的超时处理；
 */
@property (nonatomic, assign) CGFloat reqMaxTimeOut;

/**
 分隔字符串数组，默认 @[@"!@"]；
 */
@property (nonatomic, strong) NSArray <NSString *> *splitStrings;

/**
 蓝牙扫描时，外围设备的保活时间，默认 2s，自定义时间 >2s；
 超过保活时间即被视为该外围设备已失活，将从已扫描到的设备列表中移除；
 -1 指定，不执行 相关的保活逻辑，设备扫描到一次，将会始终存在于设备列表中；
 */
@property (nonatomic, assign) NSTimeInterval peripheralAliveTime;

/**
 设置默认首选连接方式，<0 优先sta，1 强制ap>，默认 0
 */
@property (nonatomic, assign) int smartType;

/**
 场景：5G网络
 sta连接之前，是否进行5G网络判断，YES - 启用5G网络判断，默认 NO；
 版本要求：蓝牙 1.0.11+  固件 1.2.7+
 另外：蓝牙 1.1.0+ 固件 1.3.0+ 支持2.4g下的11+信道
 */
@property (nonatomic, assign) BOOL verify5GEnable;

/**
 场景：星巴克，公共验证类wifi
 sta连接成功后，是否ping分配给设备的ip地址，YES - 启用ping检查，默认 NO；
 */
@property (nonatomic, assign) BOOL staPingEnable;

/**
 STA (ssid & pwd) 缓存
 公共WiFi模式，存储连接成功的ssid和pwd，YES - 启用缓存，默认 NO；
 */
@property (nonatomic, assign) BOOL staCachesStored;

/**
 STA 模式下会忽略的 ssid 字符串数组
 全匹配 ssid 开头字符串，匹配成功，判定为测试仪热点，不做为STA模式可用wifi处理
 */
@property (nonatomic, strong) NSArray<NSString *> *ignoreStrings;

/**
 StartOV788:1 延迟 500 ms 测试，默认关闭
 */
@property (nonatomic, assign) BOOL enableStartOv788TestForCameraDrive;

/**
 StartOV788:1 延迟 500 ms 测试时间，默认 500 ms
 */
@property (nonatomic, assign) double startOv788TestTimetForCameraDrive;

/**
 取得XFGeniusConfig的共享实例
 @return XFGeniusConfig的共享实例
 */
+ (XFGeniusConfig *)SharedConfig;

/**
 取得当前的配置信息
 @return 当前配置的信息字典
 */
- (NSDictionary *)configInfo;

@end
