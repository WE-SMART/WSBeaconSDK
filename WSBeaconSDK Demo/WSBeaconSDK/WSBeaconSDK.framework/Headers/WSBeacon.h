//
//  WSBeacon.h
//  微智Beacon
//
//  Created by TrusBe Sil on 16/4/19.
//  Copyright © 2016年 we-smart Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

#pragma mark - WSBeaconDelegate
@protocol WSBeaconDelegate <NSObject>

/**
 *    @author we-smart Co., Ltd.
 *
 *    Return All iBeacon Devices Found.
 *
 *    @param beacons : CLBeacon infomation
 */
- (void)foundBeacons:(NSArray *)beacons;

@end



#pragma mark - WSBeacon
@interface WSBeacon : NSObject

@property (nonatomic, weak)id<WSBeaconDelegate> delegate;

- (instancetype)initWithProximityUUID:(NSUUID *)proximityUUID major:(CLBeaconMajorValue)major minor:(CLBeaconMinorValue)minor identifier:(NSString *)identifier;

- (void)starScan;

- (void)stopScan;

/**
 *    @author we-smart Co., Ltd.
 *
 *    Find iBeacon with UUID
 *
 *    @param proximityUUID : iBeacon UUID
 *    @param identifier    : identifier (Can be empty)
 */
- (void)foundBeaconWithProximityUUID:(NSUUID *)proximityUUID identifier:(NSString *)identifier;

/**
 *    @author we-smart Co., Ltd.
 *
 *    Find iBeacon with UUID and Major
 *
 *    @param proximityUUID : iBeacon UUID
 *    @param major         : iBeacon major
 *    @param identifier    : identifier (Can be empty)
 */
- (void)foundBeaconWithProximityUUID:(NSUUID *)proximityUUID major:(CLBeaconMajorValue)major identifier:(NSString *)identifier;

/**
 *    @author we-smart Co., Ltd.
 *
 *    Find iBeacon with UUID and Major and Minor
 *
 *    @param proximityUUID : iBeacon UUID
 *    @param major         : iBeacon major
 *    @param minor         : iBeacon minor
 *    @param identifier    : identifier (Can be empty)
 */
- (void)foundBeaconWithProximityUUID:(NSUUID *)proximityUUID major:(CLBeaconMajorValue)major minor:(CLBeaconMinorValue)minor identifier:(NSString *)identifier;

@end
