//
//  WSPeripheral.h
//  微智Beacon
//
//  Created by TrusBe Sil on 15/9/18.
//  Copyright © 2015年 we-smart Co., Ltd. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <Realm/Realm.h>

@class WSPeripheral;

#pragma pack(push)
#pragma pack(1)

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

// 蓝牙连接工厂数据
typedef struct {
    u8  protocolVersion;            // 协议版本
    u8  firmVersion;                // 固件版本
    u8  supports;                   // bit0:是否支持加密，bit1:是否启用加密
} NormalFactureData;

/**************************/
// 蓝牙广播工厂数据包头
typedef struct {
    u8  manufacturerId[2];          // 厂商标识
    u8  radioType;                  // 广播类型
    u8  protocolVersion;            // 协议版本
} FactureDataHead;

// 蓝牙广播工厂数据包一
typedef struct {
    FactureDataHead Head;           // 头标识
    u8  beaconUUID[16];              // UUID
    u16 major;                      // major
    u16 minor;                      // minor
    int8_t  txPower;                // txPower
    u8  reserved[4];                // 保留字段,补零
} FactureDataOne;

// 蓝牙广播工厂数据包二
typedef struct {
    FactureDataHead Head;           // 头标识
    u8  firmVersion;                // 固件版本
    u8  deviceModel;                // 设备型号
    u8  status;                     // 状态   0:low power, 1:normal
    u8  supports;                   // 是否支持加密   0:支持, 1:不支持
    u8  battery;                    // 电量百分比    0xFF:检测失败， 0xFE:不带电池
    u16 beaconSendGap;              // 广播间隔， 毫秒
    u8  beaconSendPower;            // 广播功率
    u8  statusSendGap;              // 状态信息广播间隔（本广播的间隔）范围:1-255
    u8  macAddress[6];              // Mac地址
    u8  reserved[2];                // 保留字段, 补零
    u8  manuData[8];                // 厂商自定义数据
} FactureDataTwo;

/**************************/
// Beacon 信息包
typedef struct {
    u16 major;                      // major
    u16 minor;                      // minor
    u8  version;                    // 版本
    u8  power;                      // 功率
    u16 gap;                        // 发送间隔时间
    u16 key;                        // 加密
    u16 majorKey;                   // 加密后的 major
    u16 minorKey;                   // 加密后的 minor
    int8_t  txPower;                // txPower
} BeaconInfo;

/**************************/
// 设置指令包头
typedef struct {
    u8  head;                       // 包头标志信息, 单独一个包的时候 F0, 大于一个包的时候 E0, F1
    u16 targetId;                   // 目标 ID
    u16 sendId;                     // 发送 ID
    u16 sesId;                      // 会话 ID
    u8  cmdId;                      // 命令码
    u8  length;                     // 后面数据的长度
} ShortHeadPacket;

// 设置指令包头
typedef struct {
    u16 targetId;                   // 目标 ID
    u16 sendId;                     // 发送 ID
    u16 sesId;                      // 会话 ID
    u8  cmdId;                      // 命令码
    u8  length;                     // 后面数据的长度
} LongHeadPacket;

// 重命名
typedef struct {
    LongHeadPacket  headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 是否设置，0：设置，1：读取， 目前仅支持设置
    u8  nameLen;                    // 名字长度， 最长16字节
    u8  name[18];                   // 名字 UTF-8编码, 长度16+2个补零
} RenameInfo;

// 加密
typedef struct {
    LongHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 是否设置，0：设置，1：读取， 设置
    u8  originPsw[8];               // 原始密码，如果原始密码未设置，则全部为零
    u8  newPsw[8];                  // 新密码，全部置零表示不加密
} EncryptInfo;

// 验证授权
typedef struct {
    ShortHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  auth;                       // 填写 0x02 ,表 是进 密码授权( 0x2 )
    u8  psw[8];                     // 需要验证的密码。注意,对于尚未设置密码的设备,任何密码都能够验证通过。
} AuthInfo;

