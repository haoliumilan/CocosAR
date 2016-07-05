//
//  OneViewController.m
//  CocoaAR
//
//  Created by admin on 7/5/16.
//
//

#import "OneViewController.h"

@interface OneViewController ()

- (void)showBackBtn;
- (void)backBtnCb:(id)sender;

@end

@implementation OneViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.view setBackgroundColor:[UIColor redColor]];
    [self showBackBtn];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (void)showBackBtn {
    UIButton *backBtn = [[UIButton alloc] initWithFrame:CGRectMake(10.0, 30.0, 60.0, 35.0)];
    [self.view addSubview:backBtn];
    [backBtn setImage:[UIImage imageNamed:@"res/btn_back.png"] forState:UIControlStateNormal];
    [backBtn addTarget:self action:@selector(backBtnCb:) forControlEvents:UIControlEventTouchUpInside];
}

- (void)backBtnCb:(id)sender {
    [self dismissViewControllerAnimated:NO completion:nil];
}

@end
