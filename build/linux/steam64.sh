s sets up your Linux environment to launch a 64-bit x86_64 game as if it
#  were launched from the Steam Client. It makes sure the game favors the
#  Steam Runtime instead of local libraries, and that the Steam Overlay is
#  loaded in.
#
# You will still need a proper steam_appid.txt in the right place, and
#  the Steam Client running in the background somewhere, to get
#  Steamworks to function, but this will allow you to run from the command
#  line instead of launching from Steam directly.
#
# Running a 32-bit x86 app? Use steamapp32 instead.
#
# Usage looks like:  steamapp64 ./mygame.bin
#  or maybe:  steamapp64 gdb ./mygame.bin

#set -x

DEBUG=0
if [ "$1" == "--debug" ]; then
    DEBUG=1
    shift
fi

if [ -z "$STEAM_RUNTIME" ]; then
    #if [ "$DEBUG" == "1" ]; then
    #    STEAM_RUNTIME="$HOME/steam-runtime/runtime-debug"
    #else
        STEAM_RUNTIME="$HOME/.local/share/Steam/ubuntu12_32/steam-runtime"
    #fi
fi

export LD_PRELOAD=~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so
exec "$STEAM_RUNTIME/run.sh" "$@"
exit 1
