#!/bin/bash

RED='\033[0;31m';
GREEN='\033[0;32m';
BLUE='\033[0;34m';
YELLOW='\033[1;33m';
ORANGE='\033[0;33m';
PURPLE='\033[1;35m';
NOCOLOR='\033[0m';

echo -e "${PURPLE}
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA   VVV              VVV   EEEEEEEEE   RRRRRRRRRR
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA    VVV            VVV    EEEEEEEEE   RRR    RRR
  HHH     HHH   EEE         LLL         LLL         III   SSS         AAA      AAA     VVV          VVV     EEE         RRR    RRR
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA      VVV        VVV      EEEEEEEEE   RRRRRRRRRR
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAA      AAA       VVV      VVV       EEEEEEEEE   RRRRRR
  HHH     HHH   EEE         LLL         LLL         III         SSS   AAA      AAA        VVV    VVV        EEE         RRR RRR
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III   SSSSSSSSS   AAA      AAA         VVV  VVV         EEEEEEEEE   RRR  RRR
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III   SSSSSSSSS   AAA      AAA          VVVVVV          EEEEEEEEE   RRR   RRR
${NOCOLOR}";


while [[ true ]]; do
	printf  "${BLUE}What do you want to do?${NOCOLOR} [${GREEN}i${NOCOLOR}, ${RED}u${NOCOLOR}, ${ORANGE}a${NOCOLOR}][${GREEN}install${NOCOLOR}, ${RED}uninstall${NOCOLOR}, ${ORANGE}abort${NOCOLOR}]: "
	read INPUT;
	printf "\n";
	if [[ ${INPUT,,} = i || ${INPUT,,} = install ]]; then
		printf "${GREEN}Installing HelliSaver...${NOCOLOR}\n";
		meson setup build;
		ninja -C build;
		sudo ninja -C build install;
		printf "${BLUE}Install done.${NOCOLOR}\n";
		exit;
	elif [[ ${INPUT,,} = u || ${INPUT,,} = uninstall ]]; then
		printf "${RED}Uninstalling HelliSaver...${NOCOLOR}\n";
		sudo ninja -C build uninstall;
		rm -R build;
		printf "${BLUE}Uninstall done.${NOCOLOR}\n";
		exit;
	elif [[ ${INPUT,,} = a || ${INPUT,,} = abort ]]; then
		printf "${ORANGE}Aborting...${NOCOLOR}\n";
		printf "${BLUE}Abort done.${NOCOLOR}\n";
		exit;
	else
		printf "${YELLOW}Invalid input!${NOCOLOR}\n";
	fi
done