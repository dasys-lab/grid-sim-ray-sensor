For installing tmxparser:

    git clone git@github.com:dasys-lab/tmxparser.git

Inside the tmxparser repository:

    mkdir build
    cd build
    sudo cmake .. -DBUILD_TINYXML2=ON
    sudo make
    sudo make install
