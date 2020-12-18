
for file in `find . -name "*.c" -type f`
do
    fileOut=`basename $file .c`
    echo $fileOut
    gcc -Wall -pedantic -o "$fileOut.out" "$file"
done
