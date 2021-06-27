FROM alpine:3.13

# install cmake
RUN apk update \
    && apk add cmake make gcc g++ libc-dev linux-headers

# prepare project files
WORKDIR /app

ADD . /app

# build server
RUN cmake .
RUN make server_exec

# Command to run the executable
CMD ["./src/server_exec"]
