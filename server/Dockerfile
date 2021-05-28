FROM alpine:3.7

# install cmake
RUN apk update \
    && apk add cmake make gcc g++ libc-dev linux-headers

# prepare project files
WORKDIR /app

ADD . /app

# build server
RUN cmake .
RUN make

# Command to run the executable
CMD ["./server"]
