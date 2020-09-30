## usage: 
# 1) build Syntacts using CMake and Ninja
# 2) run this script, e.g.:
#    python package_release.py 1.3.0 C:/Users/epeze/Desktop
#    python package_release.py 1.3.0 ~/Desktop

import sys
import os
import subprocess
from shutil import copy2, copytree, ignore_patterns, rmtree, make_archive
from sys import platform

if platform == 'win32':
    unity_exe = "C:/Program Files/Unity/Hub/Editor/2019.4.11f1/Editor/Unity.exe"
    version    = sys.argv[1]
    output_dir = sys.argv[2] + "/" + "syntacts_v" + version + "_windows"
    # delete existing
    if os.path.exists(output_dir) and os.path.isdir(output_dir):
        rmtree(output_dir)
    # make release folder
    os.mkdir(output_dir)
    # package license/readme
    copy2("../LICENSE", output_dir)
    copy2("../README.md", output_dir)
    # package GUI
    copy2("../build/gui/syntacts_gui.exe",output_dir)
    # package c
    os.mkdir(output_dir + "/c")
    copy2("../c/syntacts.h",output_dir + "/c")
    copy2("../build/c/syntacts_c.dll",output_dir + "/c")
    # package csharp
    copytree("../csharp",output_dir+"/csharp",ignore=ignore_patterns(".gitignore","*-d.dll","bin","obj"))
    # package python
    copytree("../python",output_dir+"/python",ignore=ignore_patterns(".gitignore","*-d.dll","__pycache__"))
    # make unitypackage
    os.mkdir(output_dir + "/unity")
    unity_cmd = ' -quit -batchmode -nographics -projectPath "../unity/SyntactsDemo" -exportPackage "Assets/Syntacts" "' + output_dir + '/unity/syntacts.unitypackage"'
    p = subprocess.Popen(unity_exe+unity_cmd)
    p.wait()
    # package unity demo
    if os.path.exists("../unity/SyntactsDemo/Assets/Syntacts/Plygins/syntacts_c-d.dll"):
        os.remove("../unity/SyntactsDemo/Assets/Syntacts/Plugins/syntacts_c-d.dll")
    copytree("../unity/SyntactsDemo/Assets",output_dir+"/unity/SyntactsDemo/Assets",ignore=ignore_patterns(".gitignore"))
    copytree("../unity/SyntactsDemo/Packages",output_dir+"/unity/SyntactsDemo/Packages",ignore=ignore_patterns(".gitignore"))
    copytree("../unity/SyntactsDemo/ProjectSettings",output_dir+"/unity/SyntactsDemo/ProjectSettings",ignore=ignore_patterns(".gitignore"))
    # create zip
    make_archive(output_dir, 'zip', output_dir)
elif platform == 'darwin':
    unity_exe = "/Applications/Unity/Hub/Editor/2019.4.11f1/Unity.app/Contents/MacOS/Unity"
    version    = sys.argv[1]
    output_dir = sys.argv[2] + "/" + "syntacts_v" + version + "_mac"
    # delete existing
    if os.path.exists(output_dir) and os.path.isdir(output_dir):
        rmtree(output_dir)
    # make release folder
    os.mkdir(output_dir)
    # package license
    copy2("../LICENSE", output_dir)
    copy2("../README.md", output_dir)
    # package GUI
    copy2("../build/gui/syntacts_gui",output_dir)
    # package c
    os.mkdir(output_dir + "/c")
    copy2("../c/syntacts.h",output_dir + "/c")
    copy2("../build/c/libsyntacts_c.dylib",output_dir + "/c")
    # package csharp
    copytree("../csharp",output_dir+"/csharp",ignore=ignore_patterns(".gitignore","*-d.dll","bin","obj"))
    # package python
    copytree("../python",output_dir+"/python",ignore=ignore_patterns(".gitignore","*-d.dll","__pycache__"))
    # make unitypackage
    os.mkdir(output_dir + "/unity")
    unity_cmd = ' -quit -batchmode -nographics -projectPath "../unity/SyntactsDemo" -exportPackage "Assets/Syntacts" "' + output_dir + '/unity/syntacts.unitypackage"'
    os.system(unity_exe + unity_cmd)
    # package unity demo
    copytree("../unity/SyntactsDemo/Assets",output_dir+"/unity/SyntactsDemo/Assets",ignore=ignore_patterns(".gitignore"))
    copytree("../unity/SyntactsDemo/Packages",output_dir+"/unity/SyntactsDemo/Packages",ignore=ignore_patterns(".gitignore"))
    copytree("../unity/SyntactsDemo/ProjectSettings",output_dir+"/unity/SyntactsDemo/ProjectSettings",ignore=ignore_patterns(".gitignore"))
    # create zip
    make_archive(output_dir, 'zip', output_dir)