//
//  XFGeniusCamera.h
//  XiaoFuGenius
//
//  Created by xiaofutech on 2018/5/15.
//  Copyright © 2018年 xiaofutech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XFGeniusCamera : NSObject

/**
 是否反向图像，默认，自己使用 YES
 */
@property (nonatomic, assign) BOOL retroflexion;

/**
 图像传输码率设置，默认800，可选1200，
 */
@property (nonatomic, assign) int bitrate;

/**
 端口号设置，默认1000
 */
@property (nonatomic, assign) int port;

/**
 拍摄模式：0-优先拍摄表皮层，1-优先拍摄基底层（核心版本 >= 3.1.2）；默认 0；
 */
@property (nonatomic, assign) int captureMode;

/**
 摄像头开启成功 回调
 */
@property (nonatomic, copy) void (^Noti_CameraOK)(void);

/**
 图像数据传输发生错误 回调
 */
@property (nonatomic, copy) void (^Noti_CameraError)(void);

/**
 Method,01-摄像头管理实例
 */
+ (XFGeniusCamera *)SharedCamera;

/**
 移除之前设置的View 释放内存
 */
- (void)destroyedCameraView;

/**
 Method,02-开启摄像头
 @param ip 传输用ip地址
 @param cameraView 承载图像的父视图
 Tips：针对 核心固件 3.1.0+，收到摄像头成功开启回调后要调用一次 setLedMode: 方法；
 */
- (void)start_IP:(NSString *)ip CameraView:(UIView *)cameraView;

/**
 Method,03-关闭摄像头
 @param completion 完成回调
 */
- (void)stop_Completion:(void (^)(void))completion;

/**
 Method,04-拍照 - 设备核心固件版本 <= 10202 需要调用此方法
 @param pathName 图片缓存地址
 @param completion 完成回调
 */
- (void)capture:(NSString *)pathName Completion:(void (^)(BOOL isOK))completion;

/**
 Method,05-拍照
 @param rgbPath rgb图片缓存地址
 @param plPath pl图片缓存地址
 @param completion 完成回调
 */
- (void)capture_RgbPath:(NSString *)rgbPath PlPath:(NSString *)plPath
             Completion:(void (^)(BOOL isOK))completion;

/**
 Method,06-切换表皮层/基底层
 @param mode 0-表皮层，1-基底层
 */
- (void)setLedMode:(int)mode;

@end
