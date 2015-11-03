#!/bin/bash

#------------------------------------------------------------------------------
# Read the configuration file for the SDK Build
shopt -s extglob
configfile="SDK_Configuration.conf" # set the actual path name of your (DOS or Unix) config file
tr -d '\r' < $configfile > $configfile.unix
while IFS='= ' read lhs rhs
do
    if [[ ! $lhs =~ ^\ *# && -n $lhs ]]; then
        rhs="${rhs%%\#*}"    # Del in line right comments
        rhs="${rhs%%*( )}"   # Del trailing spaces
        rhs="${rhs%\"*}"     # Del opening string quotes 
        rhs="${rhs#\"*}"     # Del closing string quotes 
        declare $lhs="$rhs"
    fi
done < $configfile.unix
rm $configfile.unix
#------------------------------------------------------------------------------

cd $SDK_INSTALL


if [ ! -e "$SDK_INSTALL/$QT_INSTALL_DIR" ];
then
  echo "#------------------------------"
  echo " You should have installed Qt $QT_VERSION in the ${SDK_INSTALL}"
  echo " The $QT_INSTALL_DIR directory is not located in the ${SDK_INSTALL}"
  echo " Please install $QT_INSTALL_DIR first before running this script."
  echo " Your SDK is incomplete and you will not be able to build DREAM3D"
  echo "--------------------------------"
  exit

fi

export PATH=$PATH:$SDK_INSTALL/$QT_INSTALL_DIR/$QT_BIN_DIR

# Build the qwt libraries we need and set our Environment Variable.
QWT_FOLDER_NAME="qwt-${QWT_VERSION}_src"

if [ ! -e "$SDK_INSTALL/${QWT_FOLDER_NAME}.tar.gz" ];
then
  echo "-------------------------------------------"
  echo " You should have a Qwt ${QWT_VERSION} archive from DREAM3D."
  echo "-------------------------------------------"
  exit
fi

if [ ! -e "$SDK_INSTALL/${QWT_FOLDER_NAME}" ];
then
  tar -xvzf ${QWT_FOLDER_NAME}.tar.gz
fi

# We assume we already have downloaded the source for qwt Version 6.1.2 and have it in a folder
# called qwt-6.1.2
cd ${QWT_FOLDER_NAME}
$SDK_INSTALL/$QT_INSTALL_DIR/$QT_BIN_DIR/qmake qwt.pro
make -j${PARALLEL_BUILD}
make install

echo "" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "#--------------------------------------------------------------------------------------------------" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "# Qwt ${QWT_VERSION} Library" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "set(QWT_INSTALL \"\${DREAM3D_SDK_ROOT}/${QWT_INSTALL}\" CACHE PATH \"\")" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
