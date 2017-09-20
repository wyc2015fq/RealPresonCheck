//
//  UIImage+Helper.h
//  RMFaceID
//
//  Created by luhai on 2017/8/10.
//  Copyright © 2017年 com.remarkmedia. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Helper)
- (NSString *)saveImage:(UIImage *)tempImage WithName:(NSString *)imageName;
+ (UIImage *)imageFromColor:(UIColor *)color;
- (UIImage *)imageInRect:(CGRect)rect;
- (UIColor *)averageColor:(CGRect)rect;
- (UIImage*)rotate:(UIImageOrientation)orient;
- (UIImage *)rescaleImageToSize:(CGSize)size;
@end
