//
//  WSBeaconsViewController.m
//  WSBeaconSDKDemo
//
//  Created by TrusBe Sil on 2016/10/14.
//  Copyright © 2016年 we-smart Co., Ltd. All rights reserved.
//

#import "WSBeaconsViewController.h"
#import <WSBeaconSDK/WSBeaconSDK.h>
#import "DeviceCell.h"

@interface WSBeaconsViewController ()<WSBeaconDelegate> {
    WSBeacon            *beacon;
    NSMutableArray      *deviceArray;
}

@property (weak, nonatomic) IBOutlet UITableView *deviceTableView;

@end

@implementation WSBeaconsViewController

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
    [beacon stopScan];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}

#pragma mark - Init
- (void)initPropertys {
    deviceArray = [NSMutableArray new];

    NSUUID *uuid = [[NSUUID alloc] initWithUUIDString:@"FDA50693-A4E2-4FB1-AFCF-C6EB07647825"];
    uint16_t major = 10034;
    uint16_t minor = 29908;
    beacon = [[WSBeacon alloc] initWithProximityUUID:uuid major:major minor:minor identifier:@""];
    beacon.delegate = self;
    
    [beacon starScan];
}

#pragma mark - View
- (void)loadMainView {
    self.title = @"iBeacons";
}

#pragma mark - UITableViewDataSource And UITableViewDelegate
-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 70.0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return deviceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CMainCell = @"CMainCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CMainCell];
    cell.textLabel.numberOfLines = 0;
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault  reuseIdentifier: CMainCell];
    }
    @synchronized(deviceArray) {
        CLBeacon *tempBeacon = deviceArray[indexPath.row];
        NSString *string = [NSString stringWithFormat:@"UUID: %@\nMajor: %@, Minor: %@, RSSI: %ld", [tempBeacon.proximityUUID UUIDString], tempBeacon.major, tempBeacon.minor, (long)tempBeacon.rssi];
        cell.textLabel.text = string;
        return cell;
    }
}

#pragma mark - WSBeaconDelegate
- (void)foundBeacons:(NSArray *)beacons {
    deviceArray = [[NSMutableArray alloc] initWithArray:beacons];
    NSLog(@"Home, foundBeacons, beacons: %@， deviceArray: %@, ", beacons, deviceArray);
    [_deviceTableView reloadData];
}


#pragma mark - Actions
// 开始扫描设备
- (IBAction)startScanAction:(id)sender {
    [deviceArray removeAllObjects];
    [self.deviceTableView reloadData];
    
    [beacon starScan];
    [self.deviceTableView reloadData];
}

// 停止扫描设备
- (IBAction)stopScanAction:(id)sender {
    [beacon stopScan];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


@end
