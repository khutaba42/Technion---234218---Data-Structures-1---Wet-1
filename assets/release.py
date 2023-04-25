#!/usr/bin/env python

import os
import shutil
import zipfile

PROJ_DIR = os.getcwd()
SRC_DIR = os.path.join(PROJ_DIR, "src")
LIB_DIR = os.path.join(PROJ_DIR, "lib")
ZIP_DIR = os.path.join(PROJ_DIR, "zipped")
ZIP_FILE = "wet_1.zip"

if not os.path.isdir(ZIP_DIR):
    os.mkdir(ZIP_DIR)

# Copy all files from the src directory to the zip directory
for filename in os.listdir(SRC_DIR):
    if not filename.startswith("."):
        src_path = os.path.join(SRC_DIR, filename)
        dst_path = os.path.join(ZIP_DIR, filename)
        if os.path.isfile(src_path):
            shutil.copy2(src_path, dst_path)

# Copy all files from the lib directory to the zip directory
for filename in os.listdir(LIB_DIR):
    if not filename.startswith("."):
        src_path = os.path.join(LIB_DIR, filename)
        dst_path = os.path.join(ZIP_DIR, filename)
        if os.path.isfile(src_path):
            shutil.copy2(src_path, dst_path)

# Create the zip file
zip_file_path = os.path.join(ZIP_DIR, ZIP_FILE)
with zipfile.ZipFile(zip_file_path, "w") as zip_file:
    for filename in os.listdir(ZIP_DIR):
        if not filename.startswith(".") and filename != ZIP_FILE:
            file_path = os.path.join(ZIP_DIR, filename)
            zip_file.write(file_path, filename)

# Delete all files in the zip directory except the zip file
for filename in os.listdir(ZIP_DIR):
    if not filename.startswith(".") and filename != ZIP_FILE:
        file_path = os.path.join(ZIP_DIR, filename)
        if os.path.isfile(file_path):
            os.remove(file_path)

print("Files zipped to: ", zip_file_path)
