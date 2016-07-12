#Target
* 将镜头中的数据和AR识别的结果（目标名字、坐标变换等）传递到cocos2dx
* 如果上述目标没能实现，可以考虑直接从cocos2dx跳转到oc，通过oc代码来显示3d素材。这种方法还需要考虑android平台的问题，这是下策。


# 1.00 
## 20160712
第一次使用，用来记录CocosAR的开发过程。   
目前可以支持从cocos2d界面跳转到oc界面来使用AR，oc界面现在可以显示摄像头拍摄的画面，识别target，但是不能显示出那个茶壶。   
ImageTargetsEAGLView不add到父节点上，renderFrameVuforia方法将不会被vuforia自动调用，但这个时候可以手动获取到AR扫描的结果。根据log的显示，龙狼AR应该是这个思路，下一步就是要把镜头中的纹理传到cocos2dx的代码中。
