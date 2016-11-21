//
//  WSBluetooth.h
//  微智Beacon
//
//  Created by TrusBe Sil on 15/9/18.
//  Copyright © 2015年 we-smart Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import "WSPeripheral.h"

// 筛选设备
typedef enum : NSUInteger {
    ALL_DEVICE = 0,
    CONNECTED_DEVICE,
    BROADCAST_DEVICE,
} ScreeningDevice;

// 加密选项
typedef enum : NSUInteger {
    NOTSUPPORT = 00,    // 不支持加密
    ENCRYPT = 03,       // 已加密
    NOTENCRYPT = 01,    // 未加密
} EncryptStatus;

#pragma mark - WSBlueDelegate
@protocol WSBluetoothDelegate <NSObject>
/**
 *    @author we-smart Co., Ltd.
 *    代理事件，所选设备发送数据是否成功
 */
- (void)allDone:(BOOL)isSucc;


/**
 *    @author we-smart Co., Ltd.
 *    代理事件，当发现新设备、新广播时，代理会被唤起
 */
- (void)listChanged;

@end


#pragma mark - WSBluetooth
@interface WSBluetooth : NSObject<CBCentralManagerDelegate, WSPeripheralDelegate>
@property (nonatomic, weak) id<WSBluetoothDelegate> delegate;


/**
 *    @author we-smart Co., Ltd.
 *    这个值为最小信号值， 根据 RSSI 值筛选周围设备，值范围 -127 ~ -1
 */
@property (nonatomic) NSInteger                             filterNum;

/**
 *    @author we-smart Co., Ltd.
 *    添加设备到扫描列表的选项，默认全部添加（ALL_DEVICE），包括可连接设备（CONNECTED_DEVICE）和广播状态设备（BROADCAST_DEVICE）
 */
@property (nonatomic)ScreeningDevice                        addDeviceMode;


/**
 *    @author we-smart Co., Ltd.
 *    设备对象
 */
@property (nonatomic, strong)NSMutableDictionary           *scannedDevice;

/**
 *    @author we-smart Co., Ltd.
 *    开始搜索蓝牙设备
 */
- (void)startScan;


/**
 *    @author we-smart Co., Ltd.
 *    停止搜索蓝牙设备
 */
- (void)stopScan;


/**
 *    @author we-smart Co., Ltd.
 *    根据设备 UUID 查找该设备
 *    @param perId 设备 UUID
 *    @return 设备信息
 */
- (WSPeripheral *)getItemWithPeripheralUUID:(NSString *)peripheralUUID;


/**
 *    @author we-smart Co., Ltd.
 *    获取扫描到的设备
 *    @return : 返回设备 UUID 数组 (Peripheral UUID)
 */
- (NSArray *)getScannedDevices;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权指定设备并配置 Beacon 广播信息
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 */
- (void)authAndUpdateBeaconInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权指定设备并配置 Beacon 广播信息、功率、广播间隔时间、一米半径信号值
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 *    @param power      Beacon Power    广播功率，范围【0 ~ 7】，功率越大，广播信号半径更大，功耗更大
 *    @param gap        Beacon Gap  广播间隔时间，范围【100 ~ 10000】毫秒，间隔时间越大，每分钟广播次数越少，功耗更少
 *    @param txPower    Beacon TxPower  一米半径信号值，范围【-126 ~ -1】Db，用来计算定位相关距离
 */
- (void)authAndUpdateBeaconInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor Power:(int)power Gap:(int)gap TxPower:(int)txPower;

