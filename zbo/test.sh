set -eu
for f in $(find tc -name "*.out")
do
	echo $f
	$* < ${f//.out/.in} | diff - $f
done
# for f in $(find tc -name "*.in")
# do
# 	echo $f ${f//.in/.out}
# 	# $* < $f > ${f//.in/.out}
# 	$* < $f | diff - ${f//.in/.out}
# done
