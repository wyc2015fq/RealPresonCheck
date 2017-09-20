//
//  RMFace.h
//  O1
//
//  Created by duanhai on 2017/9/20.
//  Copyright © 2017年 duanhai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RMFace : NSObject
@property(nonatomic,copy) NSString *name;
@property(nonatomic,copy) NSString *info;
@property(nonatomic) BOOL checkStatus;
@property(nonatomic) unsigned int faceAction;
@end
