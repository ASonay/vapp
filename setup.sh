#!bin/sh

if [ "${BASH_SOURCE[0]}" != "" ]; then
    # This should work in bash.
    _src=${BASH_SOURCE[0]}
elif [ "${ZSH_NAME}" != "" ]; then
    # And this in zsh.
    _src=${(%):-%x}
elif [ "${1}" != "" ]; then
    # If none of the above works, we take it from the command line.
    _src="${1/setup.sh/}/setup.sh"
else
    echo -e "\033[1;31mERROR:\033[0m Could not determine the base directory of VAPP, i.e. where \"setup.sh\" is located."
    return 1
fi

location="$(cd -P "$(dirname "${_src}")" && pwd)"
unset _src

echo -e "\n******************************************************"
echo -e "*3D Visual manipulation tool                         *"
echo -e "******************************************************\n"

export VAPP=$location
echo -e "\nVAPP Home Dir : " ${VAPP}

alias vapp-make='cd ${VAPP}/build/; make -j4; cd ../'
alias vapp-clean='rm -rf ${VAPP}/build/'
alias vapp-build='mkdir ${VAPP}/build && cd ${VAPP}/build; cmake ../; make -j4; cd ../'

if [ -d "${VAPP}/build" ] 
then
    echo -e "Your build/bin directory already exist."
    echo -e "\nTo recompile please use \e[1m\e[4mvapp-make\e[0m.\n"
else
    echo -e "\nTo compile please use \e[1m\e[4mvapp-build\e[0m.\n"
fi

export CPATH=${CPATH}${CPATH:+:}${VAPP}
export PATH=${PATH}${PATH:+:}${VAPP}/build