## 为防止抄袭，项目信息待补

---

#### 编译指南

如果对编译过程有疑问，不妨看看GitHub CI(Actions)的构建过程和脚本。

1. 安装Qt6：百度一下

另一种方法（不想注册Qt账号）：Qt也可以使用vcpkg从source构建，根据CPU需要耗费数十分钟至数个小时不等。待补

2. Configure CMake：

指定以下变量：QT_PREFIX_DIR(Qt SDK directory), VCPKG_DIR(should be 'vcpkg'), VCPKG_TARGET_TRIPLET(vcpkg presets)

当然，不使用vcpkg包管理器也是可以的，不要设置VCPKG_DIR，自己设置好toolchain即可。Linux上不建议使用vcpkg。

例子：
```shell
cmake -B build -DQT_PREFIX_DIR=C:/Qt/6.4.1/mingw_64 -DVCPKG_DIR=vcpkg -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
```

3. Build

```shell
cd build
cmake --build . --target all
```

4. test(暂不支持)

```shell
cmake --build . --target gldemo_test
```

---

#### shader管理

本项目所有shader放置在shader文件夹下，CMake在编译时会自动读取shader并打包成头文件，放在shader/include文件夹下，项目中直接使用该头文件即可。

例如，shader/example.vert会自动编译为shader/include/example_vert.h，里面含有char *shader_example_vert可供使用。

---

#### 使用Qt框架和GLAD等库的区别

1. 窗口本身由Qt管理，不使用glfw等框架

2. OpenGL的函数符号链接由Qt完成，不需要GLAD。

3. 线性代数相关计算API由Qt提供，不需要使用GLM。

4. 图像（用于纹理）相关API由Qt提供，不需要stb_image。

5. 键盘鼠标等事件由Qt接管。

4. 为跨平台考虑，Qt使用OpenGL ES而非OpenGL。

5. QT提供了对OpenGL API的简单对象包装，将C函数包装为相应对象的成员函数，与助教提供的框架大同小异，例如对于shader program，提供了attach shader, link, use, setUniform等方法。

6. 当然，在OpenGL层不使用Qt的任何设施也是可以的。

7. Qt实现了对OpenGL的高阶包装，但是考虑到课程作业的目标，本项目不会使用这些高级API。

---

#### 单元测试

暂未实现

---

#### LICENSE

GPLv3