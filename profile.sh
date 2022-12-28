cd build/linux
export CPUPROFILE=game.prof
export PATH=/usr/local/src/FlameGraph:$PATH
#export HEAPPROFILE=HEAP_
echo Starting game with profiler...
./game
echo Building call graph...
pprof --gif game game.prof > game.gif
echo Building flamegraph...
pprof --collapsed ./game game.prof >> flamegraph.collapse
/usr/local/src/FlameGraph/flamegraph.pl flamegraph.collapse > flamegraph_out.svg
echo Building cachegrind
pprof --callgrind game game.prof > game.callgrind
echo Cleaning up...
cp ./game.prof ./prof
cp ./flamegraph.collapse ./prof
cp ./game.gif ./prof
cp ./game.callgrind ./prof
cp ./flamegraph_out.svg ./prof
cp ./game ./prof
rm ./game.prof
rm ./flamegraph_out.svg
rm ./game.gif
rm ./game.callgrind
rm ./flamegraph.collapse
cd ..
cd ..
bash analyze.sh
