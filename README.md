# YYC Commonplace

During the development of a few projects, I gradually understand how Windows make the compromise with the code written by its old developers, and what is developer wanted in contemporary C++ standard library under Windows environment. So I create this static library for all of my C++ project, After this, I do not need to write these duplicated code in each project. I can use a clear and easy way to manage these codes. I can easily fix issues found in project using this library by updating a single project, rather than fixing these duplicated code in each project one by one because all of them share the same implementations.

This project mainly is served for my personal use. But I would be honored if you would like to use this in your project. Almost of my projects, except some critical projects (they will copy this project implementations into their own project scope to eliminate non-common library dependency), will gradually adapt to this project and drop their own individual implementations.

This project includes Visual Studio project file and CMake support at the same time. So that at least I can use one of them freely.

**WIP. Do not use it now.**
