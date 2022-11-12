set -eu
for f in tc/*.in
do
	echo $f ${f//.in/.out}
	$* < $f | diff - ${f//.in/.out}
done
