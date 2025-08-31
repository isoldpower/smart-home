#!/bin/bash
# Create and configure a systemd service for the web protocol daemon service on Linux in interactive mode

# Context variables
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
EXECUTION_USER=$(whoami)

# Ensure template file exists
TEMPLATE_NAME="template.service"
TEMPLATE_FILE="$SCRIPT_DIR/$TEMPLATE_NAME"

if [ ! -f "$TEMPLATE_FILE" ]; then
  echo "Template file '$TEMPLATE_FILE' not found!"
  exit 1
else
  echo "Found template file '$TEMPLATE_FILE' found."
fi
TEMPLATE_CONTENTS=$(cat "$TEMPLATE_FILE")

# Introduction & Welcome message
echo "Template contents loaded."
echo ""
echo "Welcome to the Web Protocol Daemon Service systemd configuration script."
echo "This script will help you create and configure a systemd service for the Web Protocol Daemon Service."
echo "Feel free to press Enter to accept the default values in brackets."
echo ""

# Default values
DEFAULT_SERVICE_NAME="web-protocol-daemon"
DEFAULT_SERVICE_DESCRIPTION="Web Protocol Daemon Service"
DEFAULT_SERVICE_USER="$EXECUTION_USER"
DEFAULT_EXECUTABLE_PATH="$SCRIPT_DIR/web_protocol_daemon"
DEFAULT_ENV_VARS=""

# User prompts for service settings
read -r -p "Service name (default: $DEFAULT_SERVICE_NAME): " SERVICE_NAME
SERVICE_NAME=${SERVICE_NAME:-$DEFAULT_SERVICE_NAME}

read -r -p "Service description (default: $DEFAULT_SERVICE_DESCRIPTION): " SERVICE_DESCRIPTION
SERVICE_DESCRIPTION=${SERVICE_DESCRIPTION:-$DEFAULT_SERVICE_DESCRIPTION}

read -r -p "Service administrator (default: $DEFAULT_SERVICE_USER): " SERVICE_USER
SERVICE_USER=${SERVICE_USER:-$DEFAULT_SERVICE_USER}

read -r -p "Daemon executable path (default: $DEFAULT_EXECUTABLE_PATH): " EXECUTABLE_PATH
EXECUTABLE_PATH=${EXECUTABLE_PATH:-$DEFAULT_EXECUTABLE_PATH}

read -r -p "Environmental variables (default: $DEFAULT_ENV_VARS): " ENV_VARS
ENV_VARS=${ENV_VARS:-$DEFAULT_ENV_VARS}

DEFAULT_FILE_DIR="/etc/systemd/system/"
read -r -p "Systemd service file path (default: $DEFAULT_FILE_DIR): " SERVICE_FILE_DIR
SERVICE_FILE_DIR=${SERVICE_FILE_DIR:-$DEFAULT_FILE_DIR}
SERVICE_FILE="$SERVICE_FILE_DIR$SERVICE_NAME.service"

# Confirmation of settings
echo ""
echo "Please confirm the following settings:"
echo "Service Name: $SERVICE_NAME"
echo "Service Description: $SERVICE_DESCRIPTION"
echo "Service Administrator: $SERVICE_USER"
echo "Executable Path: $EXECUTABLE_PATH"
echo "Environmental Variables: $ENV_VARS"
echo "Service File Path: $SERVICE_FILE"
echo ""
read -r -p "Are these settings correct? (y/n): " CONFIRMATION
if [[ "$CONFIRMATION" != "y" && "$CONFIRMATION" != "Y" && "$CONFIRMATION" != "yes" && "$CONFIRMATION" != "Yes" ]]; then
  echo "Configuration aborted by user."
  exit 1
fi

# Create the systemd service file
echo "Creating systemd service file at '$SERVICE_FILE'..."
SERVICE_CONTENTS=$(echo "$TEMPLATE_CONTENTS" | sed \
    -e "s#{{SERVICE_DESCRIPTION}}#$SERVICE_DESCRIPTION#g" \
    -e "s#{{EXEC_START}}#$EXECUTABLE_PATH#g" \
    -e "s#{{SERVICE_USER}}#$SERVICE_USER#g" \
    -e "s#{{ENVIRONMENT_VARIABLES}}#$ENV_VARS#g"
)

echo "$SERVICE_CONTENTS" > "$SERVICE_FILE"

echo ""
echo ".service file created successfully at '$SERVICE_FILE'."
echo "To install run 'systemctl daemon-reload'; then run 'systemctl enable $SERVICE_FILE'"
exit 0