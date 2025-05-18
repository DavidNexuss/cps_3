mkdir -p out/instances

trap "echo 'Aborted by user'; kill 0" SIGINT

for i in instances/*; do 
  echo "Testing $i"
  timeout 200s ./solver < "$i" | tee out/$i.out | ./checker
  cp tmp.png out/$i.png
done 2>&1 | tee summary.txt
