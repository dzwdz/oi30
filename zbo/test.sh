set -eu
for f in tc/*.in
do
	# echo $f ${f//.in/.out}
	./zbo < $f | diff - ${f//.in/.out}
done
