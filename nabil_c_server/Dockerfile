FROM ubuntu:22.04

# Avoid prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools and MySQL development libraries [cite: 1, 15]
RUN apt-get update && apt-get install -y \
    build-essential \
    default-libmysqlclient-dev \
    gcc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY server.c .

# Explicitly link headers and libraries to solve the compilation error 
RUN gcc server.c -o server -lmysqlclient;

# Requirement: Port must be more than 1024 [cite: 39, 44]
EXPOSE 5001

CMD ["./server"]
