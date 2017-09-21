//
//  ViewController.m
//  O1
//
//  Created by duanhai on 2017/8/31.
//  Copyright © 2017年 duanhai. All rights reserved.
//

#import "ViewController.h"
#import <RMSDMLandMark/rm_mobile_face.h>
#include <iostream>
#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>
#import "UIImage+Helper.h"
#import <Photos/Photos.h>
#import "RMFace.h"
#import <AudioToolbox/AudioToolbox.h>
#define TICK(time) NSDate *time = [NSDate date]
#define TOCK(time) NSLog(@"%s: %f", #time, -[time timeIntervalSinceNow] * 1000)

@interface ViewController ()<AVCaptureVideoDataOutputSampleBufferDelegate>
{
    int faces_count;

    rm_handle_t handle;
    rm_mobile_face_action_t *p_face_action_array;
    CGFloat _imageOnPreviewScale;
    CGFloat _previewImageWidth;
    CGFloat _previewImageHeight;
}
@property (nonatomic , strong) AVCaptureVideoPreviewLayer *captureVideoPreviewLayer;
@property (nonatomic , strong) AVCaptureDevice *videoDevice;
@property (nonatomic,strong) UILabel *poseResultLabel;
@property (nonatomic,strong) UILabel *infoLabel;
@property (nonatomic) int count;
@property (nonatomic) int savePic2CameraRollCount;
@property (nonatomic,strong) NSArray *rmFaceArray;
@property (nonatomic) int hasFinishStatus;
@property (nonatomic) dispatch_queue_t queue;
@property (nonatomic) BOOL allowNext;
@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.count = 0;
    self.hasFinishStatus = 0;
    self.queue = dispatch_queue_create("com.duanhai.test", NULL);
    NSString *path = [[NSBundle mainBundle] resourcePath];
    rm_result_t rt = rm_mobile_tracker_106_create(path.UTF8String,0,&handle);
    if(rt != RM_OK)
    {
        std::cout << "create handle failed " << std::endl;
    }else {
        std::cout << "Load model succeed!" << std::endl;
    }
    
    self.rmFaceArray = [[NSMutableArray alloc] initWithCapacity:4];
    RMFace *face0 = [[RMFace alloc] init];
    face0.name = @"眨眼";
    face0.faceAction = 0x00000002;
    face0.info = @"请眨眼";
    face0.audioName = @"blink";
    
    RMFace *face1 = [[RMFace alloc] init];
    face1.name = @"摇头";
    face1.faceAction = 0x00000008;
    face1.info = @"请摇头";
    face1.audioName = @"head_yaw";
    
    RMFace *face2 = [[RMFace alloc] init];
    face2.name = @"点头";
    face2.faceAction = 0x00000010;
    face2.info = @"请点头";
    face2.audioName = @"head_pitch";
    
    RMFace *face3= [[RMFace alloc] init];
    face3.name = @"张嘴";
    face3.faceAction = 0x0000004;
    face3.info = @"请张嘴";
    face3.audioName = @"open_mouth";

    
    NSArray *originArray = @[face0,face1,face2,face3];
    
    self.rmFaceArray = [self getRandomServiceWithList:originArray];
    
    [self setupAndStartCapture];
    
    //延时一定时间后开启下一帧检测开关
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        self.allowNext = YES;
    });
    
    
    // Do any additional setup after loading the view, typically from a nib.
}





- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    
    
    CVPixelBufferRef pixelBuffer = (CVPixelBufferRef)CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    void *baseAddress = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    int iBytesPerRow =(int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    int iHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    int iWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    
    size_t iTop , iBottom , iLeft , iRight;
    CVPixelBufferGetExtendedPixels(pixelBuffer, &iLeft, &iRight, &iTop, &iBottom);
    
    iWidth = iWidth + (int)iLeft + (int)iRight;
    iHeight = iHeight + (int)iTop + (int)iBottom;
    //    rm_mobile_tracker_106_track
    
    rm_result_t rt1 = rm_mobile_tracker_106_track_face_action
    (
     handle,
     (unsigned char *)baseAddress,
     RM_PIX_FMT_NV12,
     iWidth,
     iHeight,
     iBytesPerRow,
     RM_CLOCKWISE_ROTATE_90,
     &p_face_action_array,
     &faces_count);
    if (faces_count > 0) {

            rm_mobile_106_t stFace = p_face_action_array[faces_count - 1].face;
            unsigned int iFaceAction = p_face_action_array[faces_count - 1].face_action;
            
            NSString *strLastFacePose = [NSString stringWithFormat:@"pose : yaw=%f , pitch=%f\nroll=%f , eye_dist=%f", stFace.yaw , stFace.pitch , stFace.roll , stFace.eye_dist];
            NSString *strLastFaceAction = [NSString stringWithFormat:@"动作检测: 眨眼:%d\n 张嘴:%d , 摇头:%d , 点头:%d" ,
                                           CHECK_FLAG(iFaceAction, RM_MOBILE_EYE_BLINK),
                                           CHECK_FLAG(iFaceAction, RM_MOBILE_MOUTH_AH),
                                           CHECK_FLAG(iFaceAction, RM_MOBILE_HEAD_YAW),
                                           CHECK_FLAG(iFaceAction, RM_MOBILE_HEAD_PITCH)];
        
                //间隔5帧判断一次
                self.count ++;
                if (self.count < 6){
                    return;
                }else {
                    self.count == 0;
                }
                
                if (self.rmFaceArray.count > 0 && self.hasFinishStatus < 4) {
                    RMFace *aFace = self.rmFaceArray[self.hasFinishStatus];
                   
                    //播放语音
                    if(!aFace.checkStatus){
                        aFace.checkStatus = YES;
                        [self playAudio:aFace.audioName];
                    }
                    
                    if (self.allowNext) {
                        if (CHECK_FLAG(iFaceAction, aFace.faceAction)) {
                            self.hasFinishStatus ++;
                        }
                        self.allowNext = NO;
                        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.8 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                            self.allowNext = YES;
                        });
                    }
                    NSLog(@"---%d",iFaceAction);
                    dispatch_async(dispatch_get_main_queue(), ^{
                        self.infoLabel.text = aFace.info;
                    });
                }
                
                if(self.hasFinishStatus == 4) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        self.infoLabel.text = @"验证通过";
                        [self playAudio:@"check_success"];
                        self.hasFinishStatus ++;
                    });
                }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            self.poseResultLabel.hidden = NO;
            self.poseResultLabel.text = [NSString stringWithFormat:@"%@\n%@" ,
                                         strLastFacePose ,
                                         strLastFaceAction];
        } );
        
    }else {
        
        dispatch_async(dispatch_get_main_queue(), ^{
            self.poseResultLabel.hidden = YES;
        } );
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    
}



- (void)setupAndStartCapture
{
    AVCaptureSession *session = [[AVCaptureSession alloc] init];
    // Set the camera preview size
    session.sessionPreset = AVCaptureSessionPreset640x480;
    CGFloat imageWidth = 480;
    CGFloat imageHeight = 640;
    
    // Get the preview frame size.
    self.captureVideoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:session];
    self.captureVideoPreviewLayer.frame = self.view.bounds;
    CGFloat previewWidth = self.captureVideoPreviewLayer.frame.size.width;
    CGFloat previewHeight = self.captureVideoPreviewLayer.frame.size.height;
    [self.captureVideoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    [self.view.layer addSublayer:self.captureVideoPreviewLayer];
    
    // Calculate the width, height and scale rate to display the preview image
    _imageOnPreviewScale = MAX(previewHeight/imageHeight, previewWidth/imageWidth);
    _previewImageWidth = imageWidth * _imageOnPreviewScale;
    _previewImageHeight = imageHeight * _imageOnPreviewScale;
    
    
    self.poseResultLabel = [[UILabel alloc]initWithFrame:CGRectMake(15, 30, self.view.frame.size.width - 30, 75)];
    self.poseResultLabel.numberOfLines = 0;
    self.poseResultLabel.textAlignment = NSTextAlignmentCenter;
    self.poseResultLabel.backgroundColor = [UIColor whiteColor];
    self.poseResultLabel.alpha = 0.7;
    self.poseResultLabel.textColor = [UIColor blackColor];
    self.poseResultLabel.font = [UIFont systemFontOfSize:14.0f];
    [self.poseResultLabel setAdjustsFontSizeToFitWidth:YES];
    [self.view addSubview:self.poseResultLabel];
    
    

    
    
    UIImageView *coverCircle = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    coverCircle.image = [UIImage imageNamed:@"Combined Shape"];
    [self.view addSubview:coverCircle];
    
    UIImageView *labelCoverView = [[UIImageView alloc] initWithFrame:CGRectMake(self.view.frame.size.width/2 - 210/2, 470, 210, 72)];
    labelCoverView.image = [UIImage imageNamed:@"button"];
    [self.view addSubview:labelCoverView];
    
    self.infoLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 15, 200, 40)];
    self.infoLabel.textColor = [UIColor whiteColor];
    self.infoLabel.textAlignment = NSTextAlignmentCenter;
    self.infoLabel.font = [UIFont boldSystemFontOfSize:14];
    self.infoLabel.text = @"请讲人脸放入原型框内";
    [labelCoverView addSubview:self.infoLabel];

    
    NSArray *devices = [AVCaptureDevice devices];
    for (AVCaptureDevice *device in devices) {
        
        if ([device hasMediaType:AVMediaTypeVideo]) {
            
            if ([device position] == AVCaptureDevicePositionFront) {
                
                self.videoDevice = device;
            }
        }
    }
    
    NSError *error = nil;
    AVCaptureDeviceInput *videoInput = [AVCaptureDeviceInput deviceInputWithDevice:self.videoDevice error:&error];
    
    if (!videoInput) {
        
        NSLog(@"trying to open camera: %@",error);
        
        return;
    }
    
    AVCaptureVideoDataOutput * dataOutput = [[AVCaptureVideoDataOutput alloc] init];
    
    [dataOutput setAlwaysDiscardsLateVideoFrames:YES];
    [dataOutput setVideoSettings:@{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)}];
    dispatch_queue_t queue = dispatch_queue_create("dataOutputQueue", NULL);
    [dataOutput setSampleBufferDelegate:self queue:queue];
    
    [session beginConfiguration];
    
    if ([session canAddInput:videoInput]) {
        [session addInput:videoInput];
    }
    if ([session canAddOutput:dataOutput]) {
        [session addOutput:dataOutput];
    }
    [session commitConfiguration];
    
    [session startRunning];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