/**
 *    @author we-smart Co., Ltd.
 *
 *    批量配置接口, 授权指定设备配置 Beacon 和厂商相关信息
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 *    @param power      Beacon Power    广播功率，范围【0 ~ 7】，功率越大，广播信号半径更大，功耗更大
 *    @param gap        Beacon Gap  广播间隔时间，范围【100 ~ 10000】毫秒，间隔时间越大，每分钟广播次数越少，功耗更少
 *    @param txPower    Beacon TxPower  一米半径信号值，范围【-126 ~ -1】Db，用来计算定位相关距离
 *    @param macAddr    设备 Mac 地址, 长度12个字节
 *    @param statusGap  设备厂商信息广播间隔, 默认为5, 表示在低功耗状态下, 每5个 Beacon 广播, 就插入一个厂商自定义信息广播
 *    @param manuData   厂商自定义信息, 长度16个字节
 */
- (void)authAndUpdateInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor Power:(int)power Gap:(int)gap TxPower:(int)txPower MacAddr:(NSString *)macAddr StatusGap:(int)statusGap ManuData:(NSString *)manuData;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权指定设备并修改密码
 *
 *    @param peripheral     指定设备
 *    @param originPassword 设备当前密码,也就是授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param newPassword    设备新密码, 长度8位,
 */
- (void)authAndSettingPasswordWithPeripheral:(WSPeripheral *)peripheral OriginPsw:(NSString *)originPassword NewPsw:(NSString *)newPassword;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权制定设备配置 Beacon 信息并配置密码
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param newPsw     设置的密码，长度8字节
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 */
- (void)authAndUpdateBeaconInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw NewPsw:(NSString *)newPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权指定设备配置 Beacon 信息、功率、广播间隔时间、一米半径信号值并配置新密码
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param newPsw     设置密码
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 *    @param power      Beacon Power    广播功率，范围【0 ~ 7】，功率越大，广播信号半径更大，功耗更大
 *    @param gap        Beacon Gap  广播间隔时间，范围【100 ~ 10000】毫秒，间隔时间越大，每分钟广播次数越少，功耗更少
 *    @param txPower    Beacon TxPower  一米半径信号值，范围【-126 ~ -1】Db，用来计算定位相关距离
 */
- (void)authAndUpdateBeaconInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw NewPsw:(NSString *)newPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor Power:(int)power Gap:(int)gap TxPower:(int)txPower;

/**
 *    @author we-smart Co., Ltd.
 *
 *    批量配置接口, 授权指定设备配置 Beacon 和厂商相关信息, 并设置新密码
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param newPsw     设置密码
 *    @param uuid       Beacon UUID
 *    @param major      Beacon Major
 *    @param minor      Beacon Minor
 *    @param power      Beacon Power    广播功率，范围【0 ~ 7】，功率越大，广播信号半径更大，功耗更大
 *    @param gap        Beacon Gap  广播间隔时间，范围【100 ~ 10000】毫秒，间隔时间越大，每分钟广播次数越少，功耗更少
 *    @param txPower    Beacon TxPower  一米半径信号值，范围【-126 ~ -1】Db，用来计算定位相关距离
 *    @param macAddr    设备 Mac 地址, 长度12个字节
 *    @param statusGap  设备厂商信息广播间隔, 默认为5, 表示在低功耗状态下, 每5个 Beacon 广播, 就插入一个厂商自定义信息广播
 *    @param manuData   厂商自定义信息, 长度16个字节
 */
- (void)authAndUpdateInfoWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw NewPsw:(NSString *)newPsw UUID:(NSData *)uuid Major:(int)major Minor:(int)minor Power:(int)power Gap:(int)gap TxPower:(int)txPower MacAddr:(NSString *)macAddr StatusGap:(int)statusGap ManuData:(NSString *)manuData;

/**
 *    @author we-smart Co., Ltd.
 *
 *    授权指定设备并配置名称信息
 *
 *    @param peripheral 指定设备
 *    @param authPsw    授权密码, 设备已加密则输入设备当前密码, 未加密则使用默认密码『00000000』
 *    @param nameString 名称信息, 第一代支持18个字节, 第二代支持16个字节长度
 */
- (void)authAndUpdateBeaconNameWithPeripheral:(WSPeripheral *)peripheral AuthPsw:(NSString *)authPsw NameString:(NSString *)nameString;

@end










