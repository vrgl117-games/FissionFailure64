FROM ghcr.io/dragonminded/libdragon
ARG LIBDRAGON_COMMIT 07f1977bbb66a8f61d949983342c27915932d5a5 #05/27/26

RUN apt-get update -yq && apt-get install -yq imagemagick

RUN cd /tmp && git clone https://github.com/DragonMinded/libdragon.git && cd libdragon && git reset --hard $LIBDRAGON_COMMIT && make install && make tools-install

WORKDIR /game
