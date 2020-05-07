for file in instances_teste/*
do
  ./tsp "$file" >> resultados.csv
done
