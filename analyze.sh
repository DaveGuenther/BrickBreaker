echo Showing profiles...
cd build/linux/prof
feh game.gif --geometry 960x990 &
kcachegrind game.callgrind
firefox flamegraph_out.svg
