clear
make -f Makefile

test_program_path="./life"

concurrency=$1

i=1
while [  $i -lt 5 ]; do
    j=0
    while [  $j -lt 3 ]; do
        $test_program_path <<< "START $i 48 12
			TIME 100
			EXIT
			"
	   	let j=j+1
    done
    let i=i+1

done


