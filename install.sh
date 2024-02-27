#!bin/sh

source "./setup.sh"

if [ -d "${VAPP}/build" ] 
then
    echo -e "Your build/bin directory already exist."
    echo -e "\nTo recompile please use \e[1m\e[4mvapp-make\e[0m.\n"
else
    vapp-build
fi
