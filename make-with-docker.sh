#!/usr/bin/env bash

set -e

CTFAK_RUNNER_IMAGE=${CTFAK_RUNNER_IMAGE:-ghcr.io/meeq/fnaf64-ctfak:1}
LIBDRAGON_WORKSPACE_IMAGE=${LIBDRAGON_WORKSPACE_IMAGE:-ghcr.io/meeq/fnaf64-libdragon:1}

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
    # Dump the game assets
    docker build \
        --file Dockerfile-ctfak \
        --build-arg "RUNNER_DEPS_IMAGE=$CTFAK_RUNNER_IMAGE" \
        --progress plain \
        --platform linux/amd64 \
        --target runner-out \
        --output type=local,dest=. \
        .
fi

# Build the ROM
docker build \
    --file Dockerfile \
    --build-arg "WORKSPACE_IMAGE=$LIBDRAGON_WORKSPACE_IMAGE" \
    --progress plain \
    --target out \
    --output type=local,dest=. \
    .
