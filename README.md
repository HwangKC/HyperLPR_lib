"# HyperLPR_lib" 

基于开源项目HyperLPR群里的hyperlpr-e2e-cv340-vs2013-console项目重新进行包装成分离的lib库和demo

原项目地址 https://github.com/zeusees/HyperLPR/tree/master/Prj-Win

版权归原作者，如侵立删

VS2015+opencv3.4.1下编译运行通过,static library形式编译

下载后需要在项目里修改opencv库的incude和lib目录设置

lprdemo编译后自动输出到output

lprdemo命令行格式：

lprdemo image_path_name [resize_to_width|0]

example:
识别d:\1.jpg,识别时转换为最大边长1080，如果没有1080大，则保持原尺寸
lprdemo d:\1.jpg 1080 


