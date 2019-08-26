for file in instances/*
do
  ./tsp "$file" >> resultados.txt
done
