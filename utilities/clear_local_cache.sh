#!/bin/sh

# Check if the .cle folder exists in the home directory
if [ -d "${HOME}/.cle" ]; then
    # Prompt the user for confirmation before removing the .cle folder from the home directory
    printf "Are you sure you want to remove the ${HOME}/.cle folder from your home directory? (y/n) "
    read confirm

    # Convert the input to lower case
    confirm=$(echo $confirm | tr '[:upper:]' '[:lower:]')

    # Check if the input is 'y' or 'yes'
    if [ "$confirm" = "y" ] || [ "$confirm" = "yes" ]; then
        # Remove the .cle folder from the home directory without prompting for confirmation
        rm -rf "${HOME}/.cle"
        echo "The ${HOME}/.cle folder has been removed from your home directory."
    else
        echo "The ${HOME}/.cle folder was not removed from your home directory."
    fi
else
    echo "The ${HOME}/.cle folder does not exist in your home directory."
fi


