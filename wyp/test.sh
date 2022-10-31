set -eu
for f in tc/*.in; do
	# echo $f
	# ./wyp < $f > ${f//in/out}
	./wyp < $f | diff --strip-trailing-cr - ${f//in/out}
done
