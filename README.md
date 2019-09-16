# Macaco
```python
import macaco as mc
```

### Instanciando um objeto da classe BananaFrame
```python
bf = mc.BananaFrame({
    'City': ['Buenos Aires', 'Brasilia', 'Santiago', 'Bogota', 'Caracas'],
    'Country': ['Argentina', 'Brazil', 'Chile', 'Colombia', 'Venezuela'],
    'Latitude': [-34.58, -15.78, -33.45, 4.60, 10.48],
    'Longitude': [-58.66, -47.91, -70.66, -74.08, -66.86]
})
```
No alto nível (Python) é feito a checagem da consistência do dicionário (consistência de tipos, keys e tamanho de values.
Caso haja inconsistência, o devido erro é acionado.

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

```python
# def display (self, n = 5, l = 10) 
# -> no máximo n colunas e l caracteres por célula
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
# def plot(self, xlabel, ylabel = False, sort = False, marker='o')
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
# Retorna formato do BananaFrame -> [axis0, axis1]
# len(bf) retorna só o axis1 (quantidade de colunas)
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

























