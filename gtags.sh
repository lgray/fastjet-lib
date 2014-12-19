#!/bin/bash
# 
# small script to generate the "gtags" files for easy navigation
# around the project (while leaving out directories like experimental,
# scripts, that contain non-principal copies of code)
#
# Usage: ./gtags.sh
#
find  ./include ./src ./tools ./plugins ./example -print | gtags --file=-
