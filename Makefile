# remove the # in the following line to enable reorg compilation (and running)
all: cruncher reorg

cruncher: cruncher.c utils.h
	gcc -I. -O3 -o cruncher cruncher.c 

loader: loader.c utils.h
	gcc -I. -O3 -o loader loader.c 

reorg: reorg.c utils.h
	gcc -I. -O3 -o reorg reorg.c 

clean:
	rm -f loader cruncher reorg time.txt
	rm -Rf dataset-sf100
	rm -Rf dataset-sf100-copy
	rm -Rf dataset-sf3000
	rm -Rf dataset-sf3000-copy


getdata100:
	wget -r --no-parent -A 'bin' http://wikistats.ins.cwi.nl/lsde-data/practical_1/dataset-sf100/
	mv wikistats.ins.cwi.nl/lsde-data/practical_1/dataset-sf100 ./
	cp -R dataset-sf100 dataset-sf100-copy
	rm -Rf wikistats.ins.cwi.nl

getdata3000:
	wget -r --no-parent -A 'bin' http://wikistats.ins.cwi.nl/lsde-data/practical_1/dataset-sf3000/
	mv wikistats.ins.cwi.nl/lsde-data/practical_1/dataset-sf3000 ./
	cp -R dataset-sf3000 dataset-sf3000-copy
	rm -Rf wikistats.ins.cwi.nl

run100:
	echo "Running reorg" > time.txt
	{ time ./reorg dataset-sf100 ; } 2>> time.txt
	mv dataset-sf100/person2.bin dataset-sf100/person.bin
	mv dataset-sf100/knows2.bin dataset-sf100/knows.bin
	echo "\nRunning queries" >> time.txt
	{ time ./cruncher dataset-sf100 queries-test.csv out.csv ; } 2>> time.txt
	diff out.csv queries-test-output-sf100.csv
	rm -Rf dataset-sf100
	cp -R dataset-sf100-copy dataset-sf100

run3000:
	echo "Running reorg" > time.txt
	{ time ./reorg dataset-sf3000 ; } 2>> time.txt
	mv dataset-sf3000/person2.bin dataset-sf3000/person.bin
	mv dataset-sf3000/knows2.bin dataset-sf3000/knows.bin
	echo "\nRunning queries" >> time.txt
	{ time ./cruncher dataset-sf3000 queries-test.csv out.csv ; } 2>> time.txt
	diff out.csv queries-test-output-sf3000.csv
	rm -Rf dataset-sf3000
	cp -R dataset-sf3000-copy dataset-sf3000
