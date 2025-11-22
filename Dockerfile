FROM ubuntu:24.04

# Setup
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    # raylib dependencies
    libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev \
    libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev \
    # build tools
    git gcc build-essential make cmake ca-certificates curl unzip && \
    rm -rf /var/lib/apt/lists/* && \
    update-ca-certificates

# Install/Build Raylib
RUN git clone https://github.com/raysan5/raylib.git raylib && \
    cd raylib && \
    mkdir build && cd build && \
    cmake -DBUILD_SHARED_LIBS=OFF .. && \
    make && \
    make install && \
    ldconfig && \
    rm -rf raylib
