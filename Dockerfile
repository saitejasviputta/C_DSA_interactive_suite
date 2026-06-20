# 1. Use the official lightweight Ubuntu image
FROM ubuntu:latest

# 2. Prevent interactive timezone/keyboard prompts from freezing the build
ENV DEBIAN_FRONTEND=noninteractive

# 3. Install the C compiler suite, Make, and Valgrind
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    valgrind \
    gdb \
    libncurses-dev \
    && rm -rf /var/lib/apt/lists/*

# 4. Set the working directory inside the isolated container
WORKDIR /app

# 5. Copy your entire repository source code into the container
COPY . .

# 6. Compile the project fresh inside the Linux container
RUN make clean && make

# 7. Execute the compiled binary
CMD ["./dsa"]