// 厂商数据配置
typedef struct {
    ShortHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 设置或读取，0x00：设置厂商自定义数据， 0x01：读取厂商自定义数据
    u8  dataLength;                 // 厂商自定义数据长度，最长8字节 注:固定08
    u8  manuData[8];                // 厂商自定义数据内容
} ManuDataInfo;

// Mac 地址配置
typedef struct {
    ShortHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 设置或读取，0x00：设置厂商自定义数据， 0x01：读取厂商自定义数据
    u8  macAddr[6];                 // 厂商自定义数据内容
} MacAddrInfo;

// 自定义蓝牙广播间隔配置
typedef struct {
    ShortHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 设置或读取，0x00：设置厂商自定义数据， 0x01：读取厂商自定义数据
    u8  bleGap;                     // 自定义蓝牙广播间隔, 范围1-255
} CustomBLEGapInfo;

// 综合广播信息读取
typedef struct {
    ShortHeadPacket headPacket;
    u8  ctrlId;                     // 控制命令
    u8  isSet;                      // 设置或读取，0x00：设置厂商自定义数据， 0x01：读取厂商自定义数据
} ReadAllInfo;

/**************************/
// 返回数据包
typedef struct {
    ShortHeadPacket headPacket;
    u8  responseCode;               // 响应吗
} ReturnInfo;

// 厂商数据返回包
typedef struct {
    ShortHeadPacket headPacket;
    u8  responseCode;               // 响应吗
    u8  dataLength;                 // 厂商自定义数据长度，最长8字节
    u8  manuData[8];                // 厂商自定义数据内容
} ManuDataReturnInfo;

#pragma pack(pop)


/**
 设备连接状态和写数据状态
 */
enum {
    STAT_DISCONNECTED = 0,
    STAT_CONNECTED,
    
    WRITE_WAITING = 0,
    WRITE_FAILED,
    WRITE_SUCC,
};


#pragma mark - WSPerDelegate

@protocol WSPeripheralDelegate <NSObject>

/**
 *    @author we-smart Co., Ltd.
 *    写入状态回调
 *    @param isSucc 是否成功
 */
- (void)written:(BOOL)isSucc;

/**
 *    @author we-smart Co., Ltd.
 *    状态改变代理
 *    @param per 设备
 */
- (void)stateChanged:(WSPeripheral *)per;

@end



#pragma mark - WSPeripheral
@interface WSPeripheral : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property (nonatomic, weak) id<WSPeripheralDelegate> delegate;

@property (nonatomic, strong) CBPeripheral      *myPeripheral;
@property (nonatomic, strong) CBCentralManager  *myCentralManager;


/**********************************************************/
// 设备名称
@property (nonatomic, strong) NSString          *perName;
// 设备 UUID
@property (nonatomic, strong) NSUUID            *uniId;
// 设备 协议版本
@property (nonatomic) int                       protocolVersion;
// 设备 固件版本
@property (nonatomic) int                       firmVersion;
// 设备 是否加密
@property (nonatomic) BOOL                      isEncrypt;
// 设备 原始密码
@property (nonatomic) NSString                  *originPsw;
// 设备 新密码
@property (nonatomic) NSString                  *updatePsw;

/**********************************************************/
// 设备广播 - iBeacon UUID
@property (nonatomic, strong) NSUUID            *beaconUUID;
// 设备广播 - Major
@property (nonatomic) int                       majorId;
// 设备广播 - Minor
@property (nonatomic) int                       minorId;
// 设备广播 - Mac 地址
@property (nonatomic, strong) NSData            *macAddress;
// 设备广播 - 设备功率 默认:7
@property (nonatomic) int                       power;
// 设备广播 - iBeacon 信息间隔 默认:2000
@property (nonatomic) int                       gap;
// 设备广播 - iBeacon 信息一米距离 RSSI 信号值
@property (nonatomic) int                       txPower;
// 设备广播 - 电池电量
@property (nonatomic) int                       battery;
// 设备广播 - 设备型号
@property (nonatomic) int                       deviceModel;
// 设备广播 - 电池状态  0：低功耗， 1：正常蓝牙广播状态
@property (nonatomic) int                       batteryStatus;
// 设备广播 - 设备是否支持加密 bit0：支持，bit1: 不支持
@property (nonatomic) int                       supports;
// 设备广播 - 状态信息广播间隔比例, 如:设置为5表示, Beacon广播5次, 然后广播一次自定义蓝牙广播
@property (nonatomic) int                       statusGap;
// 设备广播 - 厂商自定义数据
@property (nonatomic, strong) NSData            *manuData;

