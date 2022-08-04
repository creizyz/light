# Project LIGHT

## Overview

This is a toy project aimed at improving my coding skills while learning computer graphics techniques.

*Note : this project was kickstarted using the [cxx-project-template](https://github.com/creizyz/cxx-project-template) project.*

## Setup & Requirements

This section explains how to setup this project in your environment.

### Development Environment

This CXX project uses ***cmake*** as a build orchestrator and requires a ***C++20 compatible compiler***. At this stage, the following compilers were tested:

[X] gcc-11.2 (msys2)
[ ] gcc-11.2 (linux)
[ ] MSVC 17+
[ ] clang
[ ] intel compiler

### Dependencies

This project has no dependencies yet. Since the aim of this project is solely to improve my computer science and computer graphics skills, the dependencies may be kept to the minimum.

### Cmake Presets

An easy way to manage multiple cmake configurations for a CXX project is to use [cmake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html). This project declares a default cmake configuration in ```./CMakePresets.json``` that you can expand in ```./CMakeUserPresets.json```. By default, two configurations are present in the project (Debug and Release). Feel free to modify / add any configuration in the ***CMakeUserPresets.json*** file, as it is not synced through git.

Aside from managing cmake configurations, you can also manage your dependencies through the ***CMakeUserPresets.json*** file. The base file comes with a ***dependencies*** configuration that lets you declare paths and append them to ***CMAKE_PREFIX_PATH***. When setting up the project, make sure to update those paths.

## Project development

This section explains how to configure, build, test and package this project.

### How to configure the project

To configure the project using one of the available presets, go to the root directory of the project and run:

```> cmake --preset <preset>```

For a list of available presets for this project, please run:

```> cmake --list-presets```

### How to build the project

To build the project, configure the project and go to the ```./build/<preset>``` folder to run:

```> cmake --build .```

To build this project using multiple cores, run instead:

```> cmake -j<nb_cores> --build .```

### How to test the project

**[ TODO ]** Declare your test process here

### How to package the project

To package the project, configure the project and go to the ```./build/<preset>``` folder to run:

```> cmake --install .```

To select the folder in which to install this project, run instead:

```> cmake -DCMAKE_INSTALL_PREFIX=<install_path> --install .```

## Hints & Tips

If this section is empty, you're on your own. Your predecessor decided to move quickly... and leave you behind. Enjoy !

