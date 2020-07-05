# OpenSource-Radar-GUI
An Open Source Graphical User Interface(GUI) for any Radar application having Plan Position Indicator(PPI) display.

![Hello World](https://simarmannsingh.de/img/2018/11/SampleWork_bySimarMannSingh-e1545065471700.gif)

## About
OpenSource-Radar-GUI is an immediate mode Graphical User Interface which uses OpenGL for rendering the graphics on screen frame by frame. It also uses some other open-source libraries like Freetype for rendering the fonts on screen, LwIP for instantiating a light weight Networking stack which enables you to connect some FPGA hardware with the application. Mostly radars are implemented on FPGA as they requires highly parallel computations for calculating simple Fourier transforms or even complex algorithms for each segment for each sweep. I have tested this application on Xilinx Zedboard but with some changes to code it can be configured to work on other FPGA boards.

This application extensively relies on imGUI which is the reason for such snappy user interface without any hiccups. imGUI is the heart of this application. It is am amazing piece of code which is now even used by big companies like Google for some of their famous applications like YouTube on iOS.

The gif shown above was taken while the project was still in its early stages and it shows a sample GUI integrating different tabs, Screen Overlays and control options. The project is not final yet. However, it can offer some functionality out-of the box. I have some other projects at hand due to which this project doesn't get much attention. I intend to complete this project by the end of 2020. 

Wondring where to start in designing a GUI? Don't worry and checkout my technical blog post for beginners where I explained what are the fundamental building blocks of a GUI: [https://simarmannsingh.de/designguiopengl.html](https://simarmannsingh.de/designguiopengl.html). You can also check out my other blog posts on my personal website [https://simarmannsingh.de](https://simarmannsingh.de). 

## Copyright and License

Copyright 2020 Simar Mann Singh Chawala. Code released under the [GNU General Public License v3.0](https://github.com/simarmannsingh/OpenSource-Radar-GUI/blob/master/LICENSE) license.