/**********************************************************/
// 信号值
@property (nonatomic) int                       rssiNumber;
// 设备状态
@property (nonatomic) int                       status;
// 写入数据状态
@property (nonatomic) int                       writeSucc;
// 连接设备 true:连接，false:断开连接；
@property (nonatomic) BOOL                      reqConfigBeacon;



- (instancetype)initWithPeripheral:(CBPeripheral *)per centralManager:(CBCentralManager *)central rssi:(double)rssi;
- (void)authAndWriteBeaconInfoWithAuthPsw:(NSString *)authPsw UUID: (NSData *)uuidData Major: (int)ma Minor:(int)mi Power:(int)po Gap:(int)ga TxPower:(int)txPo;
- (void)authAndWriteBeaconInfoWithAuthPsw:(NSString *)authPsw NewPsw:(NSString *)newPsw UUID: (NSData *)uuidData Major: (int)ma Minor:(int)mi Power:(int)po Gap:(int)ga TxPower:(int)txPo;
- (void)authAndRenameDeviceWithAuthPsw:(NSString *)authPsw Name:(NSString *)nameStr;
- (void)authAndSetPasswordWithOriginPsw:(NSString *)originPassword NewPsw:(NSString *)newPassword;
- (void)authAndSetMacAddrWithAuthPsw:(NSString *)authPsw MacAddr:(NSString *)macAddr;
- (void)authAndSetManuDataWithAuthPsw:(NSString *)authPsw ManuStr:(NSString *)manuStr;
- (void)authAndGetManuDataWithAuthPsw:(NSString *)authPsw ManuStr:(NSString *)manuStr;
- (void)authAndSetCustBleGapWithAuthPsw:(NSString *)authPsw CustBleGap:(int)custBleGap;
- (void)authAndWriteInfoWithAuthPsw:(NSString *)authPsw UUID:(NSData *)uuidData Major:(int)ma Minor:(int)mi Power:(int)po Gap:(int)ga TxPower:(int)txPo MacAddr:(NSString *)mac StatusGap:(int)stGa ManuData:(NSString *)manu;
- (void)authAndWriteInfoWithAuthPsw:(NSString *)authPsw NewPsw:(NSString *)newPsw UUID:(NSData *)uuidData Major:(int)ma Minor:(int)mi Power:(int)po Gap:(int)ga TxPower:(int)txPo MacAddr:(NSString *)mac StatusGap:(int)stGa ManuData:(NSString *)manu;


/**
 *    @author we-smart Co., Ltd.
 *
 *    保存数据到数据库
 *
 */
- (void)saveDataToRealm:(RLMRealm *)realmDB WSPeripheral:(WSPeripheral *)peripheral Mac:(NSData *)mac protocolVersion:(int)protV FirmVersion:(int)firmV
              OriginPsw:(NSString *)oriPsw NewPsw:(NSString *)newPs
                   Uuid:(NSString *)uuid Major:(int)ma Minor:(int)mi Gap:(int)ga Power:(int)pow Txpower:(int)txpow
            DeviceModel:(int)devMod Battery:(int)bat BatteryStatus:(int)batStatus Support:(int)sup StatusGap:(int)statGap ManuData:(NSData *)manu
           LastSeenDate:(NSString *)seen ConfigDate:(NSString *)config TagA:(NSString *)tagA TagB:(NSString *)tagB TagC:(NSString *)tagC;


@end










