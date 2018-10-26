//
//  XFGeniusNEHelper.h
//  XiaoFuGenius
//
//  Created by xiaofutech on 2018/6/21.
//  Copyright © 2018年 xiaofutech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XFGeniusNEHelper : NSObject

/**
 仅支持系统 11.0+
 @param ssid wifi名称
 @param pwd wifi密码
 @param completion 完成回调
 */
+ (void)TryHotspotSSID:(NSString *)ssid Pwd:(NSString *)pwd
            Completion:(void (^)(BOOL success, NSError *error))completion;

+ (void)IPAddressConfirmed:(void (^)(BOOL success))confirmed;

@end
