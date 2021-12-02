FROM ghcr.io/dragonminded/libdragon

RUN apt-get update -yq
RUN apt-get install -yq imagemagick

RUN cd /tmp && git clone https://github.com/rasky/libdragon.git && cd libdragon && git reset --hard de9345b30c101ed6122048ea57c1631c7e1bc8fc && make install && make tools-install

WORKDIR /game