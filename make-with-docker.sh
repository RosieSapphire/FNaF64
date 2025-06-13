#!/usr/bin/env bash

set -e

if [ ! -f FiveNightsatFreddys.exe ]; then
    echo "ERROR: FiveNightsatFreddys.exe must be in the project directory!"
    exit 1
fi

if [ ! -d CTFAK2.0 ] || [ ! -d libdragon ]; then
    git submodule update --init
fi

if [ "$(uname -m)" != "x86_64" ]; then
    echo "WARNING: This project is only supported on x86_64 systems."
    echo "         You may encounter issues building on other platforms!"
fi

if [ ! -d dump ]; then
    # Build CTFAK.Cli
    if [ ! -d CTFAK.Cli ]; then
        docker build \
            --file Dockerfile-ctfak \
            --progress plain \
            --platform linux/amd64 \
            --target out \
            --output type=local,dest=. \
            .
    fi

    # Dump the game assets
    docker build \
        --file Dockerfile-dump \
        --progress plain \
        --platform linux/amd64 \
        --target out \
        --output type=local,dest=. \
        .
fi

# Build the ROM
docker build \
    --file Dockerfile \
    --progress plain \
    --platform linux/amd64 \
    --target out \
    --output type=local,dest=. \
    .
