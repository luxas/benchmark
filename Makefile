ARCH?=amd64
TEMP_DIR:=$(shell mktemp -d)
TAG=0.1.1

# Default base images for different architectures
# '/' in images has to be written as '\\/'
ifeq ($(ARCH),amd64)
	BASEIMAGE?=debian:jessie
endif
ifeq ($(ARCH),arm)
	BASEIMAGE?=resin\\/rpi-raspbian:jessie
endif

build:
	cp -r ./* ${TEMP_DIR}
	cd ${TEMP_DIR} && sed -i "s/BASEIMAGE/${BASEIMAGE}/g" Dockerfile
	cd ${TEMP_DIR} && sed -i "s/ARCH/${ARCH}/g" Dockerfile

ifeq ($(ARCH),amd64)
	# When building "normally" for amd64, remove the whole line, it has no part in the amd64 image
	cd $(TEMP_DIR) && sed -i "/CROSS_BUILD_/d" Dockerfile
else
	# When cross-building, only the placeholder "CROSS_BUILD_" should be removed
	# Register /usr/bin/qemu-ARCH-static as the handler for ARM binaries in the kernel
	docker run --rm --privileged multiarch/qemu-user-static:register --reset
	curl -sSL https://github.com/multiarch/qemu-user-static/releases/download/v2.5.0/x86_64_qemu-${ARCH}-static.tar.xz | tar -xJ -C ${TEMP_DIR}
	cd $(TEMP_DIR) && sed -i "s/CROSS_BUILD_//g" Dockerfile
endif

	docker build -t luxas/benchmark-$(ARCH):$(TAG) $(TEMP_DIR)
	docker tag -f luxas/benchmark-$(ARCH):$(TAG) luxas/benchmark-$(ARCH)

push: build
	docker push luxas/benchmark-$(ARCH):$(TAG)
	docker push luxas/benchmark-$(ARCH)

run:
	docker run -it luxas/benchmark-$(ARCH):$(TAG)

# Default action is build
all: build