mkdir -p out/instances
for i in instances/*; do 
  echo "Testing $i"
  timeout 200s ./solver < "$i" | tee out/$i.out | ./checker
  cp tmp.png out/$i.png
done
