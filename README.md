# Múltiplas Threads em C

Código do projeto final da matéria de Sistemas Operacionais (TT304A) da UNICAMP.

## 😵‍💫 Problema a ser resolvido 
Dadas três matrizes de entrada, 𝐴𝑛×𝑛,𝐵𝑛×𝑛 e 𝐶𝑛×𝑛, o programa deverá calcular inicialmente a matriz 𝐷𝑛×𝑛, tal que 𝐷𝑛×𝑛 = (𝐴𝑛×𝑛 + 𝐵𝑛×𝑛) e gravá-la em arquivo. Em seguida, o programa deverá calcular a matriz 𝐸𝑛×𝑛 tal que 𝐸𝑛×𝑛 = (𝐶𝑛×𝑛 × 𝐷𝑛×𝑛). Por último, a matriz 𝐸𝑛×𝑛 deve ser gravada em arquivo e reduzida por soma, isto é, todos os seus componentes devem ser somados, resultando em um único valor final.
Alguns passos ou tarefas podem executadas simultaneamente por mais de um thread. Assim, é possível definir três tipos de threads: de processamento (𝑡𝑝), de leitura (𝑡𝑙) e de escrita (𝑡𝑒).
Os threads de processamento (𝑡𝑝) e de leitura (𝑡𝑙) podem ser executados simultaneamente com qualquer outro tipo de thread. O thread de escrita (𝑡𝑒) só pode ser executado simultaneamente com outro thread de escrita se, e somente se, cada thread de escrita estiver gravando dados em arquivos diferentes.
A tabela a seguir indica os passos e tarefas que devem ser realizadas simultaneamente (com múltiplos threads), além do tipo e quantidade de threads.
| Passo | Tarefas | Tipos e Quantidades de Threads |
| :---: | :---: | :---: |
| Passo 1 | Leitura da Matriz A // Leitura da Matriz B | 2 threads do tipo 𝑡𝑙 |
| Passo 2 | Soma das Matrizes A + B = D | T threads do tipo 𝑡𝑝 |
| Passo 3 // Passo 4 | Gravação da Matriz D // Leitura da Matriz C | 1 thread do tipo 𝑡𝑒 e 1 thread do tipo 𝑡𝑙 |
| Passo 5 | Multiplicação das Matrizes D x C = E | T threads do tipo 𝑡𝑝 |
| Passo 6 // Passo 7 | Gravação da Matriz E // Redução da Matriz E | 1 thread do tipo 𝑡𝑒 e T threads do tipo 𝑡𝑝 |

## 💻 Requisitos do projeto

- Todas as matrizes precisam ser alocadas dinamicamente;
- Cada uma das matrizes necessárias para o programa precisa ser alocada em uma única etapa, isto é, em uma única chamada do comando malloc ou equivalente para cada matriz.
- O programa deve compilar sem erros;
- Os dados também devem estar armazenados em arquivos texto;
- Utilizar exatamente a linha de comando indicada -> <./programa T n arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat>;

## 🚀 Clonando o projeto

Para instalar o projeto:

```
git clone https://github.com/naferrett/projeto-SO.git
```

## ☕ Usando o projeto

Para usar o projeto, siga estas etapas:

1. Compile o projeto com o comando abaixo ou com o comando "make" para compilar com o Makefile
```
gcc -pthread projetoSO.c -o prog
```

2. Depois de compilado, execute o projeto com qualquer uma das 6 linhas de comando abaixo, considerando o tamanho da matriz e a quantidade de threads

Para testar a execução com uma matriz 100x100 com uma thread:
```
./prog 1 100 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```
Para testar a execução com uma matriz 100x100 com dois threads:
```
./prog 2 100 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```
Para testar a execução com uma matriz 100x100 com quatro threads:
```
./prog 4 100 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```
Para testar a execução com uma matriz 1000x1000 com uma thread:
```
./prog 1 1000 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```
Para testar a execução com uma matriz 1000x1000 com duas threads:
```
./prog 2 1000 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```
Para testar a execução com uma matriz 1000x1000 com quatro threads:
```
./prog 4 1000 arqA.dat arqB.dat arqC.dat arqD.dat arqE.dat
```

## 📊 Relatório do projeto

Para visualizar o relatório contendo a descrição do projeto, instruções para compilação, gráficos e análise dos resultados com a conclusão do experimento, acesse o arquivo contido no repositório.

## 📝 Orientação

Esse projeto foi realizado sob orientação do Prof. André Leon S. Gradvohl, Dr.
