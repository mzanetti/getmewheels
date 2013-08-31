#!/bin/bash

CODE_DIR=getmewheels
BUILD_DIR=builddir
USER=phablet
USER_ID=32011
PASSWORD=phablet
TARGET_IP=${TARGET_IP-127.0.0.1}
TARGET_SSH_PORT=${TARGET_SSH_PORT-2222}
NUM_JOBS='$(( `grep -c ^processor /proc/cpuinfo` + 1 ))'
SUDO="echo $PASSWORD | sudo -S"

exec_with_ssh() {
    ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -t $USER@$TARGET_IP -p $TARGET_SSH_PORT sudo -u $USER -i bash -ic \"$@\"
}

exec_with_adb() {
    adb shell $@
}

adb_root() {
    adb root
    adb wait-for-device
}

install_ssh_key() {
    ssh-keygen -R $TARGET_IP
    HOME_DIR="${CHROOT_PREFIX}/home/phablet"
    adb push ~/.ssh/id_rsa.pub $HOME_DIR/.ssh/authorized_keys
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh/authorized_keys
    adb shell chmod 700 $HOME_DIR/.ssh
    adb shell chmod 600 $HOME_DIR/.ssh/authorized_keys
}

setup_adb_forwarding() {
    adb forward tcp:$TARGET_SSH_PORT tcp:22
    adb forward tcp:$TARGET_DEBUG_PORT tcp:$TARGET_DEBUG_PORT
}

sync_code() {
    [ -e .bzr ] && bzr export --uncommitted --format=dir /tmp/$CODE_DIR
    [ -e .git ] && git checkout-index -a -f --prefix=/tmp/$CODE_DIR/
    rsync -crlOzv --delete --exclude builddir -e "ssh -p $TARGET_SSH_PORT -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no" /tmp/$CODE_DIR/ $USER@$TARGET_IP:$CODE_DIR/
    rm -rf /tmp/$CODE_DIR
}

build() {
    exec_with_ssh "[ $CODE_DIR/debian/control -nt getmewheels2-build-deps*deb ] && $SUDO mk-build-deps --tool=\\\"apt-get -y --no-install-recommends\\\" --build-dep --install $CODE_DIR/debian/control"
    exec_with_ssh "mkdir -p $CODE_DIR/$BUILD_DIR"
    exec_with_ssh PATH=/usr/lib/ccache:$PATH "cd $CODE_DIR/$BUILD_DIR && PATH=/usr/lib/ccache:$PATH qmake ../getmewheels2.pro"
    exec_with_ssh PATH=/usr/lib/ccache:$PATH "cd $CODE_DIR/$BUILD_DIR && PATH=/usr/lib/ccache:$PATH make -j$NUM_JOBS"
}

run() {
    echo "runningcd cd $CODE_DIR/$BUILD_DIR/src && ./getmewheels2 --desktop_file_hint=/home/$USER/$CODE_DIR/src/getmewheels2_ubuntu.desktop"
    exec_with_ssh "cd $CODE_DIR/$BUILD_DIR/src && ./getmewheels2 --desktop_file_hint=/home/$USER/$CODE_DIR/src/getmewheels2_ubuntu.desktop"
}

adb_root
[ "${TARGET_IP}" = "127.0.0.1" ] && setup_adb_forwarding


echo "Transferring code.."
sync_code
echo "Building.."
build
echo "Running.."
run

