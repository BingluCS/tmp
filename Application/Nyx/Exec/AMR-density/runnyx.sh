source ~/.bashrc
for i in {0..2}
do
    echo "0.005" > eb.txt
    echo "nocomp"
    mpirun -np 8 ./nocomp inputs > otfile/nocomp-$i.txt
    mv run/plt00211_multi.h5  run/nocomp.h5

    echo "comp"
    mpirun -np 8 ./comp inputs > otfile/comp-$i.txt
    mv run/plt00211_multi.h5  run/comp-005.h5
done
rm -rf run/*old*
