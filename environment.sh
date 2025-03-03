export DIR=$CBB/Libs
export JASPERLIB=$DIR/grib2/lib
export JASPERINC=$DIR/grib2/include
export LDFLAGS=-L$DIR/grib2/lib
export CPPFLAGS=-I$DIR/grib2/include
export PATH=$DIR/netcdf/bin:$PATH
export NETCDF=$DIR/netcdf
export HDF5=$DIR/hdf5
export LD_LIBRARY_PATH=$DIR/netcdf/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$DIR/hdf5/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$DIR/grib2/lib:$LD_LIBRARY_PATH

export MPI_DIR=$CBB_HOME/mpich
export MPI_VERSION=4.0.2

export PATH=$DIR/mpich/bin:$PATH 
export LD_LIBRARY_PATH=$MPI_DIR/lib:$LD_LIBRARY_PATH
export MANPATH=$MPI_DIR/share/man:$MANPATH
export C_INCLUDE_PATH=$MPI_DIR/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$MPI_DIR/include:$CPLUS_INCLUDE_PATH

export SZ3_HOME=$DIR/orisz3/install
export HDF5_PLUGIN_PATH=$SZ3_HOME/lib
export LD_LIBRARY_PATH=$SZ3_HOME/lib:$LD_LIBRARY_PATH
