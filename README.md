# Macaco

### Dependências

Arquivos deste repositório: ```structure.cpp```, ```integration.cpp```, ```checks_and_error_handling.py``` e ```macaco.py```.

Módulos de C++(17): ```boost/python.hpp```, ```iostream```, ```vector``` e ```variant``` e ```stdlib.h```.

Módulos de Python(3): ```matplotlib```, ```tabulate```, ```copy``` e ```functools```.

### Compile

g++ -fpic -c -o integration.o integration.cpp -std=c++17 -I/path/to/python/include -I/path/to/boost/include -L/path/to/python/lib -L/path/to/boost/lib

```
Exemplo:

-I/usr/local/Cellar/python/3.7.3/Frameworks/Python.framework/Versions/3.7/Headers

-I/usr/local/Cellar/boost/1.71.0/include/ 

-L/usr/local/Cellar/python/3.7.3/Frameworks/Python.framework/Versions/3.7/lib/

-L/usr/local/Cellar/boost/1.71.0/lib/ 
```

g++ -o integration.so -shared integration.o -lboost_python37 -lpython3.7 -std=c++17


### Instanciando um objeto da classe BananaFrame

```python
import macaco as mc

# A partir de um arquivo csv
bf = mc.read_csv('iris.csv')

# A partir de um dicionário
bf = mc.BananaFrame({
    'City': ['Buenos Aires', 'Brasilia', 'Santiago', 'Bogota', 'Caracas'],
    'Country': ['Argentina', 'Brazil', 'Chile', 'Colombia', 'Venezuela'],
    'Latitude': [-34.58, -15.78, -33.45, 4.60, 10.48],
    'Longitude': [-58.66, -47.91, -70.66, -74.08, -66.86]
})
```
No alto nível (Python) é feito a checagem da consistência do dicionário (consistência de tipos, keys e tamanho de values.
Caso haja inconsistência, o devido erro é acionado.

Obs: Tipos aceitos: string, float e int. Cada coluna só pode haver um tipo de dado

Variáveis privadas são criadas como o formato que o BananaFrame terá, os tipos, e um dicionários para traduzir o nome das colunas para o número da sua posição (essa informação é passada para o baixo nível para poder retornar uma determinada coluna).

Métodos são criados com os nomes das colunas para que possa utilizar como: 

```python
bf.City
```

```
╒══════════╤════════════╤══════════╤══════════╤════════╤═════════╕
│ City(s): │ Buenos Air │ Brasilia │ Santiago │ Bogota │ Caracas │
╘══════════╧════════════╧══════════╧══════════╧════════╧═════════╛
 macaco.BananaFrame.__serie
```


Finalmente é instanciado e armazenado como atributo privado um objeto ```integration.BananaFrame```. O objeto é inicializado no baixo nível (C++) com o tamanho do BananaFrame e os tipos das colunas.

Obs: Foram utilizadas as bibliotecas, em C++, ```<boost/python.hpp>``` para a conexão Python-C++ e ```<variant>``` para poder existir colunas de tipos diferentes em um BananaFrame.

Novamente em Python, o dicionário do parâmetro é enviado coluna por coluna para o objeto ```integration.BananaFrame```, onde cada um é indexado e armazenado no baixo nível.

Todas as colunas já estão automaticamente indexadas para melhor desempenho, como custo a ocupação da estrutura na memória é triplicado.

A indexação é realizada armazenando um vetor ordenado com os valores da coluna, um vetor de inteiros para restaurar a posição original destes valores e adicionando os dados para os vetores de linhas, que são utilizados para retornar os dados do BananaFrame.

Obs: Foi cogitado utilizar Red Black Tree para indexar as colunas, mas a complexidade de uma ```slice``` (remover linhas do BananaFrame) é ```O(nlogn)``` (buscar, no máximo, ```n``` linhas e remove-las (```logn```)), enquanto de um vetor ordenado a complexidade é ```O(n)``` (armazenar, no máximo, ```n``` linhas que não serão removidas)

### Métodos e atributos de um BananaFrame

#### - Vizualizar os dados:

```python
# bf.display (n = 5, l = 10) -> no máximo n colunas e l caracteres por célula
bf.display(3, 6) 
```

```
╒═════╤════════╤═══════════╤════════════╤═════════════╕
│     │ City   │ Country   │ Latitude   │ Longitude   │
╞═════╪════════╪═══════════╪════════════╪═════════════╡
│ 0   │ Buenos │ Argent    │ -34.58     │ -58.65      │
├─────┼────────┼───────────┼────────────┼─────────────┤
│ 1   │ Brasil │ Brazil    │ -15.77     │ -47.90      │
├─────┼────────┼───────────┼────────────┼─────────────┤
│ 2   │ Santia │ Chile     │ -33.45     │ -70.66      │
├─────┼────────┼───────────┼────────────┼─────────────┤
│ ... │ ...    │ ...       │ ...        │ ...         │
╘═════╧════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame
```

```python
# bf.plot(xlabel, ylabel = False, sort = False, marker='o')
# -> se ylabel = False, utiliza os indexes no eixo y
# -> especificar qual eixo ordenar (0 ou 1) em 'sort
bf.plot('Country', 'Latitude', sort=1, marker='o')
```
![GitHub Logo](/images/image1)
```python
bf.plot('Longitude', sort=0, marker='-')
```
![GitHub Logo](/images/image2)

#### - Atributos públicos:

```Python
# Retorna formato do BananaFrame -> [axis0, axis1] (linhas, colunas)
bf.size 
```
```
[5, 4]
```

```python
# Retorna os nomes das colunas
bf.column_names
```

```
['City', 'Country', 'Latitude', 'Longitude']
```

```python
# Retorna os indexes
bf.indexes
```

```
[0, 1, 2, 3, 4]
```

```python
# Retorna os tipos das colunas
bf.types
```

```
[str, str, float, float]
```

```Python 
# bf.'nome_da_coluna' -> Retorna a coluna como uma serie (logo as series serão explicadas) 
serie = bf.City # Equivalente a bf['City']
print(serie)
```

```
╒══════════╤════════════╤══════════╤══════════╤════════╤═════════╕
│ City(s): │ Buenos Air │ Brasilia │ Santiago │ Bogota │ Caracas │
╘══════════╧════════════╧══════════╧══════════╧════════╧═════════╛
 macaco.BananaFrame.__serie
```

```Python
# Retorna uma cópia identica do BananaFrame
# Alterações na cópia ou no original não afetará o outro objeto 
bf2 = bf.copy
bf2.display()
```

```
╒════╤════════════╤═══════════╤════════════╤═════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │
╞════╪════════════╪═══════════╪════════════╪═════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  1 │ Brasilia   │ Brazil    │     -15.78 │      -47.91 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  2 │ Santiago   │ Chile     │     -33.45 │      -70.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  3 │ Bogota     │ Colombia  │       4.6  │      -74.08 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  4 │ Caracas    │ Venezuela │      10.48 │      -66.86 │
╘════╧════════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame
 
```


#### - Operadores:

```Python
# Retorna uma lista com os valores de uma determinada linha
# key do tipo int
bf[0] # linha de index 0
```

```
['Buenos Aires', 'Argentina', -34.58000183105469, -58.65999984741211, 44]
```

```python
# Equivalente a bf.Latitude
# key do tipo string
bf['Latitude']
```

```
╒══════════════╤════════╤════════╤════════╤═════╤═══════╕
│ Latitude(s): │ -34.58 │ -15.78 │ -33.45 │ 4.6 │ 10.48 │
╘══════════════╧════════╧════════╧════════╧═════╧═══════╛
 macaco.BananaFrame.__serie
```

```Python
# Retorna uma cópia do BananaFrame somentre com determindas linhas
# key do tipo lista de int
bf2 = bf[[0, 1, 4]]
bf2.display()
```

```
╒════╤════════════╤═══════════╤════════════╤═════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │
╞════╪════════════╪═══════════╪════════════╪═════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  1 │ Brasilia   │ Brazil    │     -15.78 │      -47.91 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  2 │ Caracas    │ Venezuela │      10.48 │      -66.86 │
╘════╧════════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame
```

```Python
# Equivalente a bf[[0, 2]], mas é retornado a intercessão das listas
# key do tipo list de lists de int
bf2 = bf[[0, 2, 4], [0, 1, 2], [0, 2, 3, 4]]
bf2.display()
```

```
╒════╤════════════╤═══════════╤════════════╤═════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │
╞════╪════════════╪═══════════╪════════════╪═════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  1 │ Santiago   │ Chile     │     -33.45 │      -70.66 │
╘════╧════════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame
```

```Python
# Retorna a quantidade de colunas (axis1)
len(bf)
```

```
4
```

```python
# quivalente a bf.display(5, 10)
bf  # ou print(bf)
```

```
╒════╤════════════╤═══════════╤════════════╤═════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │
╞════╪════════════╪═══════════╪════════════╪═════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  1 │ Brasilia   │ Brazil    │     -15.78 │      -47.91 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  2 │ Santiago   │ Chile     │     -33.45 │      -70.66 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  3 │ Bogota     │ Colombia  │       4.6  │      -74.08 │
├────┼────────────┼───────────┼────────────┼─────────────┤
│  4 │ Caracas    │ Venezuela │      10.48 │      -66.86 │
╘════╧════════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame
```

```Python
# Iterar sobre um bf retorna as series (colunas)
for serie in bf:
    print(serie)
```

```
╒══════════╤════════════╤══════════╤══════════╤════════╤═════════╕
│ City(s): │ Buenos Air │ Brasilia │ Santiago │ Bogota │ Caracas │
╘══════════╧════════════╧══════════╧══════════╧════════╧═════════╛
 macaco.BananaFrame.__serie

╒═════════════╤═══════════╤════════╤═══════╤══════════╤═══════════╕
│ Country(s): │ Argentina │ Brazil │ Chile │ Colombia │ Venezuela │
╘═════════════╧═══════════╧════════╧═══════╧══════════╧═══════════╛
 macaco.BananaFrame.__serie

╒══════════════╤════════╤════════╤════════╤═════╤═══════╕
│ Latitude(s): │ -34.58 │ -15.78 │ -33.45 │ 4.6 │ 10.48 │
╘══════════════╧════════╧════════╧════════╧═════╧═══════╛
 macaco.BananaFrame.__serie

╒═══════════════╤════════╤════════╤════════╤════════╤════════╕
│ Longitude(s): │ -58.66 │ -47.91 │ -70.66 │ -74.08 │ -66.86 │
╘═══════════════╧════════╧════════╧════════╧════════╧════════╛
 macaco.BananaFrame.__serie
```
 
```Python
# Checa se alguma coluna possui o nome dado
'City' in bf
```
 
```
True
```
 
#### - Métodos públicos:

```Python
# Adicionar colunas a patir de um dicionário
dic = {'PopulationSize':[44, 209, 18, 49, 32]}
bf.add_columns(dic)
bf
``` 

```
╒════╤════════════╤═══════════╤════════════╤═════════════╤══════════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │   PopulationSize │
╞════╪════════════╪═══════════╪════════════╪═════════════╪══════════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │               44 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  1 │ Brasilia   │ Brazil    │     -15.78 │      -47.91 │              209 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  2 │ Santiago   │ Chile     │     -33.45 │      -70.66 │               18 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  3 │ Bogota     │ Colombia  │       4.6  │      -74.08 │               49 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  4 │ Caracas    │ Venezuela │      10.48 │      -66.86 │               32 │
╘════╧════════════╧═══════════╧════════════╧═════════════╧══════════════════╛
 macaco.BananaFrame
```

```Python
# Retorna a coluna como uma lista (não serie)
bf.take_column('Country')
```

```
['Argentina', 'Brazil', 'Chile', 'Colombia', 'Venezuela']
```

```Python
# Equivalente a bf['PopulationSize']
serie = bf.column('PopulationSize')
print(serie)
```

```
╒════════════════════╤════╤═════╤════╤════╤════╕
│ PopulationSize(s): │ 44 │ 209 │ 18 │ 49 │ 32 │
╘════════════════════╧════╧═════╧════╧════╧════╛
 macaco.BananaFrame.__serie
``` 

```Python
# Equivalente a bf[1]
bf.row(1) # linha de index 0
```

```
['Brasilia', 'Brazil', -15.779999732971191, -47.90999984741211, 209]
```


```Python
# Equivalente a bf[[1,2,3]]
bf2 = bf.slices([1,2,3])
bf2
```

```
╒════╤══════════╤═══════════╤════════════╤═════════════╤══════════════════╕
│    │ City     │ Country   │   Latitude │   Longitude │   PopulationSize │
╞════╪══════════╪═══════════╪════════════╪═════════════╪══════════════════╡
│  0 │ Brasilia │ Brazil    │     -15.78 │      -47.91 │              209 │
├────┼──────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  1 │ Santiago │ Chile     │     -33.45 │      -70.66 │               18 │
├────┼──────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  2 │ Bogota   │ Colombia  │       4.6  │      -74.08 │               49 │
╘════╧══════════╧═══════════╧════════════╧═════════════╧══════════════════╛
 macaco.BananaFrame
 ```

```
# Equivalente a bf = bf.slices([0,1,3,4]), o objeto é alterado
bf.cut([0,1,3,4])
bf
```

```
╒════╤════════════╤═══════════╤════════════╤═════════════╤══════════════════╕
│    │ City       │ Country   │   Latitude │   Longitude │   PopulationSize │
╞════╪════════════╪═══════════╪════════════╪═════════════╪══════════════════╡
│  0 │ Buenos Air │ Argentina │     -34.58 │      -58.66 │               44 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  1 │ Brasilia   │ Brazil    │     -15.78 │      -47.91 │              209 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  2 │ Bogota     │ Colombia  │       4.6  │      -74.08 │               49 │
├────┼────────────┼───────────┼────────────┼─────────────┼──────────────────┤
│  3 │ Caracas    │ Venezuela │      10.48 │      -66.86 │               32 │
╘════╧════════════╧═══════════╧════════════╧═════════════╧══════════════════╛
 macaco.BananaFrame
```

### - Buscas

Complexidades: O(logn) (busca binária no vetor ordenado)

```Python
# Dado o nome de uma coluna e uma key (do tipo dos dados da coluna), 
# é retornado uma lista com os índices de todas as linhas em que
# o valor naquela coluna é igual a key.
bf.query('Latitude', -15.78)
```

```
[1]
```

```Python
# Dado o nome de uma coluna, um min e um max (estes do tipo dos dados da coluna), 
# é retornado uma  lista com os índices de todas as linhas em que
# o valor naquela coluna é igual ou está entre o min e o max dados
bf.between('Longitude', -74.08, -58.66)
```

```
[0, 2, 3]
```

## Series

Serie é uma classe interna do BananaFrame, tal possui seus próprios atributos, métodos e operadores.

```
# Objeto da classe serie
serie = bf.Latitude
print(serie)
```

```
╒══════════════╤════════╤════════╤═════╤═══════╕
│ Latitude(s): │ -34.58 │ -15.78 │ 4.6 │ 10.48 │
╘══════════════╧════════╧════════╧═════╧═══════╛
 macaco.BananaFrame.__serie
```

### - Atributos e métodos públicos:

```Python
# Retorna o nome da serie
serie.name
```

```
'Latitude'
```

```Python
# Tamanho
serie.size
```

```
4
```

```Python
# Lista com o valores da serie
serie.values
```

```
[-34.58, -15.78, 4.6, 10.48]
```


```python
# serie.display (n = 5, l = 10) -> no máximo n colunas e l caracteres por célula
bf.display(3, 6) 
```


```
╒══════════════╤════════╤════════╤═════╤═════╕
│ Latitude(s): │ -34.58 │ -15.78 │ 4.6 │ ... │
╘══════════════╧════════╧════════╧═════╧═════╛
 macaco.BananaFrame.__serie
```

### - Operadores:

```Python
# Equivalente a serie.size
len(serie)
```

```
4
```

```Python
# Equivalente a serie.display()
serie # ou print(serie)
```

```
╒══════════════╤════════╤════════╤═════╤═══════╕
│ Latitude(s): │ -34.58 │ -15.78 │ 4.6 │ 10.48 │
╘══════════════╧════════╧════════╧═════╧═══════╛
 macaco.BananaFrame.__serie
```

```Python
# Retorna se o valor faz parte da serie
4.6 in serie
```

```
True
```


```Python
# Iterar sobre a serie retorna os valores
for value in serie:
    print(value)
```

```
-34.58
-15.78
4.6
10.48
```

```Python
# Equivalente a bf.query(serie.name, key), key = 4.6
serie == 4.6
```

```
[2]
```

```Python
# Equivalente a bf.between('Longitude', min, max), min, max = (-15.78, 4.6)
serie >> (-15.78, 4.6)
```

```
[1, 2]
```

## Conclusão

Misturando os operadores de um BananaFrame e de suas series é possível:

```
# Uma ou mais query de forma elegante
bf[bf.Latitude == -33.45, 
   bf.Longitude >> (-71.0, 0.0),
   bf.Country == 'Chile']
```

```
╒════╤══════════╤═══════════╤════════════╤═════════════╕
│    │ City     │ Country   │   Latitude │   Longitude │
╞════╪══════════╪═══════════╪════════════╪═════════════╡
│  0 │ Santiago │ Chile     │     -33.45 │      -70.66 │
╘════╧══════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame

```

```
# Busca geometrica
bf[bf.Latitude >> (-15.78, 10.0), 
   bf.Longitude >> (-80.0, 0.0)]
```

```
╒════╤══════════╤═══════════╤════════════╤═════════════╕
│    │ City     │ Country   │   Latitude │   Longitude │
╞════╪══════════╪═══════════╪════════════╪═════════════╡
│  0 │ Brasilia │ Brazil    │     -15.78 │      -47.91 │
├────┼──────────┼───────────┼────────────┼─────────────┤
│  1 │ Bogota   │ Colombia  │       4.6  │      -74.08 │
╘════╧══════════╧═══════════╧════════════╧═════════════╛
 macaco.BananaFrame

```

### Feito por William Sena como trabalho do curso Estrutura de Dados e Algoritmos do Mestrado EMAp/FGV com o professor Jorge Poco
