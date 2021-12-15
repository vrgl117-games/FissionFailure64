FROM ghcr.io/dragonminded/libdragon

RUN apt-get update -yq
RUN apt-get install -yq imagemagick

RUN cd /tmp && git clone https://github.com/Dragonminded/libdragon.git && cd libdragon && make install && make tools-install

WORKDIR /game