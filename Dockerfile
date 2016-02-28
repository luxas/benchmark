FROM BASEIMAGE

CROSS_BUILD_COPY qemu-ARCH-static /usr/bin/

RUN apt-get update && apt-get install -y \
		build-essential \
		gcc \
		unzip \
		curl \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /bench

RUN curl -sSL http://www.roylongbottom.org.uk/Raspberry_Pi_Benchmarks.zip > benchmarks.zip \
	&& unzip benchmarks.zip \
	&& mv "Raspberry_Pi_Benchmarks/Source Code" source \
	&& rm -r benchmarks.zip Raspberry_Pi_Benchmarks

COPY collect_data.sh aggregate.cpp ./

CMD ["./collect_data.sh"]
