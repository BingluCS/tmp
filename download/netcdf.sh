cd $CBB_HOME/download
wget -c -4 https://github.com/Unidata/netcdf-c/archive/refs/tags/v4.9.0.tar.gz
tar -xzvf v4.9.0.tar.gz
cd netcdf-c-4.9.0/
export CPPFLAGS="-I$DIR/hdf5/include -I$DIR/grib2/include"
export LDFLAGS="-L$DIR/hdf5/lib -L$DIR/grib2/lib"
export CC=mpicc
./configure --prefix=$DIR/netcdf --disable-dap
make -j 8
make install
cd ..

wget -c -4 https://github.com/Unidata/netcdf-fortran/archive/refs/tags/v4.6.0.tar.gz
tar -xvzf v4.6.0.tar.gz
cd netcdf-fortran-4.6.0/
export LD_LIBRARY_PATH="$DIR/netcdf/lib:$LD_LIBRARY_PATH"
export CPPFLAGS="-I$DIR/netcdf/include"
export LDFLAGS="-L$DIR/netcdf/lib"
./configure --prefix=$DIR/netcdf --disable-shared
make -j 8
make install
cd $CBB_HOME
