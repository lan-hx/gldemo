cmake_minimum_required(VERSION 3.15)

qt_add_resources(gldemo "gldemo_images"
        PREFIX "/"
        FILES
            texture/wall.jpg
            texture/qtlogo.png
            texture/white.png
            model/cube.obj
            model/sphere.obj
            model/bunny.obj
            model/rose.obj
        )