#pragma mark - 随机排序
- (NSArray*)getRandomServiceWithList:(NSArray*)array
{
    NSMutableSet *randomSet = [[NSMutableSet alloc] init];
    while ([randomSet count] < 4) {
        int r = arc4random() % [array count];
        [randomSet addObject:[array objectAtIndex:r]];
    }
    return [randomSet allObjects];
}

#pragma mark - 播放系统声音
- (void)playAudio:(NSString *)name {
    SystemSoundID ID;
    NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];
    AudioServicesCreateSystemSoundID((__bridge CFURLRef _Nonnull)(url), &ID);
    AudioServicesPlayAlertSound(ID);
}

#pragma mark - 截取图片到相册
- (void)savePic2CameraRoll:(CVPixelBufferRef)pixelBuffer {
    
                CIImage *ciImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
                CIContext *temporaryContext = [CIContext contextWithOptions:nil];
                CGImageRef videoImage = [temporaryContext
                                         createCGImage:ciImage
                                         fromRect:CGRectMake(0, 0,
                                                             CVPixelBufferGetWidth(pixelBuffer),
                                                             CVPixelBufferGetHeight(pixelBuffer))];
    
                UIImage *trackIamge = [UIImage imageWithCGImage:videoImage];
                trackIamge = [trackIamge rotate:UIImageOrientationRightMirrored];
                self.savePic2CameraRollCount ++;
                if (self.savePic2CameraRollCount == 3){
                    [self saveVideoToLibrary:trackIamge];
                    self.savePic2CameraRollCount = 0;
                }
                CGImageRelease(videoImage);
    
    
}

#pragma mark - 保存图片到相册
- (void)saveVideoToLibrary:(UIImage *)image
{
    [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
        if (status != PHAuthorizationStatusAuthorized) {
            NSLog(@"failed to authorize Photo library access\n");
        }
        else {
            
            
            [[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
                PHAssetChangeRequest* request = [PHAssetChangeRequest creationRequestForAssetFromImage:image];
                if (!request)
                    NSLog(@"failed to create change request");
            }
                                              completionHandler:^(BOOL success, NSError* error) {
                                                  if (!success) {
                                                      NSLog(@"failed to add pictures to Photos library: %@", error.description);
                                                  }else {
                                                      
                                                      UIAlertController *alertCtr = [UIAlertController alertControllerWithTitle:@"" message:@"Save successfully" preferredStyle:UIAlertControllerStyleAlert];
                                                      
                                                      UIAlertAction *act1 = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                                                          //
                                                      }];
                                                      
                                                      dispatch_async(dispatch_get_main_queue(), ^{
                                                          
                                                          [alertCtr addAction:act1];
                                                          [self presentViewController:alertCtr animated:YES completion:nil];
                                                      });
                                                      
                                                      
                                                      
                                                  }
                                              }];
        }
    }];
}


@end
