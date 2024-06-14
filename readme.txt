平台层：
	提供不同平台的差异化实现，包括在Windows、Macos和Linux三个PC平台的实现；另外对图形API（OpenGL、Vulkan…），提供上层窗口系统和Gui支持。

核心层：
	这一层包括一些公共工具、事件处理、配置和渲染、动画和场景管理几个模块。

应用层：
	简单实现一个场景编辑器，还有使用的sample，sandbox等。

pro1：初始化glfw窗体，窗体的基本响应
pro2：日志库，OpenGL报错，Shader类的编写，绘制一个彩色三角形
pro3:  导入glm，实现application，改为多文件夹编译 ，绘制立方体
pro4：stb，绘制纹理，事件处理，绘制带纹理的立方体
pro5：搭建相机系统，优化shader，增加uniform缓冲和查错，事件处理
pro6：重构Application程序入口
pro7：基础几何体，实现冯氏光照模型，搭建材质系统
