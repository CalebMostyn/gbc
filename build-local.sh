#!/usr/bin/env bash

# gbc-build:<version>
IMAGE_NAME="gbc-build:test"
CONTAINER_NAME="gbc-build-running"

BUILD_COMMANDS="
cd workspace/
make
"

echo "=== Running container: $CONTAINER_NAME ==="
docker run --rm -dit --name "$CONTAINER_NAME" -v "$PWD":/workspace "$IMAGE_NAME" bash

echo "=== Building inside container ==="
docker exec -it "$CONTAINER_NAME" bash -c "$BUILD_COMMANDS"

echo "=== Stopping container ==="
docker stop "$CONTAINER_NAME" >/dev/null &
