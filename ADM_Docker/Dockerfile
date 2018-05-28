FROM ubuntu:latest
LABEL author="Pai Lee Wai" email="pailee.wai@gmail.com"

WORKDIR $HOME/Docker

# Aria2 dependencies
RUN apt-get update && \
    apt-get install -y \
    gettext nettle-dev libgmp3-dev \
    libssh-dev libc-ares-dev libxml2-dev zlib1g-dev \
    libsqlite3-dev pkg-config libgcrypt-dev libssl-dev \
    libexpat-dev gcc g++ libcppunit-dev autoconf \
    automake libtool make

# Install git
RUN apt-get install -y git

# Install wget
RUN apt-get install -y wget

# Install Nodejs
RUN wget https://nodejs.org/dist/v10.0.0/node-v10.0.0-linux-x64.tar.xz
RUN tar -xf node-v10.0.0-linux-x64.tar.xz --directory /usr/local --strip-components 1

# Install curl
RUN apt-get install -y curl

# Install yarn
RUN curl -o- -L https://yarnpkg.com/install.sh | bash

# Clone ADM
RUN git clone https://github.com/jk-gan/ADM.git
RUN cd ADM && git checkout develop

# Install node_modules
#RUN cd ADM && $HOME/.yarn/bin/yarn install --ignore-engines
RUN cd ADM && npm install

# Clone and build Aria2
ADD https://api.github.com/repos/aria2/aria2/git/refs/heads/master version.json
RUN cd ADM && cd aria2 && git clone https://github.com/aria2/aria2

RUN apt-get install -y autopoint

RUN cd ADM && cd aria2 && cd aria2 && autoreconf -i && ./configure --enable-libaria2 && make

RUN cd ADM && cd aria2 && cd aria2 && make install

# Electron dependencies
RUN apt-get install -y libgtk2.0-0
RUN apt-get install -y libx11-xcb-dev
RUN apt-get install -y libxtst6
RUN apt-get install -y libxss1
RUN apt-get install -y libgconf-2-4
RUN apt-get install -y libnss3-dev
RUN apt-get install -y build-essential clang libdbus-1-dev libgtk-3-dev \
    libnotify-dev libgnome-keyring-dev libgconf2-dev \
    libasound2-dev libcap-dev libcups2-dev libxtst-dev \
    libxss1 libnss3-dev gcc-multilib g++-multilib curl \
    gperf bison

RUN cd ADM && touch start.sh
RUN cd ADM && printf  "#!/bin/sh\n./ADM/node_modules/.bin/electron ./ADM/main.js" > start.sh
RUN cd ADM && chmod 777 start.sh

RUN cd ADM && npm run gyp-config
RUN cd ADM && npm run gyp-build
RUN cd ADM && npm run build

CMD ["./ADM/start.sh"]
