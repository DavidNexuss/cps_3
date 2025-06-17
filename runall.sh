mkdir -p out/instances

trap "echo 'Aborted by user'; kill 0" SIGINT

rm tmp.png
rm tmp.dot
mkdir -p out/instances

for i in instances/*; do 
  RESULT=$(timeout 400s ./solver < "$i" 2> /dev/null| tee out/$i.out | ./checker)
  echo "$i $RESULT"
  cp tmp.png out/$i.png
done 2>&1 | tee summary.txt

rm tmp.png
rm tmp.dot
