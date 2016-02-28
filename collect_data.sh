#!/bin/bash

PROJECT_ROOT=/bench
COMPILE_TIME_TEMP=${PROJECT_ROOT}/compiletime_temp.log
OUTPUT_COMPILE_TIME_DATA=${PROJECT_ROOT}/compiletime.log
OUTPUT_PERFORMANCE_DATA=${PROJECT_ROOT}/benchmarks.log

main(){
	cd "${PROJECT_ROOT}/source"

	# Set gcc flags based on architecture
	# for armv7 only use: "-lm -O3 -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard"
	case "$(uname -m)" in
		arm*)
			OPTS="-lm -O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard"
			;;
		amd64*)
			OPTS="-lm -O3"
			;;
		i?86_64*)
			OPTS="-lm -O3"
			;;
		x86_64*)
			OPTS="-lm -O3"
			;;
	esac

	# A two-dimensional array in bash
	BENCHMARKS=(
		"whets.c:cpuidc.c, whets, whets.txt"
		"linpack.c:cpuidc.c, linpackdp, Linpack.txt"
		"linpacksp.c:cpuidc.c, linpacksp, Linpack.txt"
		"dhry.h:dhry_1.c:dhry_2.c:cpuidc.c, dhrystone, Dhry.txt"
		"memspeed.c:cpuidc.c, memspeed, memSpeed.txt, MB:256"
		"lloops2.c:cpuidc.c, liverloops, LLloops.txt"
		"busspeed.c:cpuidc.c, busspeed, busSpeed.txt"
	)

	# Loop the benchmarks
	for STR in "${BENCHMARKS[@]}"; do
		
		# Split the string at ", "
		IFS=', ' read -a BENCHMARK <<< "$STR"

		# The first string is a list of files. Replace ":" with " "
		FILES="${BENCHMARK[0]}"
		FILES="${FILES//:/ }"

		# Get the binary name
		BINARY="${BENCHMARK[1]}"

		# Get the log file
		LOG="${BENCHMARK[2]}"

		# Get extra arguments, and split ":" into " "
		ARG="${BENCHMARK[3]}"
		ARG="${ARG//:/ }"

		# Compile the binary and 
		{ time gcc $FILES $OPTS -o $BINARY 2>&1; } 2>> $COMPILE_TIME_TEMP

		# Execute the benchmark
		./$BINARY $ARG <<EOF

EOF
		# Write data to the log file after it has been processed by a bash function with the same name as the binary
		echo -e "$($BINARY $LOG)\n" >> $OUTPUT_PERFORMANCE_DATA
		echo "Done with $BINARY"
	done

	cd $PROJECT_ROOT

	# Compile the aggregator
	{ time g++ -std=c++11 aggregate.cpp -o aggregate 2>&1; } 2>> $COMPILE_TIME_TEMP

	# Only use the "real" value field reported from the linux time command.
	cat $COMPILE_TIME_TEMP | grep "real" | grep -o "[0-9]m[0-9.]*" > $OUTPUT_COMPILE_TIME_DATA
	#| tr '\n' ' '

	# Calculate the results of the benchmarks
	./aggregate

	echo "Results are in results.json"

	cat results.json
}


whets(){
	cat $1 | grep "MWIPS" | awk '{print "WHETSTONE " $2 "\n"}'
}
dhrystone(){
	cat $1 | grep "MIPS rating" | grep -o "[0-9.]*" | awk '{print "DHRYSTONE " $1 "\n"}'
}
linpackdp(){
	cat $1 | grep "Speed" | awk '{print "LINPACKDP " $2 "\n"}'
	mv $1 linpackdp.txt
}
linpacksp(){
	cat $1 | grep "Speed" | awk '{print "LINPACKSP " $2 "\n"}'
	mv $1 linpacksp.txt
}
memspeed(){
	cat $1 | grep " 8 " | awk '{print "L1 " $2 " " $3 " " $5 " " $6 "\n"}'
	cat $1 | grep " 32 " | awk '{print "L2 " $2 " " $3 " " $5 " " $6 "\n"}' | grep "[a-z]" -v
	cat $1 | grep " 256 " | awk '{print "RAMLOW " $2 " " $3 " " $5 " " $6 "\n"}'
	cat $1 | grep " 262144 " | awk '{print "RAMHIGH " $2 " " $3 " " $5 " " $6 "\n"}'

	cat $1 | grep " 256 " | awk '{print "MIPSLOW " $4 " " $7 " " $10  "\n"}'
	cat $1 | grep " 262144 " | awk '{print "MIPSHIGH " $4 " " $7 " " $10  "\n"}'
}

liverloops(){
	cat $1 | grep -A2 "Overall Ratings" | grep "[0-9]" | awk '{print "LIVERLOOPS " $3 "\n"}'
}

busspeed(){
	cat $1 | grep " 16 " | awk '{print "BUSLOW " $7 "\n"}'
	cat $1 | grep " 65536 " | awk '{print "BUSHIGH " $7 "\n"}'
}

# Execute main
main
