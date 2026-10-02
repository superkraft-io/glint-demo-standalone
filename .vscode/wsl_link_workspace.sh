#!/usr/bin/env bash
# Run from the workspace root inside WSL (the Linux build/launch tasks do this):
#   wsl -d Ubuntu-24.04 -- bash .vscode/wsl_link_workspace.sh
#
# gdb runs inside WSL, but VS Code's ${workspaceFolder} is a Windows path
# (C:/...) that gdb cannot open, and launch.json has no variable for the WSL
# path.  Link a fixed Linux path to this workspace so launch.json can use
#   /tmp/vscode-wsl-${workspaceFolderBasename}/...
set -euo pipefail
ln -sfn "$PWD" "/tmp/vscode-wsl-$(basename "$PWD")"
echo "Linked /tmp/vscode-wsl-$(basename "$PWD") -> $PWD"
