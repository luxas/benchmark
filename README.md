# luxas/benchmark

Extends [Roy Longbottoms Raspberry Pi benchmarks](http://www.roylongbottom.org.uk/Raspberry Pi Benchmarks.htm).
This Docker image compiles and executes the most of them, and collects hand-picked data from the benchmarks.
Then it runs a C++ script, which summarizes and calculates the benchmark data.

**The reference point is 100 points and a Raspberry Pi 1 Model B should perform 100 in every test.**

So, this way you may roughly measure how fast other computers or ARM boards are compared to a RPi 1.
For the moment it benchmarks only single-core performance.

And seriously, you should take these benchmarks with a big grain of salt. But they're useful anyway.

## Running the benchmarks

The images are pushed to Docker Hub, so it's easy to run directly:

```console
$ docker run -it luxas/benchmark-arm
$ docker run -it luxas/benchmark-amd64
```

## Building the images

For amd64:
```console
$ make ARCH=amd64
```

For arm:
```console
$ make ARCH=arm
```

## Results

A results file will be in `/bench/results.json` inside the container.

Example output for Raspberry Pi 2:
```json
{
	"overall": "223",
	"cpu": "150",
	"ram": "298",
	"real": "222",
	"subPerf": {
		"floatingPoint_whetstone": "126",
		"integerPerf_dhrystone": "116",
		"linearAlgebraSinglePoint_linpack": "182",
		"linearAlgebraDoublePoint_linpack": "185",
		"kernelDoublePrecision_liverloops": "142",
		"L1CacheMFLOPS_l1flops": "130",
		"L2CacheMFLOPS_l2flops": "318",
		"RAMCacheSpeed_ramflops": "388",
		"RAMMIPS_rammips": "511",
		"wordReadingBusSpeed_busspeed": "144",
		"compileTimeAllBenchmarks": "222"
	},
	"rawData": {
		"floatingPoint_whetstone": "296",
		"integerPerf_dhrystone": "1010",
		"linearAlgebraSinglePoint_linpack": "104",
		"linearAlgebraDoublePoint_linpack": "78",
		"kernelDoublePrecision_liverloops": "87",
		"L1CacheMFLOPS_l1flops": "121",
		"L2CacheMFLOPS_l2flops": "121",
		"RAMCacheSpeed_ramflops": "70",
		"RAMMIPS_rammips": "686",
		"wordReadingBusSpeed_busspeed": "989",
		"compileTimeAllBenchmarks": "57"
	}
}
```

`overall` is the average value of `cpu`, `ram` and `real`
`cpu` is the average value of `floatingPoint_whetstone`, `integerPerf_dhrystone`, `linearAlgebraSinglePoint_linpack`, `linearAlgebraDoublePoint_linpack` and `kernelDoublePrecision_liverloops`
`ram` is the average value of `L1CacheMFLOPS_l1flops`, `L2CacheMFLOPS_l2flops`, `RAMCacheSpeed_ramflops`, `RAMMIPS_rammips` and `wordReadingBusSpeed_busspeed`
`real` is the value of `compileTimeAllBenchmarks`

`rawData` is the (sometimes calculated) output from Roy Longbottom benchmarks. Except for `compileTimeAllBenchmarks`, which represents how many seconds it took for the machine to compile the source code into binary files.


## Contribute

If you like more precise benchmarks and like the idea of running the same benchmarks on ARM, amd64 and other architectures, feel free to create issues and pull requests.