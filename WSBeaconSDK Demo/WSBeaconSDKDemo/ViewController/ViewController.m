//
//  ViewController.m
//  WSBeaconSDKDemo
//
//  Created by TrusBe Sil on 16/7/25.
//  Copyright © 2016年 we-smart Co., Ltd. All rights reserved.
//

#import "ViewController.h"
#import "DeviceCell.h"
#import <WSBeaconSDK/WSBeaconSDK.h>

@interface ViewController ()<WSBluetoothDelegate, UITableViewDelegate, UITableViewDataSource> {
    WSBluetooth         *bluetooth;
    NSMutableArray      *deviceArray;
    WSPeripheral        *peripheral;
    
    NSString *uuidStr, *majorStr, *minorStr, *powerStr, *gapStr, *mRssiStr, *macStr, *statusGap, *manuStr, *newPsw, *authPsw;
}

@property (weak, nonatomic) IBOutlet UITableView *deviceTableView;

@end

@implementation ViewController

#pragma mark - Life Cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    // Init
    [self initPropertys];

    // View
    [self loadMainView];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [bluetooth stopScan];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}

#pragma mark - Init
- (void)initPropertys {
    bluetooth = [[WSBluetooth alloc] init];
    bluetooth.delegate = self;
    bluetooth.addDeviceMode = ALL_DEVICE;
    peripheral = [[WSPeripheral alloc] init];
    deviceArray = [NSMutableArray new];
    
    // 测试数据
    uuidStr = @"FDA50493-A4E2-4FB1-AFCF-C6EB07647888";
    majorStr = @"10048";
    minorStr = @"8888";
    powerStr = @"7";
    gapStr = @"2000";
    mRssiStr = @"-59";
    macStr = @"888888887788"; // 长度限制12位
    statusGap = @"3"; // 低功耗蓝牙广播间隔比率，设置为3表示，三个 Beacon 广播，插播一个自定义蓝牙广播；
    manuStr = @"1234567890123456";
    newPsw = @"00000000";
    authPsw = @"00000000";
    
    [bluetooth startScan];
}

#pragma mark - View
- (void)loadMainView {
}

