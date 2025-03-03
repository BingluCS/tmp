cd warpx_directory
git clone https://github.com/ECP-WarpX/picsar.git
git clone https://github.com/ECP-WarpX/warpx-data.git

cd WarpX
make clean
make -j 8 USE_HDF5_SZ3=FALSE 
mv main3d* nocomp

make clean
make -j 8 USE_HDF5_SZ3=TRUE SZ3_HOME=$SZ3_HOME
mv main3d* comp

cd $CBB_HOME/Application
