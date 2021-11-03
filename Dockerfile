FROM ghcr.io/dragonminded/libdragon

ARG LIBDRAGON_COMMIT 679bc1e263012d5d67d3aa92b26a56ade5a9316e
RUN apt-get update -yq
RUN apt-get install -yq imagemagick

RUN cd /tmp && git clone https://github.com/DragonMinded/libdragon.git && cd libdragon && git reset --hard $LIBDRAGON_COMMIT && make install && make tools-install

WORKDIR /game