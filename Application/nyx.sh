cd $CBB_HOME/Application

cd Nyx/subprojects
. build-sun.sh

cd $CBB_HOME/Application/Nyx/Exec/AMR-density

make clean
make -j 8 USE_HDF5_SZ3=FALSE 
mv Nyx3d* nocomp

make clean
make -j 8 USE_HDF5_SZ3=TRUE SZ3_HOME=$SZ3_HOME
mv Nyx3d* comp

cd $CBB_HOME/Application
