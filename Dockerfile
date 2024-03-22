FROM ubuntu:22.04 AS build

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

COPY . /app
WORKDIR /app/build

SHELL ["/bin/bash", "-c"]

RUN apt-get update && \
    apt-get install -y \
    python3-pip \
    cmake && \
    pip3 install conan --upgrade && \
    conan profile detect && \
    conan install .. --output-folder=. --build=missing && \ 
    source conanbuild.sh && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . && \
    source deactivate_conanbuild.sh

FROM ubuntu:22.04

RUN groupadd dev && useradd -g dev dev
USER dev
COPY --chown=dev:dev --from=build /app/build/lobby /app/lobby
COPY --chown=dev:dev --from=build /app/build/game /app/game

RUN apt-get update && \
    apt-get install -y supervisor

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

ENV HOME /app

CMD ["/usr/bin/supervisord"]

# CMD ["/app/lobby", "0.0.0.0", "8080"]
EXPOSE 8080