cd $DIR
cd orisz3
mkdir install
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=$DIR/orisz3/install -DBUILD_H5Z_FILTER=ON ..
export SZ3_HOME=$DIR/orisz3/install 
make -j 8
make install
cd $DIR
