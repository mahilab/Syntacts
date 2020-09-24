## usage: 
# 1) build Syntacts using CMake and Ninja
# 2) run this script, e.g.:
#    python package_release.py 1.2.0 C:/Users/epeze/Desktop

import sys
import os
from shutil import copy2, copytree, ignore_patterns

version    = sys.argv[1]
output_dir = sys.argv[2] + "/" + "syntacts_v" + version + "_windows"

# make release folders
os.mkdir(output_dir)
# copy license
copy2("../LICENSE", output_dir)
# copy GUI
copy2("../build/gui/syntacts-gui.exe",output_dir)
# copy csharp
copytree("../csharp",output_dir+"/csharp",ignore=ignore_patterns(".gitignore","*-d.dll","bin","obj"))
# copy python
copytree("../python",output_dir+"/python",ignore=ignore_patterns(".gitignore","*-d.dll","__pycache__"))
# copy unity
os.mkdir(output_dir + "/unity")
copytree("../unity/SyntactsDemo/Assets",output_dir+"/unity/SyntactsDemo/Assets",ignore=ignore_patterns(".gitignore"))
copytree("../unity/SyntactsDemo/Packages",output_dir+"/unity/SyntactsDemo/Packages",ignore=ignore_patterns(".gitignore"))
copytree("../unity/SyntactsDemo/ProjectSettings",output_dir+"/unity/SyntactsDemo/ProjectSettings",ignore=ignore_patterns(".gitignore"))
## TODO: unitypackage: https://stackoverflow.com/questions/62537471/c-sharp-application-to-create-unity-package