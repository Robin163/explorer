#! /bin/bash
# format the files, not include lib path

for f in $(find ./ -name '*.c' -or -name '*.cpp' -or -name '*.h' -type f)
do
	if [ -f "$f" ]; then
    	astyle --options=./build/astyle.conf --preserve-date $f
	fi
done

# delete .orig files
for f in $(find ./ -name '*.orig' -type f)
do
     rm $f
done