#pragma mark - UITableViewDataSource And UITableViewDelegate
-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 70.0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return deviceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellWithIdentifier = @"DeviceCell";
    DeviceCell *cell = (DeviceCell *)[tableView dequeueReusableCellWithIdentifier:CellWithIdentifier];
    @synchronized(deviceArray) {
        WSPeripheral *per = [bluetooth getItemWithPeripheralUUID: deviceArray[indexPath.row]];
        NSString *support = [NSString new];
        if (per.supports == 1) {
            support = @"NotEncrypt";
        } else if (per.supports == 3) {
            support = @"Encrypt";
        } else {
            support = @"NotSupportPSW";
        }
        
        NSString *battery = [NSString new];
        if (per.battery == 254 || per.battery < 0) {
            battery = @"No Battery";
        } else if (per.battery == 255) {
            battery = @"Battery: Error";
        } else {
            battery = [NSString stringWithFormat:@"Battery: %d%%", per.battery];
        }
        
        
        NSString *string = [NSString stringWithFormat:@"Name: %@\nMac: %@\nMajor: %d, Minor: %d, 加密状态: %@\nGap: %d, Power: %d, MRssi: %d, 电池状态: %@",
                            per.perName,
                            [self dataToMacTypeString:per.macAddress],
                            per.majorId,
                            per.minorId,
                            support,
                            per.gap,
                            per.power,
                            per.txPower,
                            battery];
        cell.nameLabel.text = string;
        return cell;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
//    DeviceCell *cell = (DeviceCell *)[tableView cellForRowAtIndexPath:indexPath];
    peripheral = [bluetooth getItemWithPeripheralUUID: deviceArray[indexPath.row]];
    peripheral.reqConfigBeacon = YES;
    
    NSString *string = [NSString stringWithFormat:@"UUID:%@,\nMajor:%@, Minor:%@, \nPower:%@, Gap:%@, MRssi:%@,\nMac:%@, StatusGap:%@,\nManuData:%@", uuidStr, manuStr, minorStr, powerStr, gapStr, mRssiStr, macStr, statusGap, manuStr];
    
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Cofig Data" message:string preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        [self dismissViewControllerAnimated:YES completion:nil];
    }];
    UIAlertAction *settingAction = [UIAlertAction actionWithTitle:@"Cofig" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        NSString *string = [uuidStr stringByReplacingOccurrencesOfString:@"-" withString:@""];
        unsigned char uuid[16] = { 0, }, cstr[64] = {0};
        [string getCString:cstr maxLength:64 encoding: NSASCIIStringEncoding];
        for (int idx = 0; idx < 16; idx++) {
            uuid[idx] = (asc2bin( cstr[idx << 1] ) << 4) | asc2bin(cstr[(idx << 1) + 1]);
        }
        
        NSData *uuidData = [NSData dataWithBytes:uuid length:sizeof(uuid)];
        
        // 配置指令
        [bluetooth authAndUpdateInfoWithPeripheral:peripheral
                                           AuthPsw:authPsw
                                            NewPsw:newPsw
                                              UUID:uuidData
                                             Major:[majorStr intValue]
                                             Minor:[minorStr intValue]
                                             Power:[powerStr intValue]
                                               Gap:[gapStr intValue]
                                           TxPower:[mRssiStr intValue]
                                           MacAddr:macStr
                                         StatusGap:[statusGap intValue]
                                          ManuData:manuStr];
    }];
    [alert addAction:cancelAction];
    [alert addAction:settingAction];
    [self presentViewController:alert animated:YES completion:nil];
}

#pragma mark - WSBluetoothDelegate
- (void)allDone:(BOOL)isSucc {
    if (isSucc == true) {
        NSLog(@"allDone, Setting Success.");
    } else {
        NSLog(@"allDone, Setting Fail.");
    }
}

- (void)listChanged {
    // 扫描到新设备更新列表
    @synchronized(self.deviceTableView) {
        deviceArray = [[NSMutableArray alloc] initWithArray:[bluetooth getScannedDevices]];
    }
    // 默认情况下更新数据
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.deviceTableView reloadData];
    });
}

#pragma mark - Actions
- (NSString *)dataToMacTypeString:(NSData *)data {
    NSUInteger capacity = data.length * 2;
    NSMutableString *sbuf = [NSMutableString stringWithCapacity:capacity];
    const unsigned char *buf = data.bytes;
    NSInteger i;
    for (i=0; i<data.length; ++i) {
        i == 0 ? [sbuf appendFormat:@"%02lX", (unsigned long)buf[i]] : [sbuf appendFormat:@":%02lX", (unsigned long)buf[i]];
    }
    return sbuf;
}

unsigned char asc2bin(unsigned char c) {
    if ('a' <= c && c <= 'f' )
    return c - 'a' + 10;
    else if ( 'A' <= c && c <= 'F' )
    return c - 'A' + 10;
    else if ('0' <= c && c <= '9')
    return c - '0';
    else return 0;
}

// 开始扫描设备
- (IBAction)startScanAction:(id)sender {
    [deviceArray removeAllObjects];
    [bluetooth.scannedDevice removeAllObjects];
    [self.deviceTableView reloadData];

    [bluetooth startScan];
    @synchronized(self.deviceTableView) {
        deviceArray = [[NSMutableArray alloc] initWithArray:[bluetooth getScannedDevices]];
        NSLog(@"deviceArray: %@", deviceArray);
    }
    [self.deviceTableView reloadData];
}

// 停止扫描设备
- (IBAction)stopScanAction:(id)sender {
    [bluetooth stopScan];
}










- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

@end
