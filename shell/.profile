# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi

# set PATH so it includes user's private bin if it exists
if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi
# Added by xgh
JAVA_HOME="/home/xgh/local/jdk1.7.0_17"
PATH="$JAVA_HOME/bin:$PATH"
CLASS_PATH=".:$JAVA_HOME/lib"
export JAVA_HOME PATH CLASS_PATH
#if [ -f "$HOME/local/bin/report.sh" ]; then
    #. "$HOME/local/bin/report.sh"
#fi
# Added by xgh ends here
export LANGUAGE="zh_CN:en"
export LC_MESSAGES="zh_CN.UTF-8"
export LC_CTYPE="zh_CN.UTF-8"
export LC_COLLATE="zh_CN.UTF-8"
