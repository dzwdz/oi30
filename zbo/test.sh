set -eu
for f in $(find tc -name "*.in")
do
	echo $f ${f//.in/.out}
	# $* < $f > ${f//.in/.out}
	$* < $f | diff - ${f//.in/.out}
done
