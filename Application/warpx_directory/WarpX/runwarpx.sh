source ~/.bashrc
for i in {0..2}
do
    echo "0.01" > eb.txt
    source ./base.sh
    echo "nocomp"
    mpirun -np 8 ./nocomp inputs > otfile/nocomp-$i.txt
    mv diags/plt001001.h5 diags/nocomp.h5

    echo "comp"
    mpirun -np 8 ./comp inputs > otfile/comp-$i.txt
    mv diags/plt001001.h5 diags/comp-005.h5
done
rm -rf diags/*old*
