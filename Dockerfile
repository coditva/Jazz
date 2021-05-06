FROM ubuntu:xenial

# Configure environment for tzdata
RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
ENV DEBIAN_FRONTEND noninteractive

# Install build tools
RUN apt-get update -y \
    && apt-get install -y --no-install-recommends \
        build-essential \
        nasm \
        flex \
        bison \
        texinfo \
        wget \
        curl \
        ca-certificates \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy code
COPY . /jazz
WORKDIR /jazz

# Build dependencies
RUN make deps

# Build
ENTRYPOINT ["make"]
