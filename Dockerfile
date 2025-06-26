# syntax=docker/dockerfile:1

ARG BASE_IMAGE=ubuntu:24.04
ARG WORKSPACE_IMAGE=workspace

FROM ${BASE_IMAGE} AS workspace

ARG TARGETARCH
LABEL org.opencontainers.image.source=https://github.com/meeq/FNaF64
LABEL org.opencontainers.image.description="Docker image for building the FNaF64 N64 ROM using dumped assets"

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm --force /etc/apt/apt.conf.d/docker-clean \
    && echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' \
        > /etc/apt/apt.conf.d/keep-cache \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get update --assume-yes \
    && apt-get install --assume-yes --no-install-recommends \
        build-essential \
        ca-certificates \
        ffmpeg \
        file \
        git \
        imagemagick \
        libpng-dev \
        rename \
        texinfo \
        wget

ENV N64_INST=/opt/libdragon

ADD https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-${TARGETARCH}.deb \
    /gcc-toolchain-mips64-${TARGETARCH}.deb

RUN apt install --assume-yes /gcc-toolchain-mips64-${TARGETARCH}.deb \
    && rm /gcc-toolchain-mips64-${TARGETARCH}.deb \
    && chown -R ubuntu:ubuntu $N64_INST

RUN mkdir /libdragon && chown ubuntu:ubuntu /libdragon
USER ubuntu:ubuntu

WORKDIR /libdragon
ARG LIBDRAGON_BUILD_DIR=/home/ubuntu/libdragon-build
COPY --chown=ubuntu:ubuntu ./libdragon ./
RUN make clobber \
    && make -j$(nproc) install tools-install

FROM ${WORKSPACE_IMAGE} AS build

WORKDIR /fnaf
COPY --chown=ubuntu:ubuntu copy_assets.sh Makefile ./
COPY --chown=ubuntu:ubuntu assets ./assets
COPY --chown=ubuntu:ubuntu include ./include
COPY --chown=ubuntu:ubuntu refs ./refs
COPY --chown=ubuntu:ubuntu src ./src
RUN --mount=type=bind,source=dump,target=./dump \
    ./copy_assets.sh dump/Images dump/Sounds \
    && make clean \
    && make -j$(nproc)

FROM scratch AS out

COPY --from=build /fnaf/fnaf.z64 /
