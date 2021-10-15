FROM ghcr.io/dragonminded/libdragon

ARG LIBDRAGON_COMMIT 2bd68cf095f1317454033103ab88b436e2354183 #v10.2.1
RUN apt-get update -yq
RUN apt-get install -yq imagemagick

RUN cd /tmp && git clone https://github.com/DragonMinded/libdragon.git && cd libdragon && git reset --hard $LIBDRAGON_COMMIT && make install && make tools-install

WORKDIR /game