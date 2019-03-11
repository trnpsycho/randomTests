#!/bin/bash
#
#################################################################
#   Connect to Windows script: FMC Technologies               	#
#																#
#                                                     			#
#   Revisions:                                        			#
#   ---------------------------------------                		#
#   1: Kevin Farley	10/31/2018 Original idea, first pass 		#
#																#
#################################################################


# Setup default connection options here.

# The location on the FCU you want to mount the windows folder
mountPoint="/mnt/win"

# The machine IP address that you want to mount to
mountServer="172.16.40.189" 

# The name of the windows share you wish to connect to
mountShare="UCOSShare"

# The username and password you'd like to connect with
mountUsername="UCOS"
mountPassword="qwerty"

#################################################################
# End of configuration settings. Do not edit below this line. ###
#################################################################

# This will print at the top of the screen every time
function printHeader
{
	clear
	printf "\n"
	printf "*********************************************************\n"
	printf "*							*\n"
	printf "*     FMC Technologies: Connect to Windows script	*\n"
	printf "*							*\n"
	printf "*********************************************************\n\n"
	
	printf "Current settings:\n\n"
	printVariables
	printf "\n\n"
}

# List out the current settings
function printVariables
{
	printf "    Current Mount Point = $mountPoint\n"	
	printf "    Current Server = $mountServer\n"
	printf "    Current Share = $mountShare\n"
	printf "    Current Username = $mountUsername\n"
	
}

# Main menu and options selector
function main
{	
	printHeader
	
	if mount|grep $mountServer >> /dev/null;
	then
		mountExists="(Already mounted)"
	else
		mountExists=""
	fi
	
	printf "Select what you'd like to do:\n\n"
		printf "(1) Mount Windows $mountExists\n"
		printf "(2) Remove Mount\n"
		printf "(3) Change mounting details\n\n"
		
		printf "(q) Quit\n\n"
		
		job="q"
		
		printf "Enter (1/2/3 or q) then hit [Enter]: "
			read job
		
		if [ "$job" = "1" ]; then
			clear
			mountUp
		fi
		if [ "$job" = "2" ]; then
			clear
			removeMount
		fi
		if [ "$job" = "3" ]; then
			clear
			changeDetails
		fi

}

# Used if changing from the credentials listed above
function changeDetails
{
	printHeader
	
	printf "Would you like to enter a new Server IP Address (y or n): "
		read answer

		if [ "$answer" = "y" ] 
		then
			printf "Enter IP address: "
			read temp
			mountServer="$temp"
			#printf "$mountServer"
			printHeader
		fi
		
	printf "Would you like to enter a new Server Share (y or n): "
		read answer

		if [ "$answer" = "y" ] 
		then
			printf "Enter Share name: "
			read temp
			mountShare="$temp"
			#printf "$mountShare"
			printHeader
		fi
		
	printf "Would you like to enter a new Username (y or n): "
		read answer

		if [ "$answer" = "y" ] 
		then
			printf "Enter Username: "
			read temp
			mountUsername="$temp"
			printf "Enter password for this user:"
			read temp
			mountPassword="$temp"
			printHeader
			
		fi
	
	removeMount
	main
		
}

# Mount the drive
function mountUp
{
	if [ -d $mountPoint ]	
	then
		printf "Directory exists...\n"
	else
		printf "Directory doesn't exist. Creating...\n"
		mkdir $mountPoint
		printf "Mounting windows file system...\n"
		mount -t cifs -o username=$mountUsername,password=$mountPassword //$mountServer/$mountShare $mountPoint
		printf "Done.\n\n"
		cd $mountPoint
	fi
	main
}

# Remove the mounted drive
function removeMount
{
	cd /
	umount $mountPoint
	rmdir $mountPoint
	main
}

# Call the starting routine
main
