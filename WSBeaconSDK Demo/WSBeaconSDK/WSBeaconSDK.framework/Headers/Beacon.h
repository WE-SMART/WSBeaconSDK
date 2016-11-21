//
//  Beacon.h
//  微智Beacon
//
//  Created by TrusBe Sil on 16/5/31.
//  Copyright © 2016年 we-smart Co., Ltd. All rights reserved.
//

#import <Realm/Realm.h>

#pragma mark - Beacon

@interface Beacon : RLMObject

// device info
@property NSString      *name;          // 设备名称
@property NSString      *perId;         // 设备 UUID, 主键（primaryKey）
@property NSData        *macAddres;     // 设备 Mac 地址
@property int           rssi;           // 设备信号
@property int           protocolVersion;// 协议版本
@property int           firmVersion;    // 固件版本
@property BOOL          isEncrypt;      // 是否加密
@property NSString      *originPsw;     // 当前密码
@property NSString      *updatePsw;     // 新密码

// beacon info
@property NSString      *uuid;          // Beacon UUID
@property int           major;          // Beacon Major
@property int           minor;          // Beacon Minor
@property int           gap;            // Beacon Gap
@property int           power;          // Beacon Power
@property int           txPower;        // Beacon TxPower
@property int           deviceModel;    // 设备类型
@property int           battery;        // 电量百分比 0xff:检测失败， 0xFE:不带电池
@property int           batteryStatus;  // 是否低功耗 0:low power, 1:normal
@property int           supports;       // 是否支持加密 bit0:是否支持加密, bit1:是否启用了加密
@property int           statusGap;      // 状态信息广播间隔(毫秒)
@property NSData        *manuData;      // 厂商数据

// Other info
@property NSString      *lastSeenDate;  // 最后一次发现的时间
@property NSString      *configDate;    // 最后配置时间
@property NSString      *tagA;          // 标签 A
@property NSString      *tagB;          // 标签 B
@property NSString      *tagC;          // 标签 C

@end

// RLMArray<Beacon>
RLM_ARRAY_TYPE(Beacon)







#pragma mark - Inspection

@interface Inspection : RLMObject

@property NSString  *name;              // 巡检名称, 主键（primaryKey）
@property int       uniId;              // 巡检ID
@property NSString  *creationTime;      // 巡检开始时间
@property RLMArray<Beacon> *beacons;    // 巡检到的设备

@end

// RLMArray<Inspection>
RLM_ARRAY_TYPE(Inspection)
















