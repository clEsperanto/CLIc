#!/bin/sh

# Check if the .cle folder exists in the home directory
if [ -d "${HOME}/.cache/clesperanto" ]; then
    # Prompt the user for confirmation before removing the .cle folder from the home directory
    printf "Are you sure you want to remove the clesperanto folder from your cache directory? (y/n) "
    read confirm

    # Convert the input to lower case
    confirm=$(echo $confirm | tr '[:upper:]' '[:lower:]')

    # Check if the input is 'y' or 'yes'
    if [ "$confirm" = "y" ] || [ "$confirm" = "yes" ]; then
        # Remove the .cle folder from the cache directory without prompting for confirmation
        rm -rf "${HOME}/.cache/clesperanto"
        echo "The clesperanto folder has been removed from your cache directory."
    else
        echo "The clesperanto folder was not removed from your cache directory."
    fi
else
    echo "The clesperanto folder does not exist in your cache directory."
fi